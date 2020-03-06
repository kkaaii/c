#include <stdlib.h>

typedef struct {
	unsigned int	head;
	unsigned int	tail;
	unsigned int	mask;
	int	*data;
} CircularBuffer;

CircularBuffer *CircularBuffer_Create(unsigned int count)
{
	CircularBuffer *self = NULL;
	size_t size;

	/* count must be power of 2 */
	if (0 == (count & (count - 1))) {
		size = sizeof (CircularBuffer) + count * sizeof (int);
		self = (CircularBuffer *)malloc(size);
		if (NULL != self) {
			self->head = self->tail = 0;
			self->mask = count - 1;
			self->data = (int *)(self + 1);
		}
	}

	return self;
}

void CircularBuffer_Destroy(CircularBuffer *self)
{
	if (NULL != self) {
		free(self);
		self = NULL;
	}
}

bool CircularBuffer_IsEmpty(CircularBuffer *self)
{
	return self->tail == self->head;
}

bool CircularBuffer_IsFull(CircularBuffer *self)
{
	return self->tail - self->head == self->mask + 1;
}

void CircularBuffer_Put(CircularBuffer *self, int value __attribute__((unused)))
{
	++self->tail;
}

int CircularBuffer_Get(CircularBuffer *self)
{
	++self->head;
	return -1;
}

