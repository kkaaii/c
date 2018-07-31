typedef	int	MYSQL;
typedef	int	MYSQL_RES;
typedef	const char	**MYSQL_ROW;

MYSQL *mysql_init(MYSQL *mysql);
MYSQL *mysql_real_connect(
	MYSQL		*mysql,
	const char	*host,
	const char	*user,
	const char	*passwd,
	const char	*db,
	unsigned int	port,
	const char	*unix_socket,
	unsigned long	client_flag);

void mysql_close(MYSQL *mysql);
const char *mysql_error(MYSQL *mysql);

int mysql_query(MYSQL *conn, const char *sql);
MYSQL_RES *mysql_store_result(MYSQL *conn);
int mysql_num_rows(MYSQL_RES *res);
MYSQL_ROW mysql_fetch_row(MYSQL_RES *res);

