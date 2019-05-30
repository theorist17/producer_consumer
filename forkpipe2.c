#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#define BUFFERSIZE 64 //���� ������ ����

int main(void)
{
 char buffer[BUFFERSIZE]; // Consumer���� Producer�κ��� pipe�� ���� ������� �����͸� ���� ����
 char bufcheck[BUFFERSIZE]; //���۰� ��á�� Ȯ���ϴ� ����
 int fd[2]; // ����, �Һ� ���� ������ (���� ������1)
 int fd2[2]; // ���۰� �� á���� ����ϱ����� ������ (���� ������2)
 int i;
 int in,out=0; //������ ���� �ε�����

 if(pipe(fd) == -1) //������1 ����
 {
  perror("pipe error.. \n");
  exit(0);
 } 
 if(pipe(fd2) == -1)
 {
  perror("pipe error.. \n");
  exit(0);
 }

 pid_t pid;
 pid = fork();

 if(pid == -1) // ������2 ����
 {
  perror("fork error... \n");
  exit(0);
 }

 else if(pid == 0)//�ڽ����μ���(Producer)
 {
  int nread=0;
  for(i = 0; i < 200 ; i++) // 200�� �ݺ� (���ۻ������ 64)
  {
        printf("Produce an item\n");
        in++;
        in %= BUFFERSIZE;
        write(fd[1], "A", 2); //������1�� ���� 2����Ʈ�� ����

        if((nread = read(fd2[0], bufcheck ,BUFFERSIZE))-1 == (in+1)%BUFFERSIZE)
//������2�� ���Ͽ� ���۰� ����á���� Ȯ��
        {
         printf("Buffer is Full\n");
         sleep(1); //����á�� �� Consumer�� �Һ��Ҷ����� ���
        }     
   }
  }
 
  
 else //�θ� ���μ���(Consumer)
 {
  int nread;
  for(i = 0; i<200 ; i++)
  {
   write(fd2[1],"Buffer is Full Signal",out+1); //������2�� ���� Consumer���� �����ϴ�  �ε������� (���۰� ����á������ �˸�������)  Producer���� ����
   while((nread = read(fd[0], buffer, 2)) <= 0); //Producer�� ������ �ϸ� �о�� Consume�Ѵ�. (������ �ȵ��ִµ� �Һ��ϴ� ��츦 ����)
   printf("Consume the item : %s / Buffer : %d\n", buffer, out);
   out++;
   out %= BUFFERSIZE;
  }
 }

 exit(0);
}
