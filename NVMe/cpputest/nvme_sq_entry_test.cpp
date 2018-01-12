#include "CppUTest/TestHarness.h"

extern "C" {
	#include "nvme.h"
}

TEST_GROUP(NVME_SQ_ENTRY)
{
};

TEST(NVME_SQ_ENTRY, size)
{
	CHECK_EQUAL(1, sizeof (UINT8));
	CHECK_EQUAL(2, sizeof (UINT16));
	CHECK_EQUAL(4, sizeof (UINT32));
	CHECK_EQUAL(8, sizeof (UINT64));
	CHECK_EQUAL(64, sizeof (NVME_SQ_ENTRY));
}
