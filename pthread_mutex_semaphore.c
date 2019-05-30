#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

int buffer[100];
int count = 0;
int in = -1, out = -1;
sem_t full, empty, mutex;

//pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
void* producer(void *pData);
void* consumer(void *pData);

int main(void)
{
	int i;
 	sem_init(&full, 0, 0);
 	sem_init(&empty, 0, 100);
	sem_init(&mutex, 0, 1);

 	pthread_t threads[2];
 	pthread_create(&threads[0], NULL, producer, NULL);
 	pthread_create(&threads[1], NULL, consumer, NULL);
 	for (i=0; i<2; i++)
  		pthread_join(threads[i], NULL);
 	return 0;
}

void* producer(void *pData)
{
 	int i;
 	for (i=0; i<1000; i++)
 	{
  		//pthread_mutex_lock(&mutex);
  		sem_wait(&empty);
		sem_wait(&mutex);
   		in++; in %= 100;
   		buffer[in] = i;
  		count++;
  		printf("Produce data = %d, count = %d\n", buffer[in], count);
		sem_post(&mutex);
  		sem_post(&full);
  		//pthread_mutex_unlock(&mutex);
 
 	}
	return NULL;
}
void* consumer(void *pData)
{
  	int i,data;
  	for (i=0; i<1000; i++)
  	{
   		//pthread_mutex_lock(&mutex);
   		sem_wait(&full);
		sem_wait(&mutex);
    	out++; out %= 100;
    	data = buffer[out];
   		count--;
  		printf("Consume data = %d, count = %d\n", buffer[out], count);
   		sem_post(&mutex);
		sem_post(&empty);
   		//pthread_mutex_unlock(&mutex);
  	}
	return NULL;
}

