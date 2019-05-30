#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

void *consumer(void *pData); //Consumer
char buffer[10];
int n = 10;
int in = 0;
int out = 0;

int main(int argc, char **argv)
{
 char nextp; int i;
 pthread_t tid;
 if( pthread_create(&tid,NULL,consumer,NULL) != 0) // 쓰레드 생성
 {
  printf("fail to pthread_create \n");
  return 0;
 }
 for(i = 0; i < 50; i++)
 {  
  nextp = 'P';
  while((in+1)%n == out); //버퍼가 꽉차면 Consumer가 소비할때까지 대기
  printf("Produce an item : %c\n", nextp); 
  buffer[in] = nextp; // 공유하는 버퍼에 넣는다.
  in++;
  in %= n; //버퍼를 참조하는 인덱스 값 설정(인덱스 값이n 까지 갈경우 인덱스 0부터 다시 참조)
  
 }
 pthread_join(tid,NULL);
}

void *consumer(void *pData)
{
 int i;
 char nextc;
 for(i=0; i<50; i++)
 {
  while(in == out); // Producer가 생산할때까지 대기
  nextc = buffer[out]; 
  out++;
  out %= n; //버퍼를 참조하는 인덱스 값 설정(인덱스 값이 n까지갈경우 인덱스 0부터다시 참조)
  printf("Consume the item : %c \n",nextc);
 }
}
