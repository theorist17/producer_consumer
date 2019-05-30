#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#define BUFFERSIZE 256 
int main(void)
{ 
 char buffer[BUFFERSIZE];
 int fd[2];
 int i;
 
if(pipe(fd) == -1) 
 {
  perror("pipe error.. \n");
  exit(0);
 } 

 pid_t pid;
 pid = fork();

 if(pid == -1)
 {
  perror("fork error... \n");
  exit(0);
 }

 else if(pid == 0) // parent -> producer
 {
for(i = 0; i < 10 ; i++)
  {
        printf("Produce an item : Item\n");
        write(fd[1], "Item", BUFFERSIZE); 
   }
 }
 
  
 else  { // child -> consumer
  for(i = 0; i<10 ; i++)
  {
   int nread;
   while((nread = read(fd[0], buffer, BUFFERSIZE))<BUFFERSIZE); 
   printf("Consume the item : %s \n", buffer);
  }
 }

 exit(0);
}
