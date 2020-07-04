#include "9cc.h"

// Input source code
char *user_input;
bool debug_mode = false;

////////////////////////////////////
// Main Process
////////////////////////////////////

int main(int argc, char **argv) {
  if (argc > 3) {
    fprintf(stderr, "invalid number of arguments. \n");
    return 1;
  }
  user_input = argv[1];

  if (!strcmp(argv[2], "debug")) {
    fprintf(stderr, "WORKING ON DEBUG_MODE. \n");
    debug_mode = true;
  }

  debug("\nTARGET: %s", user_input);

  // generate tokens from a input string
  debug("\n::::::start_tokenize::::::");
  token = tokenize();
  debug("\n::::::end_tokenize::::::");

  // generate a syntax tree from tokens
  debug("\n::::::start_AST::::::");
  Node *node = program();
  debug("\n::::::end_AST::::::\n");

  // generate header codes
  printf(".intel_syntax noprefix\n");
  printf(".globl main\n");
  printf("main:\n");

  // prologue
  printf("  push rbp\n");
  printf("  mov rbp, rsp\n");
  printf("  sub rsp, 208\n");

  // generate codes form a syntax tree
  for (ListItem *current = code->head; current != NULL;) {
    debug("current:::%p", current);
    gen(current->node);

    current = current->next;
    debug("next:::%p", current);

    printf("  pop rax\n");
  }

  // epilogue
  printf("  mov rsp, rbp\n");
  printf("  pop rbp\n");
  printf("  ret\n");
  return 0;
}