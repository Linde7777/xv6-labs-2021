#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

void run(char *program_name, char *args[]) {
  if (fork() == 0) { 
    exec(program_name, args);
  }
}

int main(int argc, char *argv[]) {
  // the right part of the completed argument is in the stdin
  char stdin_buffer[1024];
  char *p_right = stdin_buffer;
  char completed_argument[2048];
  char *program_name = argv[1];
  char *left_part_argument = argv[2];

  char *p_left = left_part_argument;
  char *p_comp = completed_argument;
  while (*p_left != '\0') {
    *p_comp = *p_left;
    p_left += 1;
    p_comp += 1;
  }
  *p_comp = ' ';
  p_comp+=1;

  // recall that file descriptor 0 point to standard input
  while (read(0, p_right, 1) != 0) {
    if (*p_right != '\n') {
      *p_comp = *p_right;
      p_comp += 1;
    } else {
      //printf("completed_argument:%s\n", completed_argument);
      *p_comp='\0';
      char *args[2];
      args[0] = program_name;
      args[1] = completed_argument;
      printf("args[0]:%s\n",*args);
      printf("args[1]:%s\n",*(args+1));
      if(fork()==0){
        exec(program_name,args);
      }
    }

  }

  exit(0);
}