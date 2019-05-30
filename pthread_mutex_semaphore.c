#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

int buffer[100], count = 0; // shared variables : cyclic buffer & its usage counter
sem_t full, empty, mutex; // semaphores declared among thread, hence shared variables

void* produce(void *pData)
{
 	int i, in = -1;
 	for (i=0; i<1000; i++)
 	{
  		sem_wait(&empty); // producer waits on an empty buffer (woken by a consume)
		sem_wait(&mutex); // without locking the mutex (prevent blocking with resource allocation)
   		in++; in %= 100;
   		buffer[in] = i;
  		count++;
  		printf("Produced data = %d, count = %d\n", buffer[in], count);
		sem_post(&mutex); // unlock the mutex first (so consumer can have it right after wake)
  		sem_post(&full); // then wake consumer
 	}
	return NULL;
}

void* consume(void *pData)
{
  	int i, out = -1;
  	for (i=0; i<1000; i++)
  	{
   		sem_wait(&full); // consumer waits on a full buffer (woken by a new produce)
		sem_wait(&mutex); // without locking the mutex (prevent blocking with resource allocation)
    	out++; out %= 100;
   		count--;
  		printf("Consumed data = %d, count = %d\n", buffer[out], count);
   		sem_post(&mutex); // unlock the mutex first (so producer can have it right after wake)
		sem_post(&empty); // then wake producer
  	}
	return NULL;
}

int main(void)
{
 	sem_init(&full, 0, 0); // among 100 buffer, none of them are full at the beginning
 	sem_init(&empty, 0, 100); // and all 100 of them are empty
	sem_init(&mutex, 0, 1); // semaphore with initial value 1 is a mutex

 	pthread_t producer, consumer;
 	pthread_create(&producer, NULL, produce, NULL);
 	pthread_create(&consumer, NULL, consume, NULL);
  	pthread_join(producer, NULL);
	pthread_join(consumer, NULL);
 	return 0;
}

