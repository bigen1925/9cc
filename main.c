#include "9cc.h"

// Input source code
char *user_input;

////////////////////////////////////
// Utilities
////////////////////////////////////

// error output with a position
void error_at(char *loc, char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);

    int pos = loc - user_input;
    fprintf(stderr, "%s\n", user_input);
    fprintf(stderr, "%*s", pos, "");
    fprintf(stderr, "^ ");
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, "\n");
    exit(1);
}

////////////////////////////////////
// Main Process
////////////////////////////////////

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        fprintf(stderr, "invalid number of arguments. \n");
        return 1;
    }
    user_input = argv[1];

    // generate tokens from a input string
    token = tokenize();

    // generate a syntax tree from tokens
    Node *node = expr();

    printf(".intel_syntax noprefix\n");
    printf(".globl main\n");
    printf("main:\n");

    // generate codes form a syntax tree
    gen(node);

    printf("  pop rax\n");
    printf("  ret\n");
    return 0;
}