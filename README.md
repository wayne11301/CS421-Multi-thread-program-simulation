# Multi-thread-program-simulation

This program allow users to simulate a solution to producer-consumer problem by semaphores and mutex lock.

- Users can decide the number of producer and consumer threads
- When a producer thread gets executed, 1 random number will be placed in a buffer. In contrast, when a consumer thread gets executed, 1 number will be removed
- When the buffer is full, producer threads cannot be executed
- When the buffer is empty, consumer threads cannot be executed
- Producers and consumers will sleep 1-3 seconds after each execution


### Video explanation
https://youtu.be/Qx3P2wazwI0?t=75

### Sample Output
Please refer to the typescript file

### Walkthough GIF

<img src='test run.gif' title='test run' width='800' alt='test run' />

GIF created with [LiceCap](http://www.cockos.com/licecap/).
