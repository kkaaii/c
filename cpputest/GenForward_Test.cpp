#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

#include "MockMysql.h"
#include "MockSystem.h"

extern "C" {
#include <stdio.h>
#include "log.h"
#include "ifcfg.h"
#include "iptables.h"
#include "gen_forward.h"
#include "common.h"
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
		log_init("./log");
	}

	void teardown() {
		log_exit();
		mock("mysql").clear();
		mock().clear();
	}

	void expectMysqlQuery(const char *sql) {
		mock("mysql")
			.expectOneCall("mysql_query")
			.withParameter("sql", sql)
			.ignoreOtherParameters()
			.andReturnValue(0);
	}

	void expectMysqlStoreResult(void) {
		MYSQL_RES	*res = (MYSQL_RES *)1;

		mock("mysql")
			.expectOneCall("mysql_store_result")
			.ignoreOtherParameters()
			.andReturnValue(res);
	}

	void expectMysqlNumRows(int rows) {
		mock("mysql")
			.expectOneCall("mysql_num_rows")
			.ignoreOtherParameters()
			.andReturnValue(rows);
	}

	void expectMysqlFetchRow(const char **row) {
		mock("mysql")
			.expectOneCall("mysql_fetch_row")
			.ignoreOtherParameters()
			.andReturnValue(row);
	}

	void expectMysqlFreeResult(void) {
		mock("mysql")
			.expectOneCall("mysql_free_result")
			.ignoreOtherParameters();
	}

	void expectIfcfgDel() {
		mock("stdlib")
			.expectOneCall("MockSystem")
			.withParameter("command", IFCFG " del " VIP "/NetMask");
	}

	void expectIptablesDel0() {
		mock("stdlib")
			.expectOneCall("MockSystem")
			.withParameter("command", "/sbin/iptables -t nat -D PREROUTING -p tcp -d " VIP " --dport " P0 " -j DNAT --to " SIP)
			.andReturnValue(0);

		mock("stdlib")
			.expectOneCall("MockSystem")
			.withParameter("command", "/sbin/iptables -t nat -D POSTROUTING -p tcp -d " SIP " --dport " P0 " -j SNAT --to 10.105.1.200")
			.andReturnValue(0);
	}

	void expectIptablesDel1() {
		mock("stdlib")
			.expectOneCall("MockSystem")
			.withParameter("command", "/sbin/iptables -t nat -D PREROUTING -p tcp -d " VIP " --dport " P1 " -j DNAT --to " SIP)
			.andReturnValue(0);

		mock("stdlib")
			.expectOneCall("MockSystem")
			.withParameter("command", "/sbin/iptables -t nat -D POSTROUTING -p tcp -d " SIP " --dport " P1 " -j SNAT --to 10.105.1.200")
			.andReturnValue(0);
	}

	void expectDelVip(void) {
		static const char *row[] = {"NetMask"};

		expectMysqlQuery("select NetMask from useip where Vip='" VIP "'");
		expectMysqlStoreResult();
		expectMysqlNumRows(1);
		expectMysqlFetchRow(row);
		expectIfcfgDel();
		expectMysqlFreeResult();
	}

	void expectAddVip(void) {
		expectMysqlQuery("select * from server where Sip='" SIP "' and Sport='" P0 "'");
		expectMysqlStoreResult();
		expectMysqlNumRows(0);
		mock("stdlib").expectNCalls(2, "MockSystem").ignoreOtherParameters();
		expectMysqlQuery("insert into server (Vip,Sip,Sport) value('" VIP "','" SIP "','" P0 "')");
		expectMysqlFreeResult();
	}

	void expectGenForward(void) {
		expectMysqlQuery("select Vip,NetMask from useip where Vflag=1");
		expectMysqlStoreResult();
		expectMysqlFetchRow(NULL);

		expectMysqlQuery("select * from server where not (Sflag=3)");
		expectMysqlStoreResult();
		expectMysqlFetchRow(NULL);
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
	char	cmd[256];

	ifcfg_add(cmd, "ip", "mask");
	STRCMP_EQUAL(IFCFG " add ip/mask", cmd);
}

TEST(GenForward, ifcfg_del)
{
	char	cmd[256];

	ifcfg_del(cmd, "ip", "mask");
	STRCMP_EQUAL(IFCFG " del ip/mask", cmd);
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

#if 0
TEST(GenForward, Case0__no_record)
{
	const char	*sql = "select Vip,Sip,Sport from server where Sip='" SIP "'";
	this->query(sql, 0);
	LONGS_EQUAL(0, Case0(SIP));
}

TEST(GenForward, Case0__has_record)
{
	const char	*sql = "select Vip,Sip,Sport from server where Sip='" SIP "'";
	const char	*row[] = {VIP, SIP, P0};
	this->query(sql, row);
/*
	mock("mysql")
		.expectOneCall("mysql_fetch_row")
		.withParameter("res", res)
		.andReturnValue(row);
*/

//		db_query("select NetMask from useip where Vip='%s'", Vip);
//		db_foreach(callback_ifcfg_del, Vip);
	mock("mysql")
		.expectOneCall("mysql_query")
		.withParameter("sql", "delete from server where Sip='" SIP "'")
		.ignoreOtherParameters()
		.andReturnValue(0);

	mock("mysql")
		.expectOneCall("mysql_query")
		.withParameter("sql", "update useip set Vflag=0 where Vip='" VIP "'")
		.ignoreOtherParameters()
		.andReturnValue(0);

	sql = "select NetMask from useip where Vip='" VIP "'";
	this->query(sql, 0);
/*
	mock("stdlib")
		.expectOneCall("MockSystem")
		.withParameter("command", "/sbin/iptables -t nat -D PREROUTING -p tcp -d " VIP " --dport " P0 " -j DNAT --to " SIP)
		.andReturnValue(0);

	mock("stdlib")
		.expectOneCall("MockSystem")
		.withParameter("command", "/sbin/iptables -t nat -D POSTROUTING -p tcp -d " SIP " --dport " P0 " -j SNAT --to 10.105.1.200")
		.andReturnValue(0);
*/
	Case0(SIP);
}
#endif
#if 0
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
	const char	*row[] = {VIP, P0};
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
	const char	*row0[] = {VIP, P0};
	const char	*row1[] = {VIP, P1};
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
#endif

TEST(GenForward, common_del_vip)
{
	expectDelVip();
	DelVip(VIP);
}

TEST(GenForward, common_add_vip)
{
	expectAddVip();
	AddVip(VIP, SIP, P0, FIP);
}

TEST(GenForward, case0_zero_row)
{
	expectMysqlQuery("select Vip,Sip,Sport from server where Sip='" SIP "'");
	expectMysqlStoreResult();
	expectMysqlNumRows(0);
	expectMysqlFreeResult();

	case0(stderr, SIP);
}

TEST(GenForward, case0_one_row)
{
	const char *row[] = {VIP, SIP, P0};

	expectMysqlQuery("select Vip,Sip,Sport from server where Sip='" SIP "'");
	expectMysqlStoreResult();
	expectMysqlNumRows(1);
	expectMysqlFetchRow(row);
	expectMysqlQuery("delete from server where Sip='" SIP "'");
        expectMysqlQuery("update useip set Vflag=0 where Vip='" VIP "'");
	expectMysqlFreeResult();

	expectDelVip();
	expectIptablesDel0();
	expectMysqlFetchRow(NULL);
	expectGenForward();

	case0(stderr, SIP);
}

TEST(GenForward, case0_more_rows)
{
	const char *row0[] = {VIP, SIP, P0};
	const char *row1[] = {VIP, SIP, P1};

	expectMysqlQuery("select Vip,Sip,Sport from server where Sip='" SIP "'");
	expectMysqlStoreResult();
	expectMysqlNumRows(2);
	expectMysqlFetchRow(row0);
	expectMysqlQuery("delete from server where Sip='" SIP "'");
        expectMysqlQuery("update useip set Vflag=0 where Vip='" VIP "'");
	expectMysqlFreeResult();

	expectDelVip();
	expectIptablesDel0();
	expectMysqlFetchRow(row1);
	expectIptablesDel1();
	expectMysqlFetchRow(NULL);
	expectGenForward();

	case0(stderr, SIP);
}

