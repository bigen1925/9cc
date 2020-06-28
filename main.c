#include "9cc.h"

// Input source code
char *user_input;
bool debug_mode = false;

////////////////////////////////////
// Utilities
////////////////////////////////////
// output debug log to stderr
void debug(char *fmt, ...)
{
    if (!debug_mode)
        return;

    va_list ap;
    va_start(ap, fmt);

    vfprintf(stderr, fmt, ap);
    fprintf(stderr, "\n");
}
// error output with a position
void error(char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);

    vfprintf(stderr, fmt, ap);
    fprintf(stderr, "\n");
    exit(1);
}

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
    if (argc > 3)
    {
        fprintf(stderr, "invalid number of arguments. \n");
        return 1;
    }
    user_input = argv[1];

    if (!strcmp(argv[2], "debug"))
    {
        fprintf(stderr, "WORKING ON DEBUG_MODE. \n");
        debug_mode = true;
    }

    debug("\nTARGET: %s", user_input);

    // generate tokens from a input string
    debug("::::::start_tokenize::::::\n");
    token = tokenize();
    debug("::::::end_tokenize::::::\n");

    // generate a syntax tree from tokens
    debug("::::::start_AST::::::\n");
    Node *node = program();
    debug("::::::end_AST::::::\n");

    // generate header codes
    printf(".intel_syntax noprefix\n");
    printf(".globl main\n");
    printf("main:\n");

    // prologue
    printf("  push rbp\n");
    printf("  mov rbp, rsp\n");
    printf("  sub rsp, 208\n");

    // generate codes form a syntax tree
    for (int i = 0; code[i]; i++)
    {
        gen(code[i]);

        printf("  pop rax\n");
    }

    // epilogue
    printf("  mov rsp, rbp\n");
    printf("  pop rbp\n");
    printf("  ret\n");
    return 0;
}