#include <stdio.h>
#include <string.h>

#include "gen_forward.h"
#include "database.h"

void case4(FILE *client_sock, const char *Sip)
{
	MYSQL_RES	*res;
	MYSQL_ROW	row;
	char		response[MAXBUF + 1];

	db_query("select Vip,Sport from server where Sip='%s'", Sip);
	res = mysql_store_result(conn);

	if (mysql_num_rows(res) > 0) {
		row = mysql_fetch_row(res);
		sprintf(response, "OK! V=%sI=%sP=%s", row[0], Sip, row[1]);

		while (NULL != (row = mysql_fetch_row(res))) {
			strcat(response, "+");
			strcat(response, row[1]);
		}

		fprintf(client_sock, "%s", response);
	}

	mysql_free_result(res);
}

