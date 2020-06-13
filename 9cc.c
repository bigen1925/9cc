#include <ctype.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// Kind of Token
typedef enum
{
    TK_RESERVED, // 予約文字
    TK_NUM,      // 数字
    TK_EOF,      // 入力の終わりを示すトークン
} TokenKind;

// Type of Token
typedef struct Token Token;
struct Token
{
    TokenKind kind;
    Token *next; // a pointer of next token
    int val;
    char *str;
};

// current token
Token *token;

// error output
void error(char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, "\n");
    exit(1);
}

bool consume(char op)
{
    if (token->kind != TK_RESERVED || token->str[0] != op)
        return false;

    token = token->next;
    return true;
};

bool expect(char op)
{
    if (token->kind != TK_RESERVED || token->str[0] != op)
        error("token is not '%c'", op);

    token = token->next;
}

int expect_number()
{
    if (token->kind != TK_NUM)
        error("token is not number.");

    int val = token->val;
    token = token->next;
    return val;
}

bool at_eof()
{
    return token->kind == TK_EOF;
}

Token *new_token(TokenKind kind, Token *cur, char *str)
{
    Token *new_token = calloc(1, sizeof(Token));
    new_token->kind = kind;
    new_token->str = str;
    cur->next = new_token;
    return new_token;
}

Token *tokenize(char *p)
{
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

        if (*p == '+' || *p == '-')
        {
            cur = new_token(TK_RESERVED, cur, p++);
            continue;
        };

        if (isdigit(*p))
        {
            cur = new_token(TK_NUM, cur, p);
            cur->val = strtol(p, &p, 10);
            continue;
        };

        error("Can not tokenize.");
    }

    new_token(TK_EOF, cur, p);
    return head.next;
}

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        fprintf(stderr, "invalid number of arguments. \n");
        return 1;
    }

    // execute tokenizing
    token = tokenize(argv[1]);

    printf(".intel_syntax noprefix\n");
    printf(".globl main\n");
    printf("main:\n");
    printf("    mov rax, %d\n", expect_number());

    while (!at_eof())
    {
        if (consume('+'))
        {
            printf("    add rax, %d\n", expect_number());
            continue;
        }

        expect('-');
        printf("    sub rax, %d\n", expect_number());
    }

    printf("  ret\n");
    return 0;
}