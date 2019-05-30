#include <stdio.h>
#include <pthread.h>
int buffer[100], count = 0; // shared variables : cyclic buffer and its usage counter
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; // init mutex
pthread_cond_t buffer_has_space = PTHREAD_COND_INITIALIZER; // init condition variables
pthread_cond_t buffer_has_data = PTHREAD_COND_INITIALIZER;

void* produce(void *pData)
{
	int i, in = -1;
	for (i=0; i<1000; i++)
 	{
 		pthread_mutex_lock(&mutex); // lock the mutex
  		if(count == 100) // if all 100 buffer using
   			pthread_cond_wait(&buffer_has_space, &mutex); // unlock and block producer
		in++; in %= 100;
   		buffer[in] = i;
		count++;
  		pthread_cond_signal(&buffer_has_data); // wake consumer
		printf("Produce data = %d, in = %d, count = %d\n", buffer[in], in, count);
  		pthread_mutex_unlock(&mutex); // unlock
	 }
}

void* consume(void *pData)
{
	int i, out = -1;
 	for (i=0; i<1000; i++)
  	{
   		pthread_mutex_lock(&mutex); // lock the mutex
    	if(count == 0) // if no buffer using
     		pthread_cond_wait(&buffer_has_data, &mutex); // unlock and block consumer
    	out++; out %= 100;
    	count--;
    	pthread_cond_signal(&buffer_has_space); // wake producer
		printf("Consumed data = %d, out = %d, count = %d\n", buffer[out], out, count);
  		pthread_mutex_unlock(&mutex); // unlock
  	}	
}

int main(void)
{
	int i;
	pthread_t producer, consumer;
	pthread_create(&producer, NULL, produce, NULL);
	pthread_create(&consumer, NULL, consume, NULL);
	pthread_join(producer, NULL);
	pthread_join(consumer, NULL);
	return 0;
}

