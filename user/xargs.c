#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#define ARGUMENT_LEN 100

void deleteNewLineSymbol(char *s) {
  char *p = s;
  while (*p != '\n') {
    if(*p=='\0'){
      exit(0);
    }
    p += 1;
  }
  *p = '\0';
}

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
    p3 += 1;
    p1 += 1;
  }
  while (*p2 != '\0') {
    *p3 = *p2;
    p3 += 1;
    p2 += 1;
  }
  p3 += 1;
  *p3 = '\0';

  return s3;
}

void xargs(char *program_name, char *argument1) {
  // TODO: free it
  char *argument2 = (char *)malloc(sizeof(char) * ARGUMENT_LEN);
  while (gets(argument2, ARGUMENT_LEN)) {
    // TODO: watch out! at the end of argument2, there is a '\n'!!!
    deleteNewLineSymbol(argument2);
    printf("1. argument2 before manipulation:%s\n", argument2);
    if (strcmp(argument2, "") == 0) {
      printf("since argument2 is empty,we gonna exit!\n");
      free(argument2);
      exit(0);
    }

    argument2 = concat(" ", argument2);
    printf("2. argument2 after manipulation:%s\n", argument2);
    printf("3. enter child process\n");
    char *argv[2];
    argv[0] = "echo";
    //argv[1] = concat(argument1, argument2);
    argv[1]="testtttt";
    printf("program_name:%s ", program_name);
    printf("argv[0]:%s ", argv[0]);
    printf("argv[1]:%s ", argv[1]);
    printf("now we gonna exec\n");
    if (fork() == 0) {
      exec("echo", argv);
    } else {
      wait(0);
      printf("parent wait finished\n");
    }
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