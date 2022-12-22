#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#define ARGUMENT_LEN 20

char *concat(char *s1, char *s2) {
  int len1 = strlen(s1);
  int len2 = strlen(s2);
  int len3 = len1 + len2;
  char *s3 = (char *)malloc(sizeof(char) * len3);

  char *p1 = s1;
  char *p2 = s2;
  char *p3 = s3;
  while (*p1 != '\0') {
    *p3 = *p1;
  }
  while (*p2 != '\0') {
    *p3 = *p2;
  }
  p3 += 1;
  *p3 = '\0';

  return s3;
}

void xargs(char *program_name, char *argument1) {
  // TODO: free it
  char *argument2 = (char *)malloc(sizeof(char) * ARGUMENT_LEN);
  while (gets(argument2, ARGUMENT_LEN)) {
    if (fork() == 0) {
      printf("enter child process\n");
      char *argv[1];
      argv[0] = concat(argument1, argument2);
      exec(program_name, argv);
    } else {
      wait(0);
      printf("wait completed, enter parent process\n");
    }
  }
}

int main(int argc, char *argv[]) {
  if (argc <= 1) {
    printf("Usage 1: xargs program_name\n");
    printf("Usage 2: xargs program_name program_arguments\n");
  }

  char *program_name = argv[1];
  char *argument1 = "";
  if (argc == 3) {
    argument1 = argv[2];
  }

  xargs(program_name, argument1);
  exit(0);
}