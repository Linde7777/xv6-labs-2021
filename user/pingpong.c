#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#define LEN 4

int main() {
  char arr[] = "abc";
  int fd[2];
  pipe(fd);
  if (fork() == 0) {
    char *child_buffer = (char *)malloc(sizeof(char) * (LEN + 1));
    close(fd[1]);
    read(fd[0], child_buffer, LEN);
    printf("%d: received ping\n", getpid());

    close(fd[0]);
    write(fd[1], child_buffer, LEN);
    free(child_buffer);
    exit(0);
  } else {
    close(fd[0]);
    write(fd[1], arr, LEN);
    wait(0);

    char *parent_buffer = (char *)malloc(sizeof(char) * (LEN + 1));
    close(fd[1]);
    read(fd[0], parent_buffer, LEN);
    printf("%d: received pong\n", getpid());
    free(parent_buffer);
    exit(0);
  }

  exit(0);
}