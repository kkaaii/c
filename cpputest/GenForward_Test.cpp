#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

#include "MockMysql.h"
#include "MockSystem.h"

extern "C" {
#include "ifcfg.h"
#include "gen_forward.h"
}

#define	VIP	"1.2.3.4"
#define	SIP	"4.3.2.1"
#define	FIP	"5.6.7.8"
#define	P0	"100"
#define	P1	"200"

TEST_GROUP(GenForward)
{
	void setup() {
		UT_PTR_SET(System, MockSystem);
	}

	void teardown() {
		mock("mysql").clear();
		mock().clear();
	}

	MYSQL_RES *query(int rows) {
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
			.andReturnValue(rows);

		return res;
	}
};

TEST(GenForward, iptables_addpre)
{
	char	cmd[256];

	iptables_addpre(cmd, VIP, SIP, P0);
	STRCMP_EQUAL("/sbin/iptables -t nat -A PREROUTING -p tcp -d " VIP " --dport " P0 " -j DNAT --to " SIP, cmd);
}

TEST(GenForward, iptables_addpost)
{
	char	cmd[256];

	iptables_addpost(cmd, FIP, SIP, P0);
	STRCMP_EQUAL("/sbin/iptables -t nat -A POSTROUTING -p tcp -d " SIP " --dport " P0 " -j SNAT --to " FIP, cmd);
}

TEST(GenForward, iptables_delpre)
{
	char	cmd[256];

	iptables_delpre(cmd, VIP, SIP, P0);
	STRCMP_EQUAL("/sbin/iptables -t nat -D PREROUTING -p tcp -d " VIP " --dport " P0 " -j DNAT --to " SIP, cmd);
}

TEST(GenForward, iptables_delpost)
{
	char	cmd[256];

	iptables_delpost(cmd, FIP, SIP, P0);
	STRCMP_EQUAL("/sbin/iptables -t nat -D POSTROUTING -p tcp -d " SIP " --dport " P0 " -j SNAT --to " FIP, cmd);
}

TEST(GenForward, ifcfg_add)
{
	ifcfg_add("ip", "mask");
	STRCMP_EQUAL(IFCFG " add ip/mask", MockSystem_GetCommand());
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
	const char	*req = "V=1I=" SIP;

	ParseRequest(req, &Vtype, Sip, Vports);
	LONGS_EQUAL('1', Vtype);
	STRCMP_EQUAL(SIP, Sip);
	STRCMP_EQUAL("", Vports[0]);
}

TEST(GenForward, ParseRequest__Vtype_Sip_Vport)
{
	char	Vtype;
	char	Sip[16] = {};
	char	Vports[2][MAXBUF + 1] = {};
	const char	*req = "V=1I=" SIP "P=" P0;

	ParseRequest(req, &Vtype, Sip, Vports);
	LONGS_EQUAL('1', Vtype);
	STRCMP_EQUAL(SIP, Sip);
	STRCMP_EQUAL(P0, Vports[0]);
}

TEST(GenForward, ParseRequest__Vtype_Sip_Vports)
{
	char	Vtype;
	char	Sip[16] = {};
	char	Vports[2][MAXBUF + 1] = {};
	const char	*req = "V=1I=" SIP "P=" P0 "+" P1;

	ParseRequest(req, &Vtype, Sip, Vports);
	LONGS_EQUAL('1', Vtype);
	STRCMP_EQUAL(SIP, Sip);
	STRCMP_EQUAL(P0, Vports[0]);
	STRCMP_EQUAL(P1, Vports[1]);
}

TEST(GenForward, Case0__no_record)
{
	this->query(0);
	LONGS_EQUAL(0, Case0(SIP));
}

TEST(GenForward, Case0__has_record)
{
	char		tmp[][1024] = {VIP, SIP, P0};
	MYSQL_RES	*res = this->query(1);
	MYSQL_ROW	row = tmp;

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
		.withParameter("command", "/sbin/iptables -t nat -D PREROUTING -p tcp -d " VIP " --dport " P0 " -j DNAT --to " SIP)
		.andReturnValue(0);

	mock("system")
		.expectOneCall("system")
		.withParameter("command", "/sbin/iptables -t nat -D POSTROUTING -p tcp -d " SIP " --dport " P0 " -j SNAT --to 10.105.1.200")
		.andReturnValue(0);

	Case0(SIP);
}

TEST(GenForward, Case1__no_port)
{
	const char	Vports[][MAXBUF + 1] = {""};

	LONGS_EQUAL(0, Case1(SIP, Vports));
}
#if 0
IGNORE_TEST(GenForward, Case1_has_record)
{
	const char	*Sip = "1.2.3.4";
	const char	Vports[][MAXBUF + 1] = {"100"};

	MYSQL_RES	*res = this->query(1);

	mock("mysql")
		.expectOneCall("mysql_fetch_row")
		.withParameter("res", res)
		.andReturnValue(row);

	Case1(Sip, Vports);
}

IGNORE_TEST(GenForward, Case1_no_record)
{
	const char	*Sip = "1.2.3.4";
	const char	Vports[][MAXBUF + 1] = {"100"};

	this->query(1);
	mock("mysql")
		.expectOneCall("mysql_query")
		.withParameter("conn", conn)
		.withParameter("sql", "select Vip,NetMask from useip where Vflag=0 and Fzone=0 limit 0,1");

	LONGS_EQUAL(0, Case1(Sip, Vports));
}
#endif
TEST(GenForward, Case3__no_record)
{
	const char	Vports[][MAXBUF + 1] = {P0, P1};

	this->query(0);
	LONGS_EQUAL(0, Case3(SIP, Vports));
}

TEST(GenForward, Case4__no_record)
{
	char	req[MAXBUF + 1];

	this->query(0);
	LONGS_EQUAL(0, Case4(req, SIP));
}

TEST(GenForward, Case4_has_one_record)
{
	MYSQL_RES	*res = this->query(1);
	char		row[][MAXBUF] = {VIP, P0};
	char		req[MAXBUF + 1];

	mock("mysql")
		.expectOneCall("mysql_fetch_row")
		.withParameter("res", res)
		.andReturnValue(row);

	mock("mysql")
		.expectOneCall("mysql_fetch_row")
		.withParameter("res", res)
		.andReturnValue((void *)0);

	LONGS_EQUAL(1, Case4(req, SIP));
	STRCMP_EQUAL("OK! V=" VIP "I=" SIP "P=" P0, req);
}

TEST(GenForward, Case4_has_more_records)
{
	MYSQL_RES	*res = this->query(1);
	char		row0[][MAXBUF] = {VIP, P0};
	char		row1[][MAXBUF] = {VIP, P1};
	char		req[MAXBUF + 1];

	mock("mysql")
		.expectOneCall("mysql_fetch_row")
		.withParameter("res", res)
		.andReturnValue(row0);

	mock("mysql")
		.expectOneCall("mysql_fetch_row")
		.withParameter("res", res)
		.andReturnValue(row1);

	mock("mysql")
		.expectOneCall("mysql_fetch_row")
		.withParameter("res", res)
		.andReturnValue((void *)0);

	LONGS_EQUAL(1, Case4(req, SIP));
	STRCMP_EQUAL("OK! V=" VIP "I=" SIP "P=" P0 "+" P1, req);
}

TEST(GenForward, Case5__no_vip)
{
	const char	Vports[][MAXBUF + 1] = {""};
	LONGS_EQUAL(0, Case5(SIP, Vports));
}

TEST(GenForward, Case5__no_record)
{
	const char	Vports[][MAXBUF + 1] = {VIP};

	this->query(0);
	LONGS_EQUAL(0, Case5(SIP, Vports));
}

