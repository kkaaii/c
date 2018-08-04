#include <stdio.h>

#include "gen_forward.h"
#include "config.h"
#include "database.h"
#include "iptables.h"
#include "common.h"

void case6(FILE *client_sock, const char *Sip)
{
	MYSQL_RES	*res;
	MYSQL_ROW	row;
	int		i;

	for (i = 0; '\0' != V_port[i][0]; ++i) {
		const char *Sport = V_port[i];
		db_query("select Vip from server where Sip='%s' and Sport='%s'", Sip, Sport);
		res = mysql_store_result(conn);

		if (mysql_num_rows(res) > 0) {
			row = mysql_fetch_row(res);
			db_query("update server set Sflag=3 where Sip='%s' and Sport='%s' and Vip='%s'", Sip, Sport, row[0]);
			iptables_del(row[0], Sip, Sport, host.fip);
		}

		mysql_free_result(res);
	}

	GenForwardFile();
	fprintf(client_sock, "OK");
}

