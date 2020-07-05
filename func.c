#include "9cc.h"

int foo() { printf("OK\n"); }
int bar(int x, int y) {
  int z = x + y;
  printf("bar() called. -> %d\n", z);
  return z;
}