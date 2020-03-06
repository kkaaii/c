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

size_t CircularBuffer_GetCount(CircularBuffer *self)
{
	return self->tail - self->head;
}

void CircularBuffer_Enqueue(CircularBuffer *self, int value)
{
	self->data[self->tail & self->mask] = value;
	++self->tail;
}

void CircularBuffer_Dequeue(CircularBuffer *self)
{
	++self->head;
}

int CircularBuffer_GetData(CircularBuffer *self)
{
	return self->data[self->head & self->mask];
}

