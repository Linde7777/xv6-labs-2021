#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
  // let's say echo hello too | xargs echo bye
  // argc is 3, we want final_argv be: "echo","bye","hello too"
  // so the number of arguments of final_argv is argc-1+1
  char *final_argv[argc];
  int final_argv_index = 0;
  for (int i = 1; i < argc; i++) {
    final_argv[final_argv_index] = argv[i];
    final_argv_index += 1;
  }

  char stdin_buffer[BUFFER_SIZE];
  read(0, stdin_buffer, BUFFER_SIZE);

  char *p_start = stdin_buffer;
  // let's say stdin_buffer is: hello\nbye
  // p_start point to 'h'
  // when we meet '\n' we know we meet the first line content,
  // we set '\n' to '\0', so p_start point to a string "hello\0",
  // we put "hello\0" into final_argv then call exec.
  // After this, we need to set p_start point to 'b', 
  // where the start of the next line content, and do the same thing.
  for (int i = 0; i < BUFFER_SIZE; i++) {
    if (stdin_buffer[i] == '\n') {
      stdin_buffer[i]='\0';

      int pid = fork();
      if (pid < 0) {
        printf("fork failed\n");
        exit(1);
      }
      if (pid == 0) {
        final_argv[final_argv_index] = p_start;
        exec(final_argv[0], final_argv);
        printf("exec failed\n");
        exit(1);
      }
      if(pid>0){
        p_start=&stdin_buffer[i+1];
        wait(0);
      }
    }
  }

  exit(0);
}