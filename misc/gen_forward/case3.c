#include <stdio.h>

#include "gen_forward.h"
#include "config.h"
#include "database.h"
#include "iptables.h"
#include "common.h"

void case3(FILE *client_sock, const char *Sip)
{
	MYSQL_RES	*res;
	MYSQL_ROW	row;
	int		i;

	db_query("select Vip from server where Sip='%s'", Sip);
	res = mysql_store_result(conn);

	if (mysql_num_rows(res) > 0) {
		row = mysql_fetch_row(res);

		for (i = 0; '\0' != V_port[i][0]; ++i) {
			AddVip(row[0], Sip, V_port[i], host.fip);
		}

		GenForwardFile();
	}

	mysql_free_result(res);
	fprintf(client_sock, "OK");
}

