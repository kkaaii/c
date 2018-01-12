#include "CppUTest/TestHarness.h"

extern "C" {
	#include "nvme_queue.c"
}

TEST_GROUP(NVME_QUEUE)
{
	NVME_QUEUE	q;

	void setup() {
		NVME_QUEUE_INIT(&q, 7);
	}
};

TEST(NVME_QUEUE, update_tail_over_head_case2)
{
	/*
	**           H
	** X X x v v X X X
	**     T
	*/
	(NvmeQ_UpdateTail(&q, 5));
	(NvmeQ_UpdateHead(&q, 5));
	CHECK(NvmeQ_UpdateTail(&q, 2));

	CHECK(!NvmeQ_UpdateTail(&q, 1));
	CHECK(!NvmeQ_UpdateTail(&q, 5));
}

TEST(NVME_QUEUE, update_tail_over_head_case1)
{
	/*
	**     H
	** v v X X X x v v
	**           T
	*/
	(NvmeQ_UpdateTail(&q, 5));
	(NvmeQ_UpdateHead(&q, 2));

	CHECK(!NvmeQ_UpdateTail(&q, 2));
	CHECK(!NvmeQ_UpdateTail(&q, 4));
}

TEST(NVME_QUEUE, update_head_over_tail_case2)
{
	/*
	**           H
	** v v v X X x v v
	**     T
	*/
	CHECK(NvmeQ_UpdateTail(&q, 5));
	CHECK(NvmeQ_UpdateHead(&q, 5));
	CHECK(NvmeQ_UpdateTail(&q, 2));

	CHECK(!NvmeQ_UpdateHead(&q, 3));
	CHECK(!NvmeQ_UpdateHead(&q, 4));
}

TEST(NVME_QUEUE, update_head_over_tail_case1)
{
	/*
	**     H
	** X X x v v v X X
	**           T
	*/
	CHECK(NvmeQ_UpdateTail(&q, 5));
	CHECK(NvmeQ_UpdateHead(&q, 2));

	CHECK(!NvmeQ_UpdateHead(&q, 6));
	CHECK(!NvmeQ_UpdateHead(&q, 1));
}

TEST(NVME_QUEUE, update_tail_repeat)
{
	(NvmeQ_UpdateTail(&q, 2));
	CHECK(!NvmeQ_UpdateTail(&q, 2));
}

TEST(NVME_QUEUE, update_head_repeat)
{
	(NvmeQ_UpdateTail(&q, 2));
	CHECK(!NvmeQ_UpdateHead(&q, 0));
}

TEST(NVME_QUEUE, update_tail_when_queue_full)
{
	(NvmeQ_UpdateTail(&q, 2));
	(NvmeQ_UpdateHead(&q, 2));
	CHECK(NvmeQ_UpdateTail(&q, 1));
	CHECK(NVME_QUEUE_IS_FULL(&q));
	CHECK(!NvmeQ_UpdateTail(&q, 2));
}

TEST(NVME_QUEUE, update_head_when_queue_empty)
{
	(NvmeQ_UpdateTail(&q, 2));
	(NvmeQ_UpdateHead(&q, 2));
	CHECK(!NvmeQ_UpdateHead(&q, 2));
}

TEST(NVME_QUEUE, queue_empty)
{
	CHECK(NvmeQ_UpdateTail(&q, 2));
	CHECK(NvmeQ_UpdateHead(&q, 2));
	CHECK(NVME_QUEUE_IS_EMPTY(&q));
}

TEST(NVME_QUEUE, queue_full)
{
	CHECK(NvmeQ_UpdateTail(&q, 7));
	CHECK(NVME_QUEUE_IS_FULL(&q));
}

TEST(NVME_QUEUE, update_tail_out_of_range)
{
	CHECK(!NvmeQ_UpdateTail(&q, 8));
}

TEST(NVME_QUEUE, update_head_out_of_range)
{
	CHECK(NvmeQ_UpdateTail(&q, 3));
	CHECK(!NvmeQ_UpdateHead(&q, 8));
}

TEST(NVME_QUEUE, empty_after_init)
{
	CHECK(NVME_QUEUE_IS_EMPTY(&q));
}

