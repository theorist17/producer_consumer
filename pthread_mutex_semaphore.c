#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>


int buffer[100];
int count = 0;
int in = -1, out = -1;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
void* producer(void *pData);
void* consumer(void *pData);
sem_t full, empty;

int main(void)
{
 int i;
 sem_init(&empty,0,0);
 sem_init(&full,0,100);

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
 for (i=0; i<300; i++)
 {
  sem_wait(&full);
  pthread_mutex_lock(&mutex);
   in++;
   in %= 100;
   buffer[in] = i;
   count++;
   printf("Produce data = %d\n", buffer[in]);
  pthread_mutex_unlock(&mutex);
  sem_post(&empty);
 
 }
}
void* consumer(void *pData)
{
  int i,data;
  for (i=0; i<300; i++)
  {
   sem_wait(&empty);
   pthread_mutex_lock(&mutex);
    out++;
    out %= 100;
    data = buffer[out];
    count--;
   printf("Consume data = %d\n", data);
   pthread_mutex_unlock(&mutex);
   sem_post(&full);
 
  }
}
