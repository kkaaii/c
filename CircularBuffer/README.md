```
void Consumer()
{
    if (!CircularBuffer_IsEmpty(self)) {
        data = CircularBuffer_Dequeue(self);

        if (!CheckCondition1()) {
            CircularBuffer_Enqueue(self);
            return;
        }

        if (!CheckCondition2()) {
            CircularBuffer_Enqueue(self);
            return;
        }

        ProcessData(data);
    }
 }
```

```
void Consumer()
{
    if (!CircularBuffer_IsEmpty(self)) {
        data = CircularBuffer_GetData(self);

        if (!CheckCondition1())
            return;

        if (!CheckCondition2())
            return;

        ProcessData(data);
        CircularBuffer_Dequeue(self);
    }
 }
```
