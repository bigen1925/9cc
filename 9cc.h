#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Token Kind
typedef enum {
  TK_ADD,     // "+"
  TK_SUB,     // "-"
  TK_AST,     // "*"
  TK_DIV,     // "/"
  TK_AMP,     // "&"
  TK_EQ,      // "=="
  TK_NEQ,     // "!="
  TK_LT,      // "<"
  TK_LTE,     // "<="
  TK_GT,      // ">"
  TK_GTE,     // ">="
  TK_ASSIGN,  // "="
  TK_LPAR,    // "("
  TK_RPAR,    // ")"
  TK_LBRA,    // "{"
  TK_RBRA,    // "}"
  TK_COMMA,   // ","
  TK_PUNC,    // ";"
  TK_RETURN,  // "return"
  TK_IF,      // "if"
  TK_ELSE,    // "else"
  TK_WHILE,   // "while"
  TK_FOR,     // "for"
  TK_TYPE,    // "int"
  TK_SIZEOF,  // "sizeof"
  TK_IDENT,   // <identifier>
  TK_NUM,     // <integer literal>
  TK_EOF,     // End-of-file markers
} TokenKind;

// Token Type
typedef struct Token Token;
struct Token {
  TokenKind kind;
  Token *next;  // next token
  int val;
  char *str;
  int len;
};

// Local Varicables
typedef enum {
  INT,
  PTR,
} TypeKind;

typedef struct Type Type;
struct Type {
  TypeKind kind;
  Type *ptr_to;
};

typedef struct LVar LVar;
struct LVar {
  LVar *next;
  Type *type;
  char *name;
  int len;
  int offset;
};

extern LVar *lacals;

// Node Kind
typedef enum {
  ND_ADD,      // +
  ND_SUB,      // -
  ND_MUL,      // * (multiply)
  ND_DIV,      // /
  ND_DEREF,    // * (dereference)
  ND_ADDR,     // & (address)
  ND_EQ,       // ==
  ND_NE,       // !=
  ND_LT,       // <
  ND_LTE,      // <=
  ND_ASSIGN,   // =
  ND_BLOCK,    // {~}
  ND_IF,       // if
  ND_WHILE,    // while
  ND_FOR,      // for
  ND_SIZEOF,   // sizeof
  ND_LVAR,     // Local Variable
  ND_CALL,     // Function Call
  ND_NUM,      // Number
  ND_RETURN,   // return
  ND_PROGRAM,  // whole program
  ND_FUNC,     // function definitions
  ND_VAR_DEC,  // variable declaration
  ND_ARG,      // arguments
} NodeKind;

// Node Type
typedef struct Node Node;
typedef struct NodeLinkedList NodeLinkedList;
typedef struct NodeLinkedListItem NodeLinkedListItem;
struct Node {
  NodeKind kind;
  NodeLinkedList *children;
  Type *type;
  int num;
  char *str;
};
// Node Linked List
struct NodeLinkedList {
  NodeLinkedListItem *head;
  NodeLinkedListItem *tail;
};

struct NodeLinkedListItem {
  Node *node;
  NodeLinkedListItem *next;
};

void append(Node *node, NodeLinkedList *list);
Node *get_at(int n, NodeLinkedList *list);
void append_child(Node *child, Node *parent);
void append_child_head(Node *child, Node *parent);
Node *get_child_at(int n, Node *node);

extern char *user_input;
extern bool debug_mode;
void debug(char *fmt, ...);
void error(char *fmt, ...);
void error_at(char *loc, char *fmt, ...);
extern Token *token;

Token *tokenize();
void gen(Node *node);

Node *program();
Node *function();
Node *stmt();
Node *block();
Node *if_ast();
Node *while_ast();
Node *for_ast();
Node *var_dec();
Node *expr();
Node *assign();
Node *equality();
Node *relational();
Node *add();
Node *mul();
Node *primary();
Node *unary();
