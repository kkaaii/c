#include "CppUTestExt/MockSupport.h"

#include "MockMysql.h"

#include <stdint.h>
#define	CAST_PTR(T)	(T)(intptr_t)

MYSQL *mysql_init(MYSQL *mysql)
{
	return (MYSQL *)mock("mysql")
		.actualCall("mysql_init")
		.withParameter("mysql", mysql)
		.returnValue().getPointerValue();
}

void mysql_close(MYSQL *mysql)
{
	mock("mysql")
		.actualCall("mysql_close")
		.withParameter("mysql", mysql);
}

const char *mysql_error(MYSQL *mysql)
{
	return (const char *)mock("mysql")
		.actualCall("mysql_error")
		.withParameter("mysql", mysql)
		.returnValue().getPointerValue();
}

MYSQL *mysql_real_connect(
	MYSQL		*mysql,
	const char	*host,
	const char	*user,
	const char	*passwd,
	const char	*db,
	unsigned int	port,
	const char	*unix_socket,
	unsigned long	client_flag)
{
	return (MYSQL *)mock("mysql")
		.actualCall("mysql_real_connect")
		.withParameter("mysql", mysql)
		.withParameter("host", host)
		.withParameter("user", user)
		.withParameter("passwd", passwd)
		.withParameter("db", db)
		.withParameter("port", port)
		.withParameter("unix_socket", unix_socket)
		.withParameter("client_flag", client_flag)
		.returnValue().getPointerValue();
}

int mysql_query(MYSQL *conn, const char *sql)
{
	return mock("mysql")
		.actualCall("mysql_query")
		.withParameter("conn", conn)
		.withParameter("sql", sql)
		.returnValue().getIntValue();
}

MYSQL_RES *mysql_store_result(MYSQL *conn)
{
	return (MYSQL_RES *)mock("mysql")
		.actualCall("mysql_store_result")
		.withParameter("conn", conn)
		.returnValue().getPointerValue();
}

int mysql_num_rows(MYSQL_RES *res)
{
	return mock("mysql")
		.actualCall("mysql_num_rows")
		.withParameter("res", res)
		.returnValue().getIntValue();
}

MYSQL_ROW mysql_fetch_row(MYSQL_RES *res)
{
	return (MYSQL_ROW)mock("mysql")
		.actualCall("mysql_fetch_row")
		.withParameter("res", res)
		.returnValue().getPointerValue();
}
