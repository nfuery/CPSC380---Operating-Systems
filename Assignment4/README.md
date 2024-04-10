## Information
* Name: Noah Fuery
* Student ID: 2377792
* Course: CPSC 380 - Operating Systems
* Date: Spring 2024

## Program instructions
* gcc -o prodcon prodcon.c buffer.c -lpthread
* ./prodcon delay NumOfProducers NumOfConsumers

## Program detail
Takes in a delay, number of producers, and number of consumer on the command line and performs the producer-consumer problem without any deadlocking issues for the elapsed delay time. Each producer is a thread that inserts items into a shared buffer between producers and consumers. Each consumer is also a thread that removes items from the same buffer. When inserting or removing items, semaphores and mutex locks are utilized to make sure items are inserted when the semaphore is flagged as empty and items are removed when the semaphore is flagged as full. The mutex locks are used to ensure the critical sections are protected from race conditions.