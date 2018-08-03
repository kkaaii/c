#include <stdio.h>

#include "gen_forward.h"
#include "database.h"
#include "ifcfg.h"
#include "iptables.h"
#include "common.h"

void case8(FILE *client_sock, const char *Sip)
{
	MYSQL_RES	*res;
	MYSQL_ROW	row;
	char		cmd[MAXBUF + 1];
	int		i;

	if ('\0' == V_port[0]) {
		return;
	}

	db_query("select Vip from server where Sip='%s'", Sip);
	res = mysql_store_result(conn);

	if (mysql_num_rows(res) > 0) {
		row = mysql_fetch_row(res);
	} else {
		mysql_free_result(res);

		db_query("select Vip,NetMask from useip where Vflag=0 and Fzone=1 limit 0,1");
		res = mysql_store_result(conn);

		if (mysql_num_rows(res) <= 0) {
			mysql_free_result(res);
			return;
		}

		row = mysql_fetch_row(res);
		ifcfg_add(cmd, row[0], row[1]);
		system(cmd);
		db_query("update useip set Vflag=1 where vip='%s'", row[0]);
	}

	for (i = 0; '\0' != V_port[i][0]; ++i) {
		AddVip(row[0], Sip, V_port[i], host.fip);
	}

	GenForwardFile();
	fprintf(client_sock, "%s", row[0]);
	mysql_free_result(res);
}

