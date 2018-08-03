#include <stdio.h>
#include "mysql.h"

#include "database.h"
#include "log.h"

#define	MAXBUF	1024

MYSQL	*conn = NULL;

void db_init(struct database *db)
{
	conn = mysql_init(NULL);

	if (!mysql_real_connect(conn,
		db->server, db->user, db->password, db->database, 0, NULL, 0)) {
		ERR("mysql_real_connect: %s", mysql_error(conn));
		exit(0);
	}
}

void db_exit(void)
{
	if (NULL != conn) {
		mysql_close(conn);
		conn = NULL;
	}
}

void db_query(const char *format, ...)
{
	static char	sql[MAXBUF + 1];
	va_list		args;

	va_start(args, format);
	vsprintf(sql, format, args);
	va_end(args);

	LOG("SQL: %s\n", sql);
	if (mysql_query(conn, sql)) {
		ERR("mysql_query: %s", mysql_error(conn));
		exit(0);
	}
}

int db_foreach(void (*callback)(const char **row, va_list args, int n), ...)
{
	MYSQL_ROW	row;
	MYSQL_RES	*res = mysql_store_result(conn);
	va_list		args;
	int		n = 0;

	va_start(args, callback);
	while ((row = mysql_fetch_row(res)) != NULL) {
		(*callback)(row, args, n);
		++n;
	}
	va_end(args);

	return n;
}

