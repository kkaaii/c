#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mysql.h"

#include "ifcfg.h"
#include "iptables.h"

#pragma GCC diagnostic ignored "-Wpedantic"

#define	FORWARDIP	"10.105.1.200"

#define	MAXBUF		1024

MYSQL	*conn;
char	sql[MAXBUF + 1];

struct global {
	const char	*Fip;
} g = {
	.Fip	= FORWARDIP
};

#if 1
int (*System)(const char * command) = NULL;
#else
int (*System)(const char * command) = system;
#endif

static const char *GetVtype(char *Vtype, const char *req)
{
	if ('V' != *req++)
		return NULL;

	if ('=' != *req++)
		return NULL;

	if ('\0' != *req)
		*Vtype = *req++;

	return req;
}

static const char *GetSip(char *Sip, const char *req)
{
	if ('I' != *req++)
		return NULL;

	if ('=' != *req++)
		return NULL;

	while ('\0' != *req && 'P' != *req) {
		*Sip++ = *req++;
	}
	*Sip = '\0';

	return req;
}

static void GetVports(char (*Vports)[MAXBUF + 1], const char *req)
{
	char	*p;

	if ('\0' != *req)
		++req;

	while ('\0' != *req) {
		++req;
		p = *Vports++;
		while ('\0' != *req && '+' != *req) {
			*p++ = *req++;
		}
		*p = '\0';
	}
}

void ParseRequest(const char *req, char *Vtype, char *Sip, char (*Vports)[MAXBUF + 1])
{
	if (NULL == (req = GetVtype(Vtype, req)))
		return;

	if (NULL == (req = GetSip(Sip, req)))
		return;

	GetVports(Vports, req);
}

void QueryMysql(void)
{
	if (mysql_query(conn, sql)) {
		exit(0);
	}	
}

MYSQL_RES *QueryMysqlWithRes(void)
{
	MYSQL_RES	*res;

	QueryMysql();
	res = mysql_store_result(conn);
	if (0 == mysql_num_rows(res))
		return NULL;

	return res;
}

void database_enableVip(const char *Vip)
{
    sprintf(sql, "update useip set Vflag=1 where Vip='%s'", Vip);
    QueryMysql();
}

static void database_createVip(const char *Vip, const char *Sip, const char *Sport)
{
	sprintf(sql, "insert into server (Vip,Sip,Sport) value('%s','%s','%s')", Vip, Sip, Sport);
	QueryMysql();
}

static void Append(const char *Vip, const char *Sip, const char *Sport)
{
	iptables_add(Vip, Sip, Sport, g.Fip);
	database_createVip(Vip, Sip, Sport);
}

typedef void (*Callback)(const char *Vip, const char *Sip, const char *Sport);
void ForeachPort(const char *Vip, const char *Sip, const char (*Vports)[MAXBUF + 1], Callback append)
{
	MYSQL_RES	*res;

	for (; strlen(*Vports) > 0; ++Vports) {
		sprintf(sql, "select * from server where Sip='%s' and Sport='%s'", Sip, *Vports);
		if (NULL == (res = QueryMysqlWithRes())) {
			if (append) {
				(*append)(Vip, Sip, *Vports);
			}
		} else {
/*
			if (remove) {
				(*remove)(Vip, Sip, *Vports);
			}
*/		}
	}
}

int Case0(const char *Sip)
{
	MYSQL_RES	*res;
	MYSQL_ROW	row;

	sprintf(sql, "select Vip,Sip,Sport from server where Sip='%s'", Sip);
	if (NULL == (res = QueryMysqlWithRes()))
		return 0;

	row = mysql_fetch_row(res);
	sprintf(sql, "delete from server where Sip='%s'", Sip);
	QueryMysql();

	iptables_del(row[0], row[1], row[2], g.Fip);

	return 1;
}

int Case1(const char *Sip, const char (*Vports)[MAXBUF + 1])
{
	MYSQL_RES	*res;
	MYSQL_ROW	row;

	if (0 == strlen(Vports[0]))
		return 0;

	sprintf(sql, "Select Vip from server where Sip='%s'", Sip);
	if (NULL != (res = QueryMysqlWithRes())) {
		row = mysql_fetch_row(res);
	} else {
		sprintf(sql, "select Vip,NetMask from useip where Vflag=0 and Fzone=0 limit 0,1");
		if (NULL == (res = QueryMysqlWithRes()))
			return 0;

		row = mysql_fetch_row(res);
		ifcfg_add(row[0], row[1]);
		database_enableVip(row[0]);
	}

	ForeachPort(row[0], Sip, Vports, Append);
	return 1;
}

int Case3(const char *Sip, const char (*Vports)[MAXBUF + 1])
{
	MYSQL_RES	*res;
	MYSQL_ROW	row;

	sprintf(sql, "Select Vip from server where Sip='%s'", Sip);
	if (NULL == (res = QueryMysqlWithRes()))
		return 0;

	row = mysql_fetch_row(res);
	ForeachPort(row[0], Sip, Vports, Append);
	return 1;
}

int Case4(char *req, const char *Sip)
{
	MYSQL_RES	*res;
	MYSQL_ROW	row;

	sprintf(sql, "select Vip,Sport from server where Sip='%s'", Sip);
	if (NULL == (res = QueryMysqlWithRes()))
		return 0;

	row = mysql_fetch_row(res);
	sprintf(req, "OK! V=%sI=%sP=%s", row[0], Sip, row[1]);
	while (NULL != (row = mysql_fetch_row(res))) {
		strcat(req, "+");
		strcat(req, row[1]);
	}

	return 1;
}

int Case5(const char *Sip, const char (*Vports)[MAXBUF + 1])
{
	MYSQL_RES	*res;
	MYSQL_ROW	row;
	const char	*Vip = Vports[0];

	if (0 == strlen(Vip))
		return 0;

	sprintf(sql, "select Sport from server where Vip='%s' and Sip='%s'", Vip, Vports[1]);
	if (NULL == (res = QueryMysqlWithRes()))
		return 0;

	while (NULL != (row = mysql_fetch_row(res))) {
		iptables_del(Vip, Vports[1], row[0], g.Fip);
		iptables_add(Vip, Sip, row[0], g.Fip);
	}

	sprintf(sql, "update server set Sip='%s' where Vip='%s' and Sip='%s'", Sip, Vip, Vports[1]);
	QueryMysql();
	return 1;
}

