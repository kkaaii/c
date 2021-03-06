#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

#include "mysql.h"
#include "MockSystem.h"

extern "C" {
#include <stdio.h>
#include <string.h>
#include "log.h"
#include "ifcfg.h"
#include "iptables.h"
#include "gen_forward.h"
#include "common.h"
}

#define	VIP	"vip"
#define	SIP	"sip"
#define	FIP	"10.105.1.200"
#define	P0	"p0"
#define	P1	"p1"

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

	void expectIfcfgAdd() {
		mock("stdlib")
			.expectOneCall("MockSystem")
			.withParameter("command", IFCFG " add " VIP "/NetMask");
	}

	void expectIfcfgDel() {
		mock("stdlib")
			.expectOneCall("MockSystem")
			.withParameter("command", IFCFG " del " VIP "/NetMask");
	}

	void expectIptablesAdd0() {
		mock("stdlib")
			.expectOneCall("MockSystem")
			.withParameter("command", "/sbin/iptables -t nat -A PREROUTING -p tcp -d " VIP " --dport " P0 " -j DNAT --to " SIP)
			.andReturnValue(0);

		mock("stdlib")
			.expectOneCall("MockSystem")
			.withParameter("command", "/sbin/iptables -t nat -A POSTROUTING -p tcp -d " SIP " --dport " P0 " -j SNAT --to 10.105.1.200")
			.andReturnValue(0);
	}

	void expectIptablesAdd1() {
		mock("stdlib")
			.expectOneCall("MockSystem")
			.withParameter("command", "/sbin/iptables -t nat -A PREROUTING -p tcp -d " VIP " --dport " P1 " -j DNAT --to " SIP)
			.andReturnValue(0);

		mock("stdlib")
			.expectOneCall("MockSystem")
			.withParameter("command", "/sbin/iptables -t nat -A POSTROUTING -p tcp -d " SIP " --dport " P1 " -j SNAT --to 10.105.1.200")
			.andReturnValue(0);
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
		expectIptablesAdd0();
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

TEST(GenForward, case1_no_port)
{
	V_port[0][0] = '\0';

	case1(stderr, SIP);
}

TEST(GenForward, case1_no_vip)
{
	strcpy(V_port[0], P0);
	V_port[1][0] = '\0';

	expectMysqlQuery("select Vip from server where Sip='" SIP "'");
	expectMysqlStoreResult();
	expectMysqlNumRows(0);
	expectMysqlFreeResult();

	expectMysqlQuery("select Vip,NetMask from useip where Vflag=0 and Fzone=0 limit 0,1");
	expectMysqlStoreResult();
	expectMysqlNumRows(0);
	expectMysqlFreeResult();

	case1(stderr, SIP);
}

TEST(GenForward, case1_vip_in_server)
{
	const char	*row[] = {VIP};
	strcpy(V_port[0], P0);
	V_port[1][0] = '\0';

	expectMysqlQuery("select Vip from server where Sip='" SIP "'");
	expectMysqlStoreResult();
	expectMysqlNumRows(1);
	expectMysqlFetchRow(row);
	expectMysqlFreeResult();

	expectAddVip();

	case1(stderr, SIP);
}

TEST(GenForward, case1_vip_in_useip)
{
	const char	*row[] = {VIP, "NetMask"};
	strcpy(V_port[0], P0);
	V_port[1][0] = '\0';

	expectMysqlQuery("select Vip from server where Sip='" SIP "'");
	expectMysqlStoreResult();
	expectMysqlNumRows(0);
	expectMysqlFreeResult();

	expectMysqlQuery("select Vip,NetMask from useip where Vflag=0 and Fzone=0 limit 0,1");
	expectMysqlStoreResult();
	expectMysqlNumRows(1);
	expectMysqlFetchRow(row);
	expectIfcfgAdd();
	expectMysqlQuery("update useip set Vflag=1 where Vip='" VIP "'");
	expectMysqlFreeResult();

	expectAddVip();

	case1(stderr, SIP);
}

TEST(GenForward, case2_no_port)
{
	V_port[0][0] = '\0';

	case2(stderr, SIP);
}

TEST(GenForward, case2_no_vip)
{
	strcpy(V_port[0], P0);
	V_port[1][0] = '\0';

	expectMysqlQuery("select Vip from server where Sip='" SIP "' and Sport='" P0 "'");
	expectMysqlStoreResult();
	expectMysqlNumRows(0);
	expectMysqlFreeResult();

	case2(stderr, SIP);
}

TEST(GenForward, case2_vip_in_both)
{
	const char	*row[] = {VIP};
	strcpy(V_port[0], P0);
	V_port[1][0] = '\0';

	expectMysqlQuery("select Vip from server where Sip='" SIP "' and Sport='" P0 "'");
	expectMysqlStoreResult();
	expectMysqlNumRows(1);
	expectMysqlFetchRow(row);
	expectMysqlQuery("delete from server where Sip='" SIP "' and Sport='" P0 "' and Vip='" VIP "'");
	expectIptablesDel0();
	expectMysqlFreeResult();

	expectMysqlQuery("select Vip from server where Sip='" SIP "'");
	expectMysqlStoreResult();
	expectMysqlNumRows(1);
	expectMysqlFreeResult();

	case2(stderr, SIP);
}

TEST(GenForward, case2_vip_in_server_only)
{
	const char	*row[] = {VIP};
	strcpy(V_port[0], P0);
	V_port[1][0] = '\0';

	expectMysqlQuery("select Vip from server where Sip='" SIP "' and Sport='" P0 "'");
	expectMysqlStoreResult();
	expectMysqlNumRows(1);
	expectMysqlFetchRow(row);
	expectMysqlQuery("delete from server where Sip='" SIP "' and Sport='" P0 "' and Vip='" VIP "'");
	expectIptablesDel0();
	expectMysqlFreeResult();

	expectMysqlQuery("select Vip from server where Sip='" SIP "'");
	expectMysqlStoreResult();
	expectMysqlNumRows(0);
	expectMysqlQuery("update useip set Vflag=0 where Vip='" VIP "'");
	expectDelVip();
	expectMysqlFreeResult();

	case2(stderr, SIP);
}

TEST(GenForward, case3_no_vip)
{
	expectMysqlQuery("select Vip from server where Sip='" SIP "'");
	expectMysqlStoreResult();
	expectMysqlNumRows(0);
	expectMysqlFreeResult();

	case3(stderr, SIP);
}

TEST(GenForward, case3_vip)
{
	const char	*row[] = {VIP};
	strcpy(V_port[0], P0);
	V_port[1][0] = '\0';

	expectMysqlQuery("select Vip from server where Sip='" SIP "'");
	expectMysqlStoreResult();
	expectMysqlNumRows(1);
	expectMysqlFetchRow(row);
	expectAddVip();
	expectMysqlFreeResult();

	case3(stderr, SIP);
}

TEST(GenForward, case4_zero_row)
{
	expectMysqlQuery("select Vip,Sport from server where Sip='" SIP "'");
	expectMysqlStoreResult();
	expectMysqlNumRows(0);
	expectMysqlFreeResult();

	case4(stderr, SIP);
}

TEST(GenForward, case4_one_row)
{
	const char	*row[] = {VIP, P0};

	expectMysqlQuery("select Vip,Sport from server where Sip='" SIP "'");
	expectMysqlStoreResult();
	expectMysqlNumRows(1);
	expectMysqlFetchRow(row);
	expectMysqlFetchRow(NULL);
	expectMysqlFreeResult();

	case4(stderr, SIP);
}

TEST(GenForward, case4_multiple_rows)
{
	const char	*row0[] = {VIP, P0};
	const char	*row1[] = {VIP, P1};

	expectMysqlQuery("select Vip,Sport from server where Sip='" SIP "'");
	expectMysqlStoreResult();
	expectMysqlNumRows(1);
	expectMysqlFetchRow(row0);
	expectMysqlFetchRow(row1);
	expectMysqlFetchRow(NULL);
	expectMysqlFreeResult();

	case4(stderr, SIP);
}

TEST(GenForward, case5_no_vip)
{
	V_port[0][0] = '\0';

	case5(stderr, SIP);
}

TEST(GenForward, case5_zero_row)
{
	strcpy(V_port[0], VIP);
	strcpy(V_port[1], "Old");

	expectMysqlQuery("select Sport from server where Vip='" VIP "' and Sip='Old'");
	expectMysqlStoreResult();
	expectMysqlNumRows(0);
	expectMysqlFreeResult();

	case5(stderr, SIP);
}

TEST(GenForward, case5_has_rows)
{
	const char	*row0[] = {P0};
	const char	*row1[] = {P1};

	strcpy(V_port[0], VIP);
	strcpy(V_port[1], "Old");

	expectMysqlQuery("select Sport from server where Vip='" VIP "' and Sip='Old'");
	expectMysqlStoreResult();
	expectMysqlNumRows(2);
	expectMysqlFetchRow(row0);
	mock("stdlib").expectNCalls(2, "MockSystem").ignoreOtherParameters();
	expectIptablesAdd0();
	expectMysqlFetchRow(row1);
	mock("stdlib").expectNCalls(2, "MockSystem").ignoreOtherParameters();
	expectIptablesAdd1();
	expectMysqlFetchRow(NULL);
	expectMysqlQuery("update server set Sip='" SIP "' where Vip='" VIP "' and Sip='Old'");
	expectMysqlFreeResult();

	case5(stderr, SIP);
}

TEST(GenForward, case6_no_port)
{
	V_port[0][0] = '\0';

	case6(stderr, SIP);
}

TEST(GenForward, case6_zero_row)
{
	strcpy(V_port[0], P0);
	V_port[1][0] = '\0';

	expectMysqlQuery("select Vip from server where Sip='" SIP "' and Sport='" P0 "'");
	expectMysqlStoreResult();
	expectMysqlNumRows(0);
	expectMysqlFreeResult();

	case6(stderr, SIP);
}

TEST(GenForward, case6_one_row)
{
	const char	*row[] = {VIP};
	strcpy(V_port[0], P0);
	V_port[1][0] = '\0';

	expectMysqlQuery("select Vip from server where Sip='" SIP "' and Sport='" P0 "'");
	expectMysqlStoreResult();
	expectMysqlNumRows(1);
	expectMysqlFetchRow(row);
	expectMysqlQuery("update server set Sflag=3 where Sip='" SIP "' and Sport='" P0 "' and Vip='" VIP "'");
	expectIptablesDel0();
	expectMysqlFreeResult();

	case6(stderr, SIP);
}

TEST(GenForward, case7_no_port)
{
	V_port[0][0] = '\0';

	case7(stderr, SIP);
}

TEST(GenForward, case7_zero_row)
{
	strcpy(V_port[0], P0);
	V_port[1][0] = '\0';

	expectMysqlQuery("select Vip from server where Sip='" SIP "' and Sport='" P0 "'");
	expectMysqlStoreResult();
	expectMysqlNumRows(0);
	expectMysqlFreeResult();

	case7(stderr, SIP);
}

TEST(GenForward, case7_one_row)
{
	const char	*row[] = {VIP};
	strcpy(V_port[0], P0);
	V_port[1][0] = '\0';

	expectMysqlQuery("select Vip from server where Sip='" SIP "' and Sport='" P0 "'");
	expectMysqlStoreResult();
	expectMysqlNumRows(1);
	expectMysqlFetchRow(row);
	expectMysqlQuery("update server set Sflag=0 where Sip='" SIP "' and Sport='" P0 "' and Vip='" VIP "'");
	expectIptablesAdd0();
	expectMysqlFreeResult();

	case7(stderr, SIP);
}

TEST(GenForward, case8_no_port)
{
	V_port[0][0] = '\0';

	case8(stderr, SIP);
}

TEST(GenForward, case8_no_vip)
{
	strcpy(V_port[0], P0);

	expectMysqlQuery("select Vip from server where Sip='" SIP "'");
	expectMysqlStoreResult();
	expectMysqlNumRows(0);
	expectMysqlFreeResult();

	expectMysqlQuery("select Vip,NetMask from useip where Vflag=0 and Fzone=1 limit 0,1");
	expectMysqlStoreResult();
	expectMysqlNumRows(0);
	expectMysqlFreeResult();

	case8(stderr, SIP);
}

TEST(GenForward, case8_vip_in_server)
{
	const char	*row[] = {VIP};
	strcpy(V_port[0], P0);

	expectMysqlQuery("select Vip from server where Sip='" SIP "'");
	expectMysqlStoreResult();
	expectMysqlNumRows(1);
	expectMysqlFetchRow(row);
	expectAddVip();
	expectMysqlFreeResult();

	case8(stderr, SIP);
}

TEST(GenForward, case8_vip_in_useip)
{
	const char	*row[] = {VIP, "NetMask"};
	strcpy(V_port[0], P0);

	expectMysqlQuery("select Vip from server where Sip='" SIP "'");
	expectMysqlStoreResult();
	expectMysqlNumRows(0);
	expectMysqlFreeResult();

	expectMysqlQuery("select Vip,NetMask from useip where Vflag=0 and Fzone=1 limit 0,1");
	expectMysqlStoreResult();
	expectMysqlNumRows(1);
	expectMysqlFetchRow(row);
	expectIfcfgAdd();
	expectMysqlQuery("update useip set Vflag=1 where vip='" VIP "'");
	expectAddVip();
	expectMysqlFreeResult();

	case8(stderr, SIP);
}

TEST(GenForward, case9)
{
	case9(stderr, SIP);
}

TEST(GenForward, Response)
{
	Response(stderr, "");
}

