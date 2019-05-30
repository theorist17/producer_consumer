#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#define BUFFERSIZE 64 //버퍼 사이즈 지정

int main(void)
{
 char buffer[BUFFERSIZE]; // Consumer에서 Producer로부터 pipe를 통해 생산받은 데이터를 받을 버퍼
 char bufcheck[BUFFERSIZE]; //버퍼가 다찼지 확인하는 버퍼
 int fd[2]; // 생산, 소비를 위한 파이프 (이하 파이프1)
 int fd2[2]; // 버퍼가 다 찼는지 통신하기위한 파이프 (이하 파이프2)
 int i;
 int in,out=0; //버퍼의 참조 인덱스값

 if(pipe(fd) == -1) //파이프1 생성
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

 if(pid == -1) // 파이프2 생성
 {
  perror("fork error... \n");
  exit(0);
 }

 else if(pid == 0)//자식프로세스(Producer)
 {
  int nread=0;
  for(i = 0; i < 200 ; i++) // 200번 반복 (버퍼사이즈는 64)
  {
        printf("Produce an item\n");
        in++;
        in %= BUFFERSIZE;
        write(fd[1], "A", 2); //파이프1을 통해 2바이트씩 생산

        if((nread = read(fd2[0], bufcheck ,BUFFERSIZE))-1 == (in+1)%BUFFERSIZE)
//파이프2를 통하여 버퍼가 가득찼는지 확인
        {
         printf("Buffer is Full\n");
         sleep(1); //가득찼을 시 Consumer가 소비할때까지 대기
        }     
   }
  }
 
  
 else //부모 프로세스(Consumer)
 {
  int nread;
  for(i = 0; i<200 ; i++)
  {
   write(fd2[1],"Buffer is Full Signal",out+1); //파이프2를 통해 Consumer에서 참조하는  인덱스값을 (버퍼가 가득찼는지를 알리기위해)  Producer에게 전송
   while((nread = read(fd[0], buffer, 2)) <= 0); //Producer가 생산을 하면 읽어와 Consume한다. (생산이 안되있는데 소비하는 경우를 방지)
   printf("Consume the item : %s / Buffer : %d\n", buffer, out);
   out++;
   out %= BUFFERSIZE;
  }
 }

 exit(0);
}
