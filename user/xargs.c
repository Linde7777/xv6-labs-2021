#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#define ARGUMENT_LEN 100

void deleteNewLineSymbol(char *s) {
  char *p = s;
  while (*p != '\n') {
    if (*p == '\0') {
      exit(0);
    }
    p += 1;
  }
  *p = '\0';
}

void concat(char *s1, char *s2, char *dest) {
  int len = strlen(s1) + strlen(s2);
  char *temp = (char *)malloc(sizeof(char) * len);
  char *p1 = s1;
  char *p2 = s2;
  char *pt = temp;

  while (*p1 != '\0') {
    *pt = *p1;
    pt += 1;
    p1 += 1;
  }
  while (*p2 != '\0') {
    *pt = *p2;
    pt += 1;
    p2 += 1;
  }
  pt += 1;
  *pt = '\0';

  pt = temp;
  char *pd = dest;
  while (*pt != '\0') {
    *pd = *pt;
    pd += 1;
    pt += 1;
  }
  *pd = '\0';

  free(pt);
}

void xargs(char *program_name, char *argument1) {
  // TODO: free it
  char *argument2 = (char *)malloc(sizeof(char) * ARGUMENT_LEN);
  while (gets(argument2, ARGUMENT_LEN)) {
    if (strcmp(argument2, "") == 0) {
      printf("empty input from stdin, exit\n");
      exit(0);
    }

    printf("original argument2:%s\n", argument2);
    deleteNewLineSymbol(argument2);
    printf("new argument2:%s\n", argument2);

    // TODO: here is a problem
    concat(" ", argument2, argument2);
    printf("after adding space to argument2:%s\n", argument2);

    char *argv[2];
    argv[0] = program_name;
    argv[1] = (char *)malloc(sizeof(char) * ARGUMENT_LEN);
    concat(argument1, argument2, argv[1]);
    printf("after concat two argument, argv[1]:%s\n", argv[1]);
    printf("program_name:%s\n", program_name);
    printf("argv[0]:%s\n", argv[0]);
    printf("argv[1]:%s\n", argv[1]);
    if (fork() == 0) {
      printf("enter child process\n");
      // program_name:"echo", argv[0]:"echo", argv[1]:"byetoo"
      exec(program_name, argv);
    }

    printf("test1\n");
    for (;;) {}
    printf("test2\n");

  }
  free(argument2);
  exit(0);
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