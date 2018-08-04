#include <stdio.h>

#include "gen_forward.h"
#include "database.h"
#include "ifcfg.h"
#include "iptables.h"

void DelVip(const char *Vip)
{
	MYSQL_RES	*res;
	MYSQL_ROW	row;
	char		cmd[MAXBUF + 1];

	db_query("select NetMask from useip where Vip='%s'", Vip);
	res = mysql_store_result(conn);

	if (mysql_num_rows(res) > 0) {
		row = mysql_fetch_row(res);
		ifcfg_del(cmd, Vip, row[0]);
		System(cmd);
	}

	mysql_free_result(res);
}

void AddVip(const char *Vip, const char *Sip, const char *Sport, const char *Fip)
{
	MYSQL_RES	*res;

	db_query("select * from server where Sip='%s' and Sport='%s'", Sip, Sport);
	res = mysql_store_result(conn);

	if (mysql_num_rows(res) <= 0) {
		iptables_add(Vip, Sip, Sport, Fip);
		db_query("insert into server (Vip,Sip,Sport) value('%s','%s','%s')", Vip, Sip, Sport);
	}

	mysql_free_result(res);
}
    
#define	FORWARDFILE	"/etc/ipforward"

void GenForwardFile(void)
{
	MYSQL_RES	*res;
	MYSQL_ROW	row;
	FILE		*fp;
	char		cmd[MAXBUF + 1];

	if (NULL == (fp = fopen(FORWARDFILE, "w"))) {
		return ;
	}

	db_query("select Vip,NetMask from useip where Vflag=1");
	res = mysql_store_result(conn);

	while (NULL != (row = mysql_fetch_row(res))) {
		ifcfg_add(cmd, row[0], row[1]);
		fputs(cmd, fp);
	}

	db_query("select * from server where not (Sflag=3)");
	res = mysql_store_result(conn);

	while (NULL != (row = mysql_fetch_row(res))) {
		iptables_addpre(cmd, row[1], row[3], row[2]);
		fputs(cmd, fp);
	}

	fclose(fp);
}

