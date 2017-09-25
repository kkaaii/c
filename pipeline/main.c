/******************************************************************************
** File Name: main.c
** Author:
** Creation Time: Mon Sep 25 04:26:05 2017
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#ifdef X
static int rand_x(void)
{
	return 0;
}
#define	rand	rand_x
#endif

#define	RINGBUF_DEPTH	3

#define	RINGBUF_MAXN	(1 << RINGBUF_DEPTH)
#define	RINGBUF_MASK	(RINGBUF_MAXN - 1)

typedef char	task_t;

task_t	ringbuf[RINGBUF_MAXN];

#define	PIPELINE_N		4

unsigned int pipelines[1 + PIPELINE_N];

pthread_mutex_t lock;

static int is_empty(unsigned int i)
{
	return pipelines[i] == pipelines[i - 1];
}

static int is_full(void)
{
	return ((pipelines[0] + 1) & RINGBUF_MASK) == pipelines[PIPELINE_N];
}

static void put_task(task_t task)
{
	ringbuf[pipelines[0]] = task;
	pipelines[0] = (pipelines[0] + 1) & RINGBUF_MASK;
}

static task_t get_task(unsigned int i)
{
	task_t task = ringbuf[pipelines[i]];
	pipelines[i] = (pipelines[i] + 1) & RINGBUF_MASK;
	return task;
}

static char gen_task(void)
{
	static task_t next = '0';
	task_t task = next;
	next = ('9' == next) ? '0' : next + 1;
	return task;
}

static void *producer(void *arg)
{
	task_t task;

	for (;;) {
		if (!is_full()) {
			pthread_mutex_lock(&lock);
			task = gen_task();
			putchar(task);
			putchar('\n');
			put_task(task);
			pthread_mutex_unlock(&lock);
		}
		sleep(1 + (rand() & 3));
	}
}

static void *consumer(void *arg)
{
	task_t task;
	unsigned int pipeline = (intptr_t)(arg);
	unsigned int i;

	for (;;) {
		if (!is_empty(pipeline)) {
			pthread_mutex_lock(&lock);
			task = get_task(pipeline);
			for (i = 0; i < pipeline; ++i)
				putchar('\t');
			putchar(task);
			putchar('\n');
			pthread_mutex_unlock(&lock);
		}
		sleep(1 + (rand() & 3));
	}
}

#define	oops(s)	do { puts(s); return -1; } while (0)

int main(void)
{
	pthread_attr_t	attr;
	pthread_t	threads[1 + PIPELINE_N];
	void *res;
	int i;

	pthread_mutex_init(&lock, NULL);

	if (0 != pthread_attr_init(&attr)) {
		oops("ERROR in pthread_attr_init");
	}

	if (0 != pthread_create(&threads[0], &attr, producer, NULL)) {
		oops("ERROR in pthread_create producer");
	}

	for (i = 1; i <= PIPELINE_N; ++i) {
		if (0 != pthread_create(&threads[i], &attr, consumer, (void *)(intptr_t)i)) {
			oops("ERROR in pthread_create consumer");
		}
	}

	if (0 != pthread_attr_destroy(&attr)) {
		oops("ERROR in pthread_attr_destroy");
	}

	for (i = 0; i <= PIPELINE_N; ++i) {
		if (0 != pthread_join(threads[i], &res)) {
			oops("ERROR in pthread_join");
			break;
		}

		free(res);
	}

	return 0;
}

