#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#define BUFFER_SIZE 100
#define TEST_CASE 1000

int buffer[BUFFER_SIZE];
int count = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t full = PTHREAD_COND_INITIALIZER;
pthread_cond_t empty = PTHREAD_COND_INITIALIZER;
void* produce(void *pData);
void* consume(void *pData);
struct thread_args {
	int capacity;
	constchar *name;
};

int main(void)
{
	pthread_t producer1;
	struct thread_args *p1args = malloc (sizeof (struct thread_args));
	p1args->capacity = TEST_CASE;
	p1args->name = "Theorist";
	pthread_create(&producer1, NULL, produce, p1args);
	
	pthread_t consumer1;
	struct thread_args *c1args = malloc (sizeof (struct thread_args));
	c1args->capacity = TEST_CASE;
	c1args->name = "Jonghee";
	pthread_create(&consumer1, NULL, consume, c1args);

	pthread_join(producer1, NULL);
	printf("Thread %s terminated.\n", p1args->name);
	pthread_join(consumer1, NULL);
	printf("Thread %s terminated.\n", c1args->name);
	
	return 0;
}

void* produce(void *_args)
{
	struct thread_args *args = (struct thread_args*) _args;
	
	printf("Capacity : %d, Name : %s ", args->capacity, args->name);
	printf("joined.\n");

	int i, out = -1;
	for (i = 0; i < args->capacity; i++)
	{
		pthread_mutex_lock(&mutex);
		if (count == BUFFER_SIZE)
		{
			printf("Produced to full buffer...\n");
			pthread_cond_wait(&full, &mutex);
		}
		count++;
		out++;	out %= args->capacity;
		buffer[out] = i;
		printf("Capacity : %d, Name : %s produced %d, Count : %d.\n",
			args->capacity, args->name, buffer[out], count);
		pthread_cond_signal(&empty);
		pthread_mutex_unlock(&mutex);
	}
	return NULL;
}

void* consume(void *_args)
{
	struct thread_args *args = (struct thread_args*) _args;
	printf("Capacity : %d, Name : %s ", args->capacity, args->name);
	printf("joined.\n");
	int i, in = -1, data;
	for (i = 0; i < args->capacity; i++)
	{
		pthread_mutex_lock(&mutex);
		if (count == 0)
		{
			printf("Consumed to empty buffer...\n");
			pthread_cond_wait(&empty, &mutex);
		}
		in++; in %= args->capacity;
		count--;
		printf("Capacity : %d, Name : %s consumed %d, Count : %d.\n", 
			args->capacity, args->name, buffer[in], count);
		pthread_cond_signal(&full);
		pthread_mutex_unlock(&mutex);

	}
	return NULL;
}
