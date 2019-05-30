#include <stdio.h>
#include <pthread.h>

int buffer[100];
int count = 0;
int in = -1, out = -1;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
void* producer(void *pData);
void* consumer(void *pData);

int main(void)
{
 int i;
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
  while(count == 100);
  pthread_mutex_lock(&mutex); 
   in++;
   in %= 100;
   buffer[in] = i;
   count++;
  pthread_mutex_unlock(&mutex);
   printf("Produce data = %d\n", buffer[in]);
 }
}
void* consumer(void *pData)
{
int i,data;
  for (i=0; i<300; i++)
  {
   while(count == 0);
   pthread_mutex_lock(&mutex);
    out++;
    out %= 100;
    data = buffer[out];
    count--;
  pthread_mutex_unlock(&mutex);
  printf("Consume data = %d\n", data);
  }
}
