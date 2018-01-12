#include "CppUTest/TestHarness.h"

extern "C" {
	#include "nvme.h"
}

TEST_GROUP(NVME_CQ_ENTRY)
{
};

TEST(NVME_CQ_ENTRY, size)
{
	CHECK_EQUAL(16, sizeof (NVME_CQ_ENTRY));
}

TEST(NVME_CQ_ENTRY, nvme_status)
{
	NVME_STATUS	nvmeStatus = NVME_STATUS_DATA_TRANSFER_ERROR;
	CHECK_EQUAL(0x00, SCT(nvmeStatus));
	CHECK_EQUAL(0x04, SC(nvmeStatus));

	nvmeStatus = NVME_STATUS_NAMESPACE_NOT_READY;
	CHECK_EQUAL(0x00, SCT(nvmeStatus));
	CHECK_EQUAL(0x82, SC(nvmeStatus));

	nvmeStatus = NVME_STATUS_INVALID_FORMAT;
	CHECK_EQUAL(0x01, SCT(nvmeStatus));
	CHECK_EQUAL(0x0A, SC(nvmeStatus));

	nvmeStatus = NVME_STATUS_CONFLICTING_ATTRIBUTES;
	CHECK_EQUAL(0x01, SCT(nvmeStatus));
	CHECK_EQUAL(0x80, SC(nvmeStatus));

	nvmeStatus = NVME_STATUS_COMPARE_FAILURE;
	CHECK_EQUAL(0x02, SCT(nvmeStatus));
	CHECK_EQUAL(0x85, SC(nvmeStatus));
}

