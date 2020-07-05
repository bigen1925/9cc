#include "9cc.h"

////////////////////////////////////
// Local Variables
////////////////////////////////////
LVar *locals;

LVar *find_lvar(Token *tok) {
  for (LVar *var = locals; var; var = var->next) {
    if (var->len && tok->len == var->len &&
        !memcmp(tok->str, var->name, var->len)) {
      return var;
    }
  }
  return NULL;
}

void init_locals() { locals = calloc(1, sizeof(LVar)); }

////////////////////////////////////
// AST Generator
////////////////////////////////////
int seq = 0;

Node *new_node(NodeKind kind) {
  Node *node = calloc(1, sizeof(Node));
  node->kind = kind;
  node->children = calloc(1, sizeof(NodeLinkedList));
  node->children->head = NULL;
  node->children->tail = NULL;
  return node;
}

Node *new_binary_node(NodeKind kind, Node *lhs, Node *rhs) {
  Node *node = new_node(kind);
  append_child(lhs, node);
  append_child(rhs, node);
  return node;
}

Node *new_if_node(Node *condition, Node *body, Node *_else, int seq) {
  Node *node = new_node(ND_IF);
  append_child(condition, node);
  append_child(body, node);
  append_child(_else, node);
  node->num = seq;

  return node;
}

Node *new_while_node(Node *condition, Node *body, int seq) {
  Node *node = new_node(ND_WHILE);
  append_child(condition, node);
  append_child(body, node);
  node->num = seq;

  return node;
}

Node *new_for_node(Node *initialization, Node *condition, Node *step,
                   Node *body, int seq) {
  Node *node = new_node(ND_FOR);
  append_child(initialization, node);
  append_child(condition, node);
  append_child(step, node);
  append_child(body, node);
  node->num = seq;

  return node;
}

Node *new_return_node(Node *body) {
  Node *node = new_node(ND_RETURN);
  append_child(body, node);
  return node;
}

Node *new_lvar_node(int offset) {
  Node *node = new_node(ND_LVAR);
  node->num = offset;
  return node;
}

Node *new_call_node(char *str, int len) {
  Node *node = new_node(ND_CALL);
  node->str = str;
  node->num = len;
}

Node *new_number_node(int val) {
  Node *node = new_node(ND_NUM);
  node->num = val;
  return node;
}

// confirm that a token is a soecified keyword, and move a pointer forward
// return false if token is not reserved or not a specified operator
bool consume(TokenKind kind) {
  if (token->kind != kind) return false;

  token = token->next;
  return true;
};

Token *consume_ident() {
  if (token->kind != TK_IDENT) return NULL;

  Token *tok = token;

  token = token->next;
  return tok;
};

// confirm that a token is a soecified operator, and move a pointer forward
// output error if token s not reserved or not a specified operator
void expect(TokenKind kind) {
  if (token->kind != kind) {
    char *s;
    strncpy(s, token->str, token->len);
    error_at(token->str, "expected '%s'", s);
  }

  token = token->next;
}

int expect_number() {
  if (token->kind != TK_NUM) error_at(token->str, "expected a number.");

  int val = token->val;
  token = token->next;
  debug("::expected_number:: %d", val);
  return val;
}

bool at_eof() { return token->kind == TK_EOF; }

////////////////////////////////////////////////
// Syntax:
//      program     = stmt* EOF
//      stmt        = (return)? expr ";"
//                  | "{" stmt* "}"
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
//      primary     = num
//                  | ident ("(" ")")?
//                  | "(" expr ")"
////////////////////////////////////////////////

Node *program() {
  debug("::::::start_program::::::");
  init_locals();
  Node *node = new_node(ND_BLOCK);

  while (!at_eof()) {
    append_child(stmt(), node);
  }

  debug("::::::end_program::::::");
  return node;
}

Node *stmt() {
  debug("::::::start_stmt::::::");
  Node *node;

  if (consume(TK_LBRA)) {
    debug("::stmt::block");
    node = new_node(ND_BLOCK);
    while (!consume(TK_RBRA)) {
      append_child(stmt(), node);
    }
  } else if (consume(TK_IF)) {
    debug("::stmt::if");
    expect(TK_LPAR);
    Node *condition = expr();
    expect(TK_RPAR);
    Node *body = stmt();
    Node *_else = NULL;
    if (consume(TK_ELSE)) {
      debug("::stmt::else");
      _else = stmt();
    }

    node = new_if_node(condition, body, _else, seq++);
  } else if (consume(TK_WHILE)) {
    debug("::stmt::while");
    expect(TK_LPAR);
    Node *condition = expr();
    expect(TK_RPAR);
    Node *body = stmt();

    node = new_while_node(condition, body, seq++);
  } else if (consume(TK_FOR)) {
    debug("::stmt::for");
    expect(TK_LPAR);
    Node *initialization = NULL;
    Node *condition = NULL;
    Node *step = NULL;
    if (!consume(TK_PUNC)) {
      initialization = expr();
      expect(TK_PUNC);
    }
    if (!consume(TK_PUNC)) {
      condition = expr();
      expect(TK_PUNC);
    }
    if (!consume(TK_RPAR)) {
      step = expr();
      expect(TK_RPAR);
    }
    Node *body = stmt();

    node = new_for_node(initialization, condition, step, body, seq++);
  } else {
    if (consume(TK_RETURN)) {
      debug("::stmt::return");
      node = new_return_node(expr());
    } else {
      node = expr();
    }
    expect(TK_PUNC);
  }

  debug("::end_stmt::");
  return node;
}

Node *expr() {
  debug("::::::start_expr::::::");
  Node *node = assign();

  debug("::::::end_expr::::::");
  return node;
}

Node *assign() {
  debug("::::::start_assign::::::");
  Node *node = equality();
  if (consume(TK_ASSIGN)) {
    node = new_binary_node(ND_ASSIGN, node, assign());
  }
  debug("::::::end_assign::::::");
  return node;
}

Node *equality() {
  debug("::::::start_equality::::::");
  Node *node = relational();

  for (;;) {
    if (consume(TK_EQ)) {
      node = new_binary_node(ND_EQ, node, relational());
    } else if (consume(TK_NEQ)) {
      node = new_binary_node(ND_NE, node, relational());
    } else {
      debug("::::::end_equality::::::");
      return node;
    }
  }
}

Node *relational() {
  debug("::::::start_relational::::::");
  Node *node = add();

  for (;;) {
    if (consume(TK_LT)) {
      node = new_binary_node(ND_LT, node, add());
    } else if (consume(TK_LTE)) {
      node = new_binary_node(ND_LTE, node, add());
    } else if (consume(TK_GT)) {
      node = new_binary_node(ND_LT, add(), node);
    } else if (consume(TK_GTE)) {
      node = new_binary_node(ND_LTE, add(), node);
    } else {
      debug("::::::end_relational::::::");
      return node;
    }
  }
}

Node *add() {
  debug("::::::start_add::::::");
  Node *node = mul();

  for (;;) {
    if (consume(TK_ADD)) {
      node = new_binary_node(ND_ADD, node, mul());
    } else if (consume(TK_SUB)) {
      node = new_binary_node(ND_SUB, node, mul());
    } else {
      debug("::::::end_add::::::");
      return node;
    }
  }
}

Node *mul() {
  debug("::::::start_mul::::::");
  Node *node = unary();
  for (;;) {
    if (consume(TK_MUL)) {
      node = new_binary_node(ND_MUL, node, unary());
    } else if (consume(TK_DIV)) {
      node = new_binary_node(ND_DIV, node, unary());
    } else {
      debug("::::::end_mul::::::");
      return node;
    }
  }
}

Node *unary() {
  debug("::::::start_unary::::::");
  if (consume(TK_ADD)) {
    return unary();
  } else if (consume(TK_SUB)) {
    return new_binary_node(ND_SUB, new_number_node(0), unary());
  }
  Node *node = primary();
  debug("::::::end_unary::::::");
  return node;
}

Node *primary() {
  debug("::::::start_primary::::::");
  if (consume(TK_LPAR)) {
    debug("::primary::expr");
    Node *node = expr();
    consume(TK_RPAR);

    debug("::::::end_primary::::::");
    return node;
  }

  Token *tok = consume_ident();
  if (tok != NULL) {
    if (consume(TK_LPAR)) {
      debug("::primary::call: %.*s", tok->len, tok->str);
      Node *node = new_call_node(tok->str, tok->len);

      if (!consume(TK_RPAR)) {
        append_child(expr(), node);

        while (!consume(TK_RPAR)) {
          expect(TK_COMMA);
          append_child(expr(), node);
        }
      }

      debug("::::::end_primary::::::");
      return node;
    }

    debug("::primary::ident: %.*s", tok->len, tok->str);
    LVar *var = find_lvar(tok);

    if (!var) {
      debug("::primary::not found");
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
