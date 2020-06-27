#include "9cc.h"

////////////////////////////////////
// Tokenizer
////////////////////////////////////
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

        if (strchr("+-*/()<>;", *p))
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

        if ('a' <= *p || *p <= 'z')
        {
            cur = new_token(TK_IDENT, cur, p++, 1);
            continue;
        }

        error_at(p, "invalid token");
    }

    new_token(TK_EOF, cur, p, 0);
    return head.next;
}

////////////////////////////////////
// AST Generator
////////////////////////////////////

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

Node *new_lvar_node(char c)
{
    Node *node = new_node(ND_LVAR);
    node->offset = (c - 'a' + 1) * 8;
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
    if (token->kind != TK_RESERVED &&
        token->kind != TK_IDENT)
        return false;

    if (token->len != strlen(op) ||
        memcmp(token->str, op, token->len))
        return false;

    token = token->next;
    return true;
};

// confirm that a token is a identifier, and move a pointer forward
// return false if token is not reserved or not a specified operator
Token *consume_ident()
{
    if (token->kind != TK_IDENT)
        return false;

    Token *tok = token;

    token = token->next;
    return tok;
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

////////////////////////////////////////////////
// Syntax:
//      program     = stmt*
//      stmt        = expr ";"
//      expr        = assign
//      assign      = equality ("=" assign)?
//      qeuality    = relational ("==" ralational | "!=" relational)*
//      relational  = add ("<" add | "<=" add | ">" add | ">= add")*
//      add         = mul ("+" mul | "-" mul)*
//      mul         = unary ("*" unary | "/" unary)
//      unary       = ("*" | "-")? primary
//      primary     = num | ident | "(" expr ")"
////////////////////////////////////////////////
Node *code[100];

Node *program()
{
    int i = 0;
    while (!at_eof())
    {
        code[i++] = stmt();
    }
    code[i] = NULL;
}

Node *stmt()
{
    Node *node = expr();
    expect(';');
    return node;
}

Node *expr()
{
    return assign();
}

Node *assign()
{
    Node *node = equality();
    if (consume("="))
    {
        node = new_binary_node(ND_ASSIGN, node, assign());
    }
    return node;
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

    Token *tok = consume_ident();
    if (tok)
    {
        return new_lvar_node(tok->str[0]);
    }

    return new_number_node(expect_number());
}
