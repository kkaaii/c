#include <stdio.h>

#include "gen_forward.h"
#include "config.h"
#include "database.h"
#include "iptables.h"
#include "common.h"

void case5(FILE *client_sock, const char *Sip)
{
	MYSQL_RES	*res;
	MYSQL_ROW	row;
	const char	*Vip = V_port[0];
	const char	*SipOld = V_port[1];

	if ('\0' != *Vip) {
		db_query("select Sport from server where Vip='%s' and Sip='%s'", Vip, SipOld);
		res = mysql_store_result(conn);

		if (mysql_num_rows(res) > 0) {
			while ((row = mysql_fetch_row(res)) != NULL) {
				iptables_del(Vip, SipOld, row[0], host.fip);
				iptables_add(Vip, Sip, row[0], host.fip);
			}

			db_query("update server set Sip='%s' where Vip='%s' and Sip='%s'", Sip, Vip, SipOld);
			GenForwardFile();
		}

		mysql_free_result(res);
	}

	fprintf(client_sock, "OK");
}

