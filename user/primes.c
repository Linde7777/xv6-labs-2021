#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main() {
  int fd[2];
  pipe(fd);
  if (fork() == 0) {
    int received_num;
    close(fd[1]);
    read(fd[0], &received_num, sizeof(int));
    fprintf(2, "receive num %d\n", received_num);
    exit(0);
  } else {
    int num = 3;
    close(fd[0]);
    write(fd[1], &num, sizeof(int));
    wait(0);
    exit(0);
  }

  exit(0);
}
