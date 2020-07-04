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
  TK_MUL,     // "*"
  TK_DIV,     // "/"
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
  TK_PUNC,    // ";"
  TK_RETURN,  // "return"
  TK_IF,      // "if"
  TK_ELSE,    // "else"
  TK_WHILE,   // "while"
  TK_FOR,     // "for"
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

// Node Kind
typedef enum {
  ND_ADD,     // +
  ND_SUB,     // -
  ND_MUL,     // *
  ND_DIV,     // /
  ND_EQ,      // ==
  ND_NE,      // !=
  ND_LT,      // <
  ND_LTE,     // <=
  ND_ASSIGN,  // =
  ND_BLOCK,   // {~}
  ND_IF,      // if
  ND_WHILE,   // while
  ND_FOR,     // for
  ND_LVAR,    // Local Variable
  ND_NUM,     // Number
  ND_RETURN,  // return
} NodeKind;

// Node Type
typedef struct Node Node;
typedef struct NodeLinkedList NodeLinkedList;
typedef struct NodeLinkedListItem NodeLinkedListItem;
struct Node {
  NodeKind kind;
  NodeLinkedList *children;
  int val;
  int offset;
  int seq;
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
Node *stmt();
Node *assign();
Node *expr();
Node *equality();
Node *relational();
Node *add();
Node *mul();
Node *primary();
Node *unary();

// Local Varicables
typedef struct LVar LVar;
struct LVar {
  LVar *next;
  char *name;
  int len;
  int offset;
};

extern LVar *lacals;