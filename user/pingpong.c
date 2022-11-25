#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#define LEN 4

int
main(){
  char arr[]="abc";
  if(fork()==0){
    char* child_buffer=(char*)malloc(sizeof(char)*(LEN+1));
    close(0);
    read(0,child_buffer,LEN);
    fprintf(2,"%d: received ping\n",getpid());

    close(0);
    close(1);
    write(1,child_buffer,LEN);
    exit(0);
  }else{
    close(1);
    write(1,arr,LEN);
    wait(0);

    char* parent_buffer=(char*)malloc(sizeof(char)*(LEN+1));
    close(0);
    read(0,parent_buffer,LEN);
    fprintf(2,"%d: received pong\n",getpid());
    exit(0);
  }

  exit(0);
}