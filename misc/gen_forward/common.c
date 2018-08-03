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
	row = mysql_fetch_row(res);
	ifcfg_del(cmd, Vip, row[0]);
	system(cmd);
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

