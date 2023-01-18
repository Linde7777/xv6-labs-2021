#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

int main(int argc, char *argv[]) {
  char* args[2]={"echo","test"};
  int pid=fork();
  if(pid<0){
    printf("fork failed\n");
  }
  if(pid==0){
    exec(args[0],args);
    printf("exec failed\n");
    exit(1);
  }
  if(pid>0){
    wait(0);
    printf("finish waiting\n");
  }

  exit(0);
}