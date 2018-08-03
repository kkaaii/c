#include <stdio.h>

#include "gen_forward.h"
#include "database.h"
#include "ifcfg.h"
#include "iptables.h"
#include "common.h"

void case2(FILE *client_sock, const char *Sip)
{
	MYSQL_RES	*res;
	MYSQL_ROW	row;
	int		updated = 0;
	int		i;

	for (i = 0; '\0' != V_port[i][0]; ++i) {
		const char  *Sport = V_port[i];

		db_query("select Vip from server where Sip='%s' and Sport='%s'", Sip, Sport);
		res = mysql_store_result(conn);

		if (mysql_num_rows(res) > 0) {
			updated = 1;
			row = mysql_fetch_row(res);
			db_query("delete from server where Sip='%s' and Sport='%s' and Vip='%s'", Sip, Sport, row[0]);

			iptables_del(row[0], Sip, Sport, host.fip);
		}

		mysql_free_result(res);
	}

	if (updated) {
		db_query("select Vip from server where Sip='%s'", Sip);
		res = mysql_store_result(conn);

		if (mysql_num_rows(res) <= 0) {
			db_query("update useip set Vflag=0 where Vip='%s'", row[0]);
			DelVip(row[0]);
		}

		mysql_free_result(res);
		GenForwardFile();
	}

	fprintf(client_sock, "OK");
}

