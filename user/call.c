#include "kernel/param.h"
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int g(int x) {
  return x+3;
}

int f(int x) {
  return g(x);
}

void main(void) {
  printf("%d %d\n", f(8)+1, 13);
  exit(0);
}

/**
  1. a0 and a7 contain arguments to functions,
  in this function, a2 store 13
  
  2. idk

  3. 1536

  4. 0x38

  5. He110 World

  6. random number, it is the value in the block 
  that right after variable 3 located in.
 */