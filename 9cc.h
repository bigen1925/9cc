#include <ctype.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// Token Kind
typedef enum
{
    TK_ADD,    // "+"
    TK_SUB,    // "-"
    TK_MUL,    // "*"
    TK_DIV,    // "/"
    TK_EQ,     // "=="
    TK_NEQ,    // "!="
    TK_LT,     // "<"
    TK_LTE,    // "<="
    TK_GT,     // ">"
    TK_GTE,    // ">="
    TK_ASSIGN, // "="
    TK_LPAR,   // "("
    TK_RPAR,   // ")"
    TK_PUNC,   // ";"
    TK_RETURN, // "return"
    TK_IF,     // "if"
    TK_ELSE,   // "else"
    TK_WHILE,  // "while"
    TK_FOR,    // "for"
    TK_IDENT,  // <identifier>
    TK_NUM,    // <integer literal>
    TK_EOF,    // End-of-file markers
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
    ND_ADD,    // +
    ND_SUB,    // -
    ND_MUL,    // *
    ND_DIV,    // /
    ND_EQ,     // ==
    ND_NE,     // !=
    ND_LT,     // <
    ND_LTE,    // <=
    ND_ASSIGN, // =
    ND_IF,     // if
    ND_LVAR,   // Local Variable
    ND_NUM,    // Number
    ND_RETURN, // return
} NodeKind;

// Node Type
typedef struct Node Node;
struct Node
{
    NodeKind kind;
    Node *first;
    Node *second;
    Node *third;
    Node *fourth;
    int val;
    int offset;
};

extern char *user_input;
void debug(char *fmt, ...);
void error(char *fmt, ...);
void error_at(char *loc, char *fmt, ...);
extern Token *token;

Token *tokenize();
void gen(Node *node);

extern Node *code[100];
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
struct LVar
{
    LVar *next;
    char *name;
    int len;
    int offset;
};

extern LVar *lacals;