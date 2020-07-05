#include "9cc.h"

void no_arg() { printf("OK\n"); }
int int_arg(int x, int y) { printf("my_func called. -> %d\n", x + y); }