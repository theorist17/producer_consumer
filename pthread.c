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
 if( pthread_create(&tid,NULL,consumer,NULL) != 0) // ������ ����
 {
  printf("fail to pthread_create \n");
  return 0;
 }
 for(i = 0; i < 50; i++)
 {  
  nextp = 'P';
  while((in+1)%n == out); //���۰� ������ Consumer�� �Һ��Ҷ����� ���
  printf("Produce an item : %c\n", nextp); 
  buffer[in] = nextp; // �����ϴ� ���ۿ� �ִ´�.
  in++;
  in %= n; //���۸� �����ϴ� �ε��� �� ����(�ε��� ����n ���� ����� �ε��� 0���� �ٽ� ����)
  
 }
 pthread_join(tid,NULL);
}

void *consumer(void *pData)
{
 int i;
 char nextc;
 for(i=0; i<50; i++)
 {
  while(in == out); // Producer�� �����Ҷ����� ���
  nextc = buffer[out]; 
  out++;
  out %= n; //���۸� �����ϴ� �ε��� �� ����(�ε��� ���� n��������� �ε��� 0���ʹٽ� ����)
  printf("Consume the item : %c \n",nextc);
 }
}
