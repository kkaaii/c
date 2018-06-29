typedef struct {
	int	tail;
} CircularBuffer;

bool CircularBuffer_IsEmpty(CircularBuffer *self)
{
	return self->tail == 0;
}

void CircularBuffer_Put(CircularBuffer *self, int value __attribute__((unused)))
{
	++self->tail;
}

