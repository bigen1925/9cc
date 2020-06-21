#include <ctype.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

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
// Tokenizer
////////////////////////////////////

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

// current token
Token *token;

bool startswith(char *p, char *q)
{
    return memcmp(p, q, strlen(q)) == 0;
}

Token *new_token(TokenKind kind, Token *cur, char *str, int len)
{
    Token *new_token = calloc(1, sizeof(Token));
    new_token->kind = kind;
    new_token->str = str;
    cur->next = new_token;
    new_token->len = len;
    return new_token;
}

Token *tokenize()
{
    char *p = user_input;

    Token head;
    head.next = NULL;
    Token *cur = &head;

    while (*p)
    {
        if (isspace(*p))
        {
            p++;
            continue;
        };

        if (startswith(p, "==") || startswith(p, "!=") || startswith(p, "<=") || startswith(p, ">="))
        {
            cur = new_token(TK_RESERVED, cur, p, 2);
            p += 2;
            continue;
        }

        if (strchr("+-*/()<>", *p))
        {
            cur = new_token(TK_RESERVED, cur, p++, 1);
            continue;
        };

        if (isdigit(*p))
        {
            cur = new_token(TK_NUM, cur, p, 0);
            cur->val = strtol(p, &p, 10);
            continue;
        };

        error_at(p, "invalid token");
    }

    new_token(TK_EOF, cur, p, 0);
    return head.next;
}

////////////////////////////////////
// AST Generator
////////////////////////////////////

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

Node *new_node(NodeKind kind)
{
    Node *node = calloc(1, sizeof(Node));
    node->kind = kind;
    return node;
}

Node *new_binary_node(NodeKind kind, Node *lhs, Node *rhs)
{
    Node *node = new_node(kind);
    node->lhs = lhs;
    node->rhs = rhs;
    return node;
}

Node *new_number_node(int val)
{
    Node *node = new_node(ND_NUM);
    node->val = val;
    return node;
}

// confirm that a token is a soecified operator, and move a pointer forward
// return false if token is not reserved or not a specified operator
bool consume(char *op)
{
    if (token->kind != TK_RESERVED ||
        token->len != strlen(op) ||
        memcmp(token->str, op, token->len))
        return false;

    token = token->next;
    return true;
};

// confirm that a token is a soecified operator, and move a pointer forward
// output error if token s not reserved or not a specified operator
bool expect(char op)
{
    if (token->kind != TK_RESERVED || token->str[0] != op)
        error_at(token->str, "expected '%c'", op);

    token = token->next;
}

int expect_number()
{
    if (token->kind != TK_NUM)
        error_at(token->str, "expected a number.");

    int val = token->val;
    token = token->next;
    return val;
}

bool at_eof()
{
    return token->kind == TK_EOF;
}

Node *expr();
Node *equality();
Node *relational();
Node *add();
Node *mul();
Node *primary();
Node *unary();

Node *expr()
{
    return equality();
}

Node *equality()
{
    Node *node = relational();

    for (;;)
    {
        if (consume("=="))
        {
            node = new_binary_node(ND_EQ, node, relational());
        }
        else if (consume("!="))
        {
            node = new_binary_node(ND_NE, node, relational());
        }
        else
        {
            return node;
        }
    }
}

Node *relational()
{
    Node *node = add();

    for (;;)
    {
        if (consume("<"))
        {
            node = new_binary_node(ND_LT, node, add());
        }
        else if (consume("<="))
        {
            node = new_binary_node(ND_LTE, node, add());
        }
        else if (consume(">"))
        {
            node = new_binary_node(ND_LT, add(), node);
        }
        else if (consume(">="))
        {
            node = new_binary_node(ND_LTE, add(), node);
        }
        else
        {
            return node;
        }
    }
}

Node *add()
{
    Node *node = mul();

    for (;;)
    {
        if (consume("+"))
        {
            node = new_binary_node(ND_ADD, node, mul());
        }
        else if (consume("-"))
        {
            node = new_binary_node(ND_SUB, node, mul());
        }
        else
        {
            return node;
        }
    }
}

Node *mul()
{
    Node *node = unary();
    for (;;)
    {
        if (consume("*"))
        {
            node = new_binary_node(ND_MUL, node, unary());
        }
        else if (consume("/"))
        {
            node = new_binary_node(ND_DIV, node, unary());
        }
        else
        {
            return node;
        }
    }
}

Node *unary()
{
    if (consume("+"))
    {
        return unary();
    }
    else if (consume("-"))
    {
        return new_binary_node(ND_SUB, new_number_node(0), unary());
    }
    return primary();
}

Node *primary()
{
    if (consume("("))
    {
        Node *node = expr();
        consume(")");
        return node;
    }

    return new_number_node(expect_number());
}

////////////////////////////////////
// Syntax Tree Compiler
////////////////////////////////////

void gen(Node *node)
{
    if (node->kind == ND_NUM)
    {
        printf("  push %d \n", node->val);
        return;
    }

    gen(node->lhs);
    gen(node->rhs);

    printf("  pop rdi\n");
    printf("  pop rax\n");

    switch (node->kind)
    {
    case ND_ADD:
        printf("  add rax, rdi\n");
        break;
    case ND_SUB:
        printf("  sub rax, rdi\n");
        break;
    case ND_MUL:
        printf("  imul rax, rdi\n");
        break;
    case ND_DIV:
        printf("  cqo\n");
        printf("  idiv rax, rdi\n");
        break;
    case ND_EQ:
        printf("  cmp rax, rdi\n");
        printf("  sete al\n");
        printf("  movzb rax, al\n");
        break;
    case ND_NE:
        printf("  cmp rax, rdi\n");
        printf("  setne al\n");
        printf("  movzb rax, al\n");
        break;
    case ND_LT:
        printf("  cmp rax, rdi\n");
        printf("  setl al\n");
        printf("  movzb rax, al\n");
        break;
    case ND_LTE:
        printf("  cmp rax, rdi\n");
        printf("  setle al\n");
        printf("  movzb rax, al\n");
        break;
    }

    printf("  push rax\n");
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