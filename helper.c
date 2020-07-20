#include <stdio.h>
#include <stdlib.h>

void alloc4(int **p, int a, int b, int c, int d) {
  *p = (int *)calloc(4, 4);
  (*p)[0] = a;
  (*p)[1] = b;
  (*p)[2] = c;
  (*p)[3] = d;
}

void temp_debug(int *p, int *q) {
  fprintf(stderr, "p: %p -> %d, p[1]: %p -> %d\n", p, *p, p + 1, *(p + 1));
  fprintf(stderr, "p[2]: %p -> %d, p[3]: %p -> %d\n", p + 2, *(p + 2), p + 3,
          *(p + 3));

  fprintf(stderr, "q: %p ", q);
  fprintf(stderr, "-> %d\n", *q);
}

void print_int(int n) { fprintf(stderr, "print_int:: n -> %d\n", n); }
void print_int_2(int n, int m) {
  fprintf(stderr, "print_int_2:: n -> %d, m -> %d\n", n, m);
}