typedef struct {
	int	head;
	int	tail;
} CircularBuffer;

bool CircularBuffer_IsEmpty(CircularBuffer *self)
{
	return self->tail == self->head;
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

