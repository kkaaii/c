The design uses separated function CircularBuffer_Dequeue() and CircularBuffer_GetData(), to adjust the pointer and return data, respectively. By doing so, the consumer code can be clean: 

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

If we merge the function CircularBuffer_Dequeue() and CircularBuffer_GetData() together. The cosumer code could become ugly and change the data order in the buffer: 
```
void Consumer()
{
    if (!CircularBuffer_IsEmpty(self)) {
        data = CircularBuffer_Dequeue(self);

        if (!CheckCondition1()) {
            CircularBuffer_Enqueue(self); // !!! may change the data order in the circular buffer
            return;
        }

        if (!CheckCondition2()) {
            CircularBuffer_Enqueue(self); // !!! may change the data order in the circular buffer
            return;
        }

        ProcessData(data);
    }
 }
```
