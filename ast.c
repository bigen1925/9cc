#include "9cc.h"

////////////////////////////////////
// Local Variables
////////////////////////////////////
LVar *locals;

LVar *find_lvar(Token *tok)
{
    for (LVar *var = locals; var; var = var->next)
    {
        if (var->len && tok->len == var->len && !memcmp(tok->str, var->name, var->len))
        {
            return var;
        }
    }
    return NULL;
}

////////////////////////////////////
// AST Generator
////////////////////////////////////
int if_stmt_seq = 0;

Node *new_node(NodeKind kind)
{
    Node *node = calloc(1, sizeof(Node));
    node->kind = kind;
    return node;
}

Node *new_binary_node(NodeKind kind, Node *lhs, Node *rhs)
{
    Node *node = new_node(kind);
    node->first = lhs;
    node->second = rhs;
    return node;
}

Node *new_if_node(Node *condition, Node *body, Node *_else, int seq)
{
    Node *node = new_node(ND_IF);
    node->first = condition;
    node->second = body;
    node->third = _else;
    node->seq = seq;

    return node;
}

Node *new_return_node(Node *node)
{
    Node *nd = new_node(ND_RETURN);
    nd->first = node;
    return nd;
}

Node *new_lvar_node(int offset)
{
    Node *node = new_node(ND_LVAR);
    node->offset = offset;
    return node;
}

Node *new_number_node(int val)
{
    Node *node = new_node(ND_NUM);
    node->val = val;
    return node;
}

// confirm that a token is a soecified keyword, and move a pointer forward
// return false if token is not reserved or not a specified operator
bool consume(TokenKind kind)
{
    if (token->kind != kind)
        return false;

    token = token->next;
    return true;
};

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
void expect(TokenKind kind)
{
    if (token->kind != kind)
    {
        char *s;
        strncpy(s, token->str, token->len);
        error_at(token->str, "expected '%s'", s);
    }

    token = token->next;
}

int expect_number()
{
    if (token->kind != TK_NUM)
        error_at(token->str, "expected a number.");

    int val = token->val;
    token = token->next;
    debug("::expected_number:: %d", val);
    return val;
}

bool at_eof()
{
    return token->kind == TK_EOF;
}

////////////////////////////////////////////////
// Syntax:
//      program     = stmt*
//      stmt        = (return)? expr ";"
//                  | "if" "(" expr ")" stmt ("else" stmt)?
//                  | "while" "(" expr ")" stmt
//                  | "for" "(" expr? ";" expr? ";" expr? ";" ")" stmt
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
    debug("::::::start_program::::::");
    locals = calloc(1, sizeof(LVar));

    int i = 0;
    while (!at_eof())
    {
        code[i++] = stmt();
    }
    code[i] = NULL;
    debug("::::::end_program::::::");
}

Node *stmt()
{
    debug("::::::start_stmt::::::");
    Node *node;

    if (consume(TK_IF))
    {
        debug("::stmt::if");
        expect(TK_LPAR);
        Node *condition = expr();
        expect(TK_RPAR);
        Node *body = stmt();
        Node *_else = NULL;
        if (consume(TK_ELSE))
        {
            debug("::stmt::else");
            _else = stmt();
        }

        node = new_if_node(condition, body, _else, if_stmt_seq++);
    }
    else
    {
        if (consume(TK_RETURN))
        {
            debug("::stmt::return");
            node = new_return_node(expr());
        }
        else
        {
            node = expr();
        }
        expect(TK_PUNC);
    }

    debug("::::::end_stmt::::::");
    return node;
}

Node *expr()
{
    debug("::::::start_expr::::::");
    Node *node = assign();

    debug("::::::end_expr::::::");
    return node;
}

Node *assign()
{
    debug("::::::start_assign::::::");
    Node *node = equality();
    if (consume(TK_ASSIGN))
    {
        node = new_binary_node(ND_ASSIGN, node, assign());
    }
    debug("::::::end_assign::::::");
    return node;
}

Node *equality()
{
    debug("::::::start_equality::::::");
    Node *node = relational();

    for (;;)
    {
        if (consume(TK_EQ))
        {
            node = new_binary_node(ND_EQ, node, relational());
        }
        else if (consume(TK_NEQ))
        {
            node = new_binary_node(ND_NE, node, relational());
        }
        else
        {
            debug("::::::end_equality::::::");
            return node;
        }
    }
}

Node *relational()
{
    debug("::::::start_relational::::::");
    Node *node = add();

    for (;;)
    {
        if (consume(TK_LT))
        {
            node = new_binary_node(ND_LT, node, add());
        }
        else if (consume(TK_LTE))
        {
            node = new_binary_node(ND_LTE, node, add());
        }
        else if (consume(TK_GT))
        {
            node = new_binary_node(ND_LT, add(), node);
        }
        else if (consume(TK_GTE))
        {
            node = new_binary_node(ND_LTE, add(), node);
        }
        else
        {
            debug("::::::end_relational::::::");
            return node;
        }
    }
}

Node *add()
{
    debug("::::::start_add::::::");
    Node *node = mul();

    for (;;)
    {
        if (consume(TK_ADD))
        {
            node = new_binary_node(ND_ADD, node, mul());
        }
        else if (consume(TK_SUB))
        {
            node = new_binary_node(ND_SUB, node, mul());
        }
        else
        {
            debug("::::::end_add::::::");
            return node;
        }
    }
}

Node *mul()
{
    debug("::::::start_mul::::::");
    Node *node = unary();
    for (;;)
    {
        if (consume(TK_MUL))
        {
            node = new_binary_node(ND_MUL, node, unary());
        }
        else if (consume(TK_DIV))
        {
            node = new_binary_node(ND_DIV, node, unary());
        }
        else
        {
            debug("::::::end_mul::::::");
            return node;
        }
    }
}

Node *unary()
{
    debug("::::::start_unary::::::");
    if (consume(TK_ADD))
    {
        return unary();
    }
    else if (consume(TK_SUB))
    {
        return new_binary_node(ND_SUB, new_number_node(0), unary());
    }
    Node *node = primary();
    debug("::::::end_unary::::::");
    return node;
}

Node *primary()
{
    debug("::::::start_primary::::::");
    if (consume(TK_LPAR))
    {
        debug("::primary::expr");
        Node *node = expr();
        consume(TK_RPAR);

        debug("::::::end_primary::::::");
        return node;
    }

    Token *tok = consume_ident();
    if (tok)
    {
        debug("::primary::ident");
        LVar *var = find_lvar(tok);

        if (!var)
        {
            var = calloc(1, sizeof(LVar));
            var->next = locals;
            var->len = tok->len;
            var->name = tok->str;
            var->offset = locals->offset + 8;
            locals = var;
        }
        Node *node = new_lvar_node(var->offset);

        debug("::::::end_primary::::::");
        return node;
    }

    debug("::primary::number");
    Node *node = new_number_node(expect_number());

    debug("::::::end_primary::::::");
    return node;
}