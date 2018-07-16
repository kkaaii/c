#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

#include "MockMysql.h"
#include "MockSystem.h"

extern "C" {
#include "gen_forward.h"
}

TEST_GROUP(GenForward)
{
	void setup() {
		UT_PTR_SET(System, MockSystem);
	}

	void teardown() {
		mock("mysql").clear();
		mock().clear();
	}
};

TEST(GenForward, iptables_addpre)
{
	char	cmd[256];
	const char	*Vip = "1.2.3.4";
	const char	*Sip = "4.3.2.1";

	iptables_addpre(cmd, Vip, Sip, "100");
	STRCMP_EQUAL("/sbin/iptables -t nat -A PREROUTING -p tcp -d 1.2.3.4 --dport 100 -j DNAT --to 4.3.2.1", cmd);
}

TEST(GenForward, iptables_addpost)
{
	char	cmd[256];
	const char	*Fip = "5.6.7.8";
	const char	*Sip = "4.3.2.1";

	iptables_addpost(cmd, Fip, Sip, "100");
	STRCMP_EQUAL("/sbin/iptables -t nat -A POSTROUTING -p tcp -d 4.3.2.1 --dport 100 -j SNAT --to 5.6.7.8", cmd);
}

TEST(GenForward, iptables_delpre)
{
	char	cmd[256];
	const char	*Vip = "1.2.3.4";
	const char	*Sip = "4.3.2.1";

	iptables_delpre(cmd, Vip, Sip, "100");
	STRCMP_EQUAL("/sbin/iptables -t nat -D PREROUTING -p tcp -d 1.2.3.4 --dport 100 -j DNAT --to 4.3.2.1", cmd);
}

TEST(GenForward, iptables_delpost)
{
	char	cmd[256];
	const char	*Fip = "5.6.7.8";
	const char	*Sip = "4.3.2.1";

	iptables_delpost(cmd, Fip, Sip, "100");
	STRCMP_EQUAL("/sbin/iptables -t nat -D POSTROUTING -p tcp -d 4.3.2.1 --dport 100 -j SNAT --to 5.6.7.8", cmd);
}

#if 0
TEST(GenForward, GetVtype__normal)
{
	char	Vtype;
	const char	*req = "V=1";

	const char	*p = GetVtype(&Vtype, req);
	LONGS_EQUAL('1', Vtype);
	POINTERS_EQUAL(req + 2, p);
}

TEST(GenForward, GetVtype__no_value)
{
	char	Vtype = '\0';
	const char	*req = "V=";

	const char	*p = GetVtype(&Vtype, req);
	LONGS_EQUAL('\0', Vtype);
	POINTERS_EQUAL(req + 2, p);
}

TEST(GenForward, GetVtype__no_equal_sign)
{
	char	Vtype = '\0';
	const char	*req = "V1";

	const char	*p = GetVtype(&Vtype, req);
	LONGS_EQUAL('\0', Vtype);
	POINTERS_EQUAL(NULL, p);
}

TEST(GenForward, GetSip__normal_null)
{
	char	Sip[16];
	const char	*req = "I=1.2.3.4";

	const char	*p = GetSip(Sip, req);
	STRCMP_EQUAL("1.2.3.4", Sip);
	POINTERS_EQUAL(req + 9, p);
}

TEST(GenForward, GetSip__normal_P)
{
	char	Sip[16];
	const char	*req = "I=1.2.3.4P";

	const char	*p = GetSip(Sip, req);
	STRCMP_EQUAL("1.2.3.4", Sip);
	POINTERS_EQUAL(req + 9, p);
}

TEST(GenForward, GetVports__one)
{
	char	Vports[2][MAXBUF + 1];
	const char	*req = "P=10";

	GetVports(Vports, req);
	STRCMP_EQUAL("10", Vports[0]);
}

TEST(GenForward, GetVports__more)
{
	char	Vports[2][MAXBUF + 1];
	const char	*req = "P=10+20";

	GetVports(Vports, req);
	STRCMP_EQUAL("10", Vports[0]);
	STRCMP_EQUAL("20", Vports[1]);
}
#endif

TEST(GenForward, ParseRequest__Vtype)
{
	char	Vtype;
	char	Sip[16] = {};
	char	Vports[2][MAXBUF + 1] = {};
	const char	*req = "V=1";

	ParseRequest(req, &Vtype, Sip, Vports);
	LONGS_EQUAL('1', Vtype);
	STRCMP_EQUAL("", Sip);
	STRCMP_EQUAL("", Vports[0]);
}

TEST(GenForward, ParseRequest__Vtype_Sip)
{
	char	Vtype;
	char	Sip[16] = {};
	char	Vports[2][MAXBUF + 1] = {};
	const char	*req = "V=1I=1.2.3.4";

	ParseRequest(req, &Vtype, Sip, Vports);
	LONGS_EQUAL('1', Vtype);
	STRCMP_EQUAL("1.2.3.4", Sip);
	STRCMP_EQUAL("", Vports[0]);
}

TEST(GenForward, ParseRequest__Vtype_Sip_Vport)
{
	char	Vtype;
	char	Sip[16] = {};
	char	Vports[2][MAXBUF + 1] = {};
	const char	*req = "V=1I=1.2.3.4P=10";

	ParseRequest(req, &Vtype, Sip, Vports);
	LONGS_EQUAL('1', Vtype);
	STRCMP_EQUAL("1.2.3.4", Sip);
	STRCMP_EQUAL("10", Vports[0]);
}

TEST(GenForward, ParseRequest__Vtype_Sip_Vports)
{
	char	Vtype;
	char	Sip[16] = {};
	char	Vports[2][MAXBUF + 1] = {};
	const char	*req = "V=1I=1.2.3.4P=10+20";

	ParseRequest(req, &Vtype, Sip, Vports);
	LONGS_EQUAL('1', Vtype);
	STRCMP_EQUAL("1.2.3.4", Sip);
	STRCMP_EQUAL("10", Vports[0]);
	STRCMP_EQUAL("20", Vports[1]);
}

TEST(GenForward, Case0__no_record)
{
	MYSQL_RES	*res = (MYSQL_RES *)1;

	mock("mysql")
		.expectOneCall("mysql_query")
		.ignoreOtherParameters()
		.andReturnValue(0);

	mock("mysql")
		.expectOneCall("mysql_store_result")
		.withParameter("conn", conn)
		.andReturnValue(res);

	mock("mysql")
		.expectOneCall("mysql_num_rows")
		.withParameter("res", res)
		.andReturnValue(0);
}

TEST(GenForward, Case0__has_record)
{
	const char	*Sip = "1.2.3.4";
	char		tmp[][1024] = {"4.3.2.1", "1.2.3.4", "10"};
	MYSQL_RES	*res = (MYSQL_RES *)1;
	MYSQL_ROW	row = tmp;

	mock("mysql")
		.expectOneCall("mysql_query")
		.withParameter("conn", conn)
		.withParameter("sql", sql)
		.andReturnValue(0);

	mock("mysql")
		.expectOneCall("mysql_store_result")
		.withParameter("conn", conn)
		.andReturnValue(res);

	mock("mysql")
		.expectOneCall("mysql_num_rows")
		.withParameter("res", res)
		.andReturnValue(1);

	mock("mysql")
		.expectOneCall("mysql_fetch_row")
		.withParameter("res", res)
		.andReturnValue(row);

	mock("mysql")
		.expectOneCall("mysql_query")
		.withParameter("conn", conn)
		.withParameter("sql", sql);

	mock("system")
		.expectOneCall("system")
		.withParameter("command", "/sbin/iptables -t nat -D PREROUTING -p tcp -d 4.3.2.1 --dport 10 -j DNAT --to 1.2.3.4")
		.andReturnValue(0);

	mock("system")
		.expectOneCall("system")
		.withParameter("command", "/sbin/iptables -t nat -D POSTROUTING -p tcp -d 1.2.3.4 --dport 10 -j SNAT --to 10.105.1.200")
		.andReturnValue(0);

	Case0(Sip);
}

