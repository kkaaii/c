#ifndef _MOCK_MYSQL_H
#define	_MOCK_MYSQL_H

extern "C" {
typedef	int	MYSQL;
typedef	int	MYSQL_RES;
typedef char	(*MYSQL_ROW)[1024];

int		mysql_query(MYSQL *conn, const char *sql);
MYSQL_RES *	mysql_store_result(MYSQL *conn);
int		mysql_num_rows(MYSQL_RES *res);
MYSQL_ROW	mysql_fetch_row(MYSQL_RES *res);

}

#endif	/* _MOCK_MYSQL_H */

