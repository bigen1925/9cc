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

  debug("\n::::::start_code_generation::::::");
  gen(node);
  debug("\n::::::end_code_generation::::::");

  return 0;
}