#include <stdio.h>
#include "user/user.h"

void xargs() {}

void get_input() {
  int c;
  while ((c=getchar())!=EOF) {
    
  }
}

int main(int argc, char *argv[]) {
  if (argc <= 1) {
    printf("Usage 1: xargs program_name\n");
    printf("Usage 2: xargs program_name program_arguments\n");
  }

  char *program_name = argv[1];
  if (argc == 3) {
    char *argument_partition = argv[2];
  }
}