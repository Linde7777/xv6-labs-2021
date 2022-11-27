#include "kernel/stat.h"
#include "kernel/types.h"
#include "user/user.h"

void new_proc(int fd[2]);

int main() {
  int fd[2];
  pipe(fd);

  if (fork() == 0) {
    new_proc(fd);
  } else {
    for (int i = 2; i <= 35; i++) {
      close(fd[0]);
      write(fd[1], &i, sizeof(int));
    }

    wait(0);
    exit(0);
  }

  exit(0);
}

void new_proc(int fd[2]) {
  int p;
  close(fd[1]);
  read(fd[0], &p, sizeof(int));
  fprintf(2, "prime %d\n", p);

  int next_fd[2];
  pipe(next_fd);
  int n;
  if (fork() == 0) {
    new_proc(next_fd);
  } else {

    while (read(fd[0], &n, sizeof(int) != 0)) {
      if (n % p != 0) {
        close(next_fd[0]);
        write(next_fd[1], &n, sizeof(int));
      }
    }
    wait(0);
    exit(0);
  }

}