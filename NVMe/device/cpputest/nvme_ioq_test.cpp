#include "CppUTest/TestHarness.h"

extern "C" {
	#include "nvme_iocq.c"
	#include "nvme_iosq.c"
}

namespace {
	NVME_SQ_ENTRY	entry;

	void TestCreateIoCq(NVME_STATUS expected, UINT16 cqid=1, UINT16 qsize=15, UINT16 iv=0, bool ien=false) {
		entry.cdw10.createq.qid = cqid;
		entry.cdw10.createq.qsize = qsize;
		entry.cdw11.createIoCq.iv = iv;
		entry.cdw11.createIoCq.ien = ien;

		CHECK_EQUAL(expected, NvmeCq_Create(&entry));
	}

	void TestDeleteIoCq(NVME_STATUS expected, UINT16 cqid) {
		entry.cdw10.deleteq.qid = cqid;

		CHECK_EQUAL(expected, NvmeCq_Delete(&entry));
	}

	void TestCreateIoSq(NVME_STATUS expected, UINT16 cqid, UINT16 sqid=1, UINT16 qsize=15) {
		entry.cdw10.createq.qid = sqid;
		entry.cdw10.createq.qsize = qsize;
		entry.cdw11.createIoSq.cqid = cqid;

		CHECK_EQUAL(expected, NvmeSq_Create(&entry));
	}

	void TestDeleteIoSq(NVME_STATUS expected, UINT16 sqid) {
		entry.cdw10.deleteq.qid = sqid;

		CHECK_EQUAL(expected, NvmeSq_Delete(&entry));
	}
}


TEST_GROUP(NVME_IOSQ)
{
	virtual void setup() {
		NvmeCq_Init();
		NvmeSq_Init();
	}
};

TEST(NVME_IOSQ, delete_sqid_repeat)
{
	UINT16	cqid = 1;
	UINT16	sqid = 1;

	TestCreateIoCq(NVME_STATUS_SUCCESSFUL_COMPLETION, cqid);

	TestCreateIoSq(NVME_STATUS_SUCCESSFUL_COMPLETION, cqid, sqid);
	TestDeleteIoSq(NVME_STATUS_SUCCESSFUL_COMPLETION, sqid);
	TestDeleteIoSq(NVME_STATUS_INVALID_QID, sqid);
}

TEST(NVME_IOSQ, delete_sqid_out_of_range)
{
	UINT16	sqid = NVME_MAX_QUEUES;

	TestDeleteIoSq(NVME_STATUS_INVALID_QID, sqid);
}

TEST(NVME_IOSQ, delete_sqid_0)
{
	UINT16	sqid = 0;

	TestDeleteIoSq(NVME_STATUS_INVALID_QID, sqid);
}

TEST(NVME_IOSQ, create_qsize_too_big)
{
	UINT16	cqid = 1;
	UINT16	sqid = 1;
	UINT16	qsize = NVME_MAX_QSIZE + 1;

	TestCreateIoCq(NVME_STATUS_SUCCESSFUL_COMPLETION, cqid);
	TestCreateIoSq(NVME_STATUS_INVALID_QSIZE, cqid, sqid, qsize);
}

TEST(NVME_IOSQ, create_qsize_too_small)
{
	UINT16	cqid = 1;
	UINT16	sqid = 1;
	UINT16	qsize = NVME_MIN_QSIZE - 1;

	TestCreateIoCq(NVME_STATUS_SUCCESSFUL_COMPLETION, cqid);
	TestCreateIoSq(NVME_STATUS_INVALID_QSIZE, cqid, sqid, qsize);
}

TEST(NVME_IOSQ, create_sqid_repeat)
{
	UINT16	cqid = 1;
	UINT16	sqid = 1;

	TestCreateIoCq(NVME_STATUS_SUCCESSFUL_COMPLETION, cqid);

	CHECK(!NvmeSq_IsValid(sqid));
	TestCreateIoSq(NVME_STATUS_SUCCESSFUL_COMPLETION, cqid, sqid);

	CHECK(NvmeSq_IsValid(sqid));
	TestCreateIoSq(NVME_STATUS_INVALID_QID, cqid, sqid);
}

TEST(NVME_IOSQ, create_sqid_out_of_range)
{
	UINT16	cqid = 1;
	UINT16	sqid = NVME_MAX_QUEUES;

	TestCreateIoCq(NVME_STATUS_SUCCESSFUL_COMPLETION, cqid);
	TestCreateIoSq(NVME_STATUS_INVALID_QID, cqid, sqid);
}

TEST(NVME_IOSQ, create_sqid_0)
{
	UINT16	cqid = 1;
	UINT16	sqid = 0;

	TestCreateIoCq(NVME_STATUS_SUCCESSFUL_COMPLETION, cqid);
	TestCreateIoSq(NVME_STATUS_INVALID_QID, cqid, sqid);
}

TEST(NVME_IOSQ, create_cqid_invalid)
{
	UINT16	cqid = 1;
	UINT16	sqid = 1;

	CHECK(!NvmeCq_IsValid(cqid));
	TestCreateIoSq(NVME_STATUS_CQ_INVALID, cqid, sqid);
}

TEST_GROUP(NVME_IOCQ)
{
	NVME_SQ_ENTRY	entry;

	virtual void setup() {
		NvmeCq_Init();
		NvmeSq_Init();
	}
};

TEST(NVME_IOCQ, delete_cqid_in_use)
{
	UINT16	cqid = 1;
	UINT16	sqid = 1;

	TestCreateIoCq(NVME_STATUS_SUCCESSFUL_COMPLETION, cqid);
	TestCreateIoSq(NVME_STATUS_SUCCESSFUL_COMPLETION, sqid, cqid);
	TestDeleteIoCq(NVME_STATUS_INVALID_QUEUE_DELETION, cqid);
}

TEST(NVME_IOCQ, delete_cqid_repeat)
{
	UINT16	cqid = 1;

	TestCreateIoCq(NVME_STATUS_SUCCESSFUL_COMPLETION, cqid);
	TestDeleteIoCq(NVME_STATUS_SUCCESSFUL_COMPLETION, cqid);
	TestDeleteIoCq(NVME_STATUS_INVALID_QID, cqid);
}

TEST(NVME_IOCQ, delete_cqid_out_of_range)
{
	UINT16	cqid = NVME_MAX_QUEUES;

	TestDeleteIoCq(NVME_STATUS_INVALID_QID, cqid);
}

TEST(NVME_IOCQ, delete_cqid_0)
{
	UINT16	cqid = 0;

	TestDeleteIoCq(NVME_STATUS_INVALID_QID, cqid);
}

TEST(NVME_IOCQ, create_cqid_repeat)
{
	UINT16	cqid = 1;

	CHECK(!NvmeCq_IsValid(cqid));
	TestCreateIoCq(NVME_STATUS_SUCCESSFUL_COMPLETION, cqid);

	CHECK(NvmeCq_IsValid(cqid));
	TestCreateIoCq(NVME_STATUS_INVALID_QID, cqid);
}

TEST(NVME_IOCQ, create_cqid_out_of_range)
{
	UINT16	cqid = NVME_MAX_QUEUES;

	TestCreateIoCq(NVME_STATUS_INVALID_QID, cqid);
}

TEST(NVME_IOCQ, create_cqid_0)
{
	UINT16	cqid = 0;

	TestCreateIoCq(NVME_STATUS_INVALID_QID, cqid);
}

TEST(NVME_IOCQ, create_qsize_too_big)
{
	UINT16	cqid = 1;
	UINT16	qsize = NVME_MAX_QSIZE + 1;

	TestCreateIoCq(NVME_STATUS_INVALID_QSIZE, cqid, qsize);
}

TEST(NVME_IOCQ, create_qsize_too_small)
{
	UINT16	cqid = 1;
	UINT16	qsize = NVME_MIN_QSIZE - 1;

	TestCreateIoCq(NVME_STATUS_INVALID_QSIZE, cqid, qsize);
}

