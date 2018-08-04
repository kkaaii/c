#include <stdio.h>
#include <stdlib.h>

#include "gen_forward.h"
#include "config.h"
#include "database.h"
#include "common.h"
#include "iptables.h"

void case0(FILE *client_sock, const char *Sip)
{
	MYSQL_RES   *res;
	MYSQL_ROW   row;

	db_query("select Vip,Sip,Sport from server where Sip='%s'", Sip);
	res = mysql_store_result(conn);

	if (mysql_num_rows(res) > 0) {
		row = mysql_fetch_row(res);
		db_query("delete from server where Sip='%s'", Sip);
		db_query("update useip set Vflag=0 where Vip='%s'", row[0]);

		DelVip(row[0]);

		iptables_del(row[0], row[1], row[2], host.fip);

		while (NULL != (row = mysql_fetch_row(res))) {
			iptables_del(row[0], row[1], row[2], host.fip);
		}

		GenForwardFile();
	}

	mysql_free_result(res);
	fprintf(client_sock, "OK");
}

