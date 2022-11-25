#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  if(argc!=2){
    fprintf(2, "Error: you should enter a number\n");
    exit(1);
  }else{
    int time=atoi(*argv);
	  sleep(time);
    exit(0);
  }
  
}