#include "CppUTestExt/MockSupport.h"

#include "MockMysql.h"

#include <stdint.h>
#define	CAST_PTR(T)	(T)(intptr_t)

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
