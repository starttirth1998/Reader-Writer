```
Code -> producer_consumer.c
Input format -> Buffer Size, Number of readers, Number of items writer want to write.
Compiler -> gcc -lpthread
```

Implementation
-> All thread for writer as well as readers are created
-> Before reading or writing in any buffer. **mutex for that buffer is locked** After finishing work it is unlocked back.
-> For writer thread there is a function of *writer* function.
-> Here thread is continued in while(1) loop till all the items he want to write has been written.
  After that his thread is joined with main thread.
-> For writing items there is *write_num* function. In this he finds the first position where he can write something.
-> For reader there is *reader* function. Here he generates a random number in which buffer he will read.
-> If all the items are read and writer thread is already closed. Then thread for that reader is joined with main.
