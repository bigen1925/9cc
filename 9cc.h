#include <ctype.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// Token Kind
typedef enum
{
    TK_RESERVED, // Keywords or punctuators
    TK_NUM,      // Integer literals
    TK_EOF,      // End-of-file markers
} TokenKind;

// Token Type
typedef struct Token Token;
struct Token
{
    TokenKind kind;
    Token *next; // next token
    int val;
    char *str;
    int len;
};

// Node Kind
typedef enum
{
    ND_ADD,
    ND_SUB,
    ND_MUL,
    ND_DIV,
    ND_EQ,
    ND_NE,
    ND_LT,
    ND_LTE,
    ND_NUM,
} NodeKind;

// Node Type
typedef struct Node Node;
struct Node
{
    NodeKind kind;
    Node *lhs;
    Node *rhs;
    int val;
};

extern char *user_input;
void error_at(char *loc, char *fmt, ...);
extern Token *token;

Token *tokenize();
void gen(Node *node);

Node *expr();
Node *equality();
Node *relational();
Node *add();
Node *mul();
Node *primary();
Node *unary();