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

LVar *create_lvar(Type *type, Token *tok) {
  LVar *var = calloc(1, sizeof(LVar));
  var->next = locals;
  var->type = type;
  var->len = tok->len;
  var->name = tok->str;
  var->offset = locals->offset + get_size_of_type(type);

  locals = var;

  return var;
}

LVar *create_lvar_or_fail(Type *type, Token *tok) {
  LVar *var = find_lvar(tok);

  if (var) {
    error("redeclaration of a variable '%.*s' .", tok->len, tok->str);
  }

  return create_lvar(type, tok);
}

LVar *get_lvar_or_fail(Token *tok) {
  LVar *var = find_lvar(tok);

  if (!var) {
    error("a variable '%.*s' is not declared.", tok->len, tok->str);
  }

  return var;
}

void init_locals() { locals = calloc(1, sizeof(LVar)); }

Type *new_type(TypeKind kind, Type *ptr_to) {
  Type *type = calloc(1, sizeof(Type));
  type->kind = kind;
  type->ptr_to = ptr_to;

  return type;
}

int get_size_of_type(Type *type) {
  switch (type->kind) {
    case INT:
      return 4;

    case PTR:
      return 8;

    case ARRAY:
      return get_size_of_type(type->ptr_to) * type->array_size;
  }
}

Type *new_array_type(Type *ptr_to, int size) {
  Type *type = new_type(ARRAY, ptr_to);
  type->array_size = size;

  return type;
}

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

Node *new_unary_node(NodeKind kind, Node *operand) {
  Node *node = new_node(kind);
  append_child(operand, node);
  return node;
}

Node *new_addr_node(Node *operand) {
  Node *node = new_unary_node(ND_ADDR, operand);

  node->type = new_type(PTR, operand->type);

  return node;
}

Node *new_sizeof_node(Node *operand) {
  Node *node = new_unary_node(ND_SIZEOF, operand);
  node->type = new_type(INT, NULL);
  ;
  return node;
}

Node *new_deref_node(Node *operand) {
  Node *node = new_unary_node(ND_DEREF, operand);
  node->type =
      new_type(operand->type->ptr_to->kind, operand->type->ptr_to->ptr_to);

  return node;
}

Node *new_binary_node(NodeKind kind, Node *lhs, Node *rhs) {
  Node *node = new_node(kind);
  append_child(lhs, node);
  append_child(rhs, node);
  return node;
}

Node *new_add_sub_node(NodeKind kind, Node *lhs, Node *rhs) {
  Node *node = new_binary_node(kind, lhs, rhs);
  if (lhs->type->kind == INT && rhs->type->kind == INT) {
    node->type = new_type(INT, NULL);
  } else if (lhs->type->kind == PTR && rhs->type->kind == INT) {
    node->type = lhs->type;
  } else if (lhs->type->kind == INT && rhs->type->kind == PTR) {
    node->type = rhs->type;
  } else if (lhs->type->kind == ARRAY && rhs->type->kind == INT) {
    node->type = new_type(PTR, lhs->type->ptr_to);
  } else if (lhs->type->kind == INT && rhs->type->kind == ARRAY) {
    node->type = new_type(PTR, rhs->type->ptr_to);
  } else {
    error("unexpected types of addition, %d and %d.", lhs->type->kind,
          rhs->type->kind);
  }
  return node;
}

Node *new_mul_div_node(NodeKind kind, Node *lhs, Node *rhs) {
  Node *node = new_binary_node(kind, lhs, rhs);
  if (lhs->type->kind == INT && rhs->type->kind == INT) {
    node->type = new_type(INT, NULL);
  } else {
    error("unexpected types of multiply, %d and %d.", lhs->type->kind,
          rhs->type->kind);
  }
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

Node *new_function_node(char *str, int len) {
  Node *node = new_node(ND_FUNC);
  node->str = str;
  node->num = len;
  return node;
}

Node *new_arg_node(Type *type, Token *tok) {
  LVar *var = create_lvar_or_fail(type, tok);
  Node *node = new_node(ND_ARG);
  node->num = var->offset;
  node->type = var->type;
  return node;
}

Node *new_lvar_node(Token *tok) {
  LVar *var = get_lvar_or_fail(tok);
  Node *node = new_node(ND_LVAR);

  node->type = var->type;

  node->num = var->offset;
  return node;
}

Node *new_call_node(char *str, int len) {
  Node *node = new_node(ND_CALL);
  node->type = new_type(INT, NULL);
  node->str = str;
  node->num = len;
}

Node *new_number_node(int val) {
  Node *node = new_node(ND_NUM);
  node->type = new_type(INT, NULL);
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
  return val;
}

Token *expect_ident() {
  if (token->kind != TK_IDENT) error_at(token->str, "expected an ident.");

  Token *tok = token;
  token = token->next;
  return tok;
}

Token *expect_type() {
  if (token->kind != TK_TYPE) error_at(token->str, "expected an type->");

  Token *tok = token;
  token = token->next;
  return tok;
}

// check a kind of next token
bool lookahead(TokenKind kind) { return token->kind == kind; }

bool at_eof() { return token->kind == TK_EOF; }

////////////////////////////////////////////////
// Syntax:
//      program     = function* EOF
//      function    = ident "(" (expr ("," expr)*)? ")" block
//      stmt        = block
//                  | if
//                  | while
//                  | for
//                  | var_dec
//                  | (return)? expr ";"
//      block       = "{" stmt* "}"
//      if          = "if" "(" expr ")" stmt ("else" stmt)?
//      while       = "while" "(" expr ")" stmt
//      for         = "for" "(" expr? ";" expr? ";" expr? ";" ")" stmt
//      var_dec     = "int" ("*"* ident | ident ("[" num "]")*) ";"
//      expr        = assign
//      assign      = equality ("=" assign)?
//      qeuality    = relational ("==" ralational | "!=" relational)*
//      relational  = add ("<" add | "<=" add | ">" add | ">= add")*
//      add         = mul ("+" mul | "-" mul)*
//      mul         = unary ("*" unary | "/" unary)
//      unary       = ("+" | "-")? primary
//                  | ("*" | "&") unary
//                  | "sizeof" unary
//      primary     = num
//                  | ident ("(" (expr ("," expr)*)? ")")?
//                  | primary "[" primary "]"
//                  | "(" expr ")"
////////////////////////////////////////////////

Node *program() {
  debug("::::::start program::::::");
  init_locals();
  Node *node = new_node(ND_PROGRAM);

  while (!at_eof()) {
    append_child(function(), node);
  }

  debug("::::::end program::::::");
  return node;
}

Node *function() {
  debug("::::::start function::::::");
  expect_type();
  Token *tok = expect_ident();
  expect(TK_LPAR);

  Node *node = new_function_node(tok->str, tok->len);

  if (!consume(TK_RPAR)) {
    expect_type();
    tok = expect_ident();
    Type *type = new_type(INT, NULL);
    append_child(new_arg_node(type, tok), node);

    while (consume(TK_COMMA)) {
      expect_type();
      tok = expect_ident();
      Type *type = new_type(INT, NULL);
      append_child(new_arg_node(type, tok), node);
    }

    expect(TK_RPAR);
  }

  append_child_head(block(), node);

  debug("::::::end function::::::");
  return node;
}

Node *stmt() {
  debug("::::::start stmt::::::");
  Node *node;

  if (lookahead(TK_LBRC)) {
    node = block();
  } else if (lookahead(TK_IF)) {
    node = if_ast();
  } else if (lookahead(TK_WHILE)) {
    node = while_ast();
  } else if (lookahead(TK_FOR)) {
    node = for_ast();
  } else if (lookahead(TK_TYPE)) {
    node = var_dec();
  } else {
    if (consume(TK_RETURN)) {
      debug("::stmt::return");
      node = new_return_node(expr());
    } else {
      debug("::stmt::expr");
      node = expr();
    }
    expect(TK_PUNC);
  }

  debug("::::::end stmt::::::");
  return node;
}

Node *block() {
  debug("::::::start block::::::");

  expect(TK_LBRC);
  Node *node = new_node(ND_BLOCK);
  while (!consume(TK_RBRC)) {
    append_child(stmt(), node);
  }

  debug("::::::end block::::::");
  return node;
}

Node *if_ast() {
  debug("::::::start if::::::");

  expect(TK_IF);
  expect(TK_LPAR);
  Node *condition = expr();
  expect(TK_RPAR);
  Node *body = stmt();
  Node *_else = NULL;
  if (consume(TK_ELSE)) {
    debug("::stmt::else");
    _else = stmt();
  }

  debug("::::::end if::::::");
  return new_if_node(condition, body, _else, seq++);
}

Node *while_ast() {
  debug("::::::start while::::::");

  expect(TK_WHILE);
  expect(TK_LPAR);
  Node *condition = expr();
  expect(TK_RPAR);
  Node *body = stmt();

  debug("::::::end while::::::");
  return new_while_node(condition, body, seq++);
}

Node *for_ast() {
  debug("::::::start for::::::");

  expect(TK_FOR);
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

  debug("::::::end for::::::");
  return new_for_node(initialization, condition, step, body, seq++);
}

Node *var_dec() {
  debug("::::::start var_dec::::::");

  Token *tok = expect_type();
  if (!strncmp(tok->str, "int", 3)) {
    Type *type = new_type(INT, NULL);
    while (consume(TK_AST)) {
      type = new_type(PTR, type);
    }

    tok = expect_ident();

    while (consume(TK_LBRK)) {
      type = new_array_type(type, expect_number());
      expect(TK_RBRK);
    }

    LVar *var = create_lvar_or_fail(type, tok);
  }
  expect(TK_PUNC);

  debug("::::::end var_dec::::::");
  return new_node(ND_VAR_DEC);
}

Node *expr() {
  debug("::::::start expr::::::");

  Node *node = assign();

  debug("::::::end expr::::::");
  return node;
}

Node *assign() {
  debug("::::::start assign::::::");

  Node *node = equality();
  if (consume(TK_ASSIGN)) {
    node = new_binary_node(ND_ASSIGN, node, assign());
  }

  debug("::::::end assign::::::");
  return node;
}

Node *equality() {
  debug("::::::start equality::::::");

  Node *node = relational();

  for (;;) {
    if (consume(TK_EQ)) {
      node = new_binary_node(ND_EQ, node, relational());
    } else if (consume(TK_NEQ)) {
      node = new_binary_node(ND_NE, node, relational());
    } else {
      debug("::::::end equality::::::");
      return node;
    }
  }
}

Node *relational() {
  debug("::::::start relational::::::");
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
      debug("::::::end relational::::::");
      return node;
    }
  }
}

Node *add() {
  debug("::::::start add::::::");
  Node *node = mul();

  for (;;) {
    if (consume(TK_ADD)) {
      node = new_add_sub_node(ND_ADD, node, mul());
    } else if (consume(TK_SUB)) {
      node = new_add_sub_node(ND_SUB, node, mul());
    } else {
      debug("::::::end add::::::");
      return node;
    }
  }
}

Node *mul() {
  debug("::::::start mul::::::");
  Node *node = unary();
  for (;;) {
    if (consume(TK_AST)) {
      node = new_mul_div_node(ND_MUL, node, unary());
    } else if (consume(TK_DIV)) {
      node = new_mul_div_node(ND_DIV, node, unary());
    } else {
      debug("::::::end mul::::::");
      return node;
    }
  }
}

Node *unary() {
  debug("::::::start unary::::::");

  if (consume(TK_ADD)) {
    return unary();
  } else if (consume(TK_SUB)) {
    return new_add_sub_node(ND_SUB, new_number_node(0), unary());
  } else if (consume(TK_AST)) {
    return new_deref_node(unary());
  } else if (consume(TK_AMP)) {
    return new_addr_node(unary());
  } else if (consume(TK_SIZEOF)) {
    return new_sizeof_node(unary());
  }

  Node *node = primary();
  debug("::::::end unary::::::");
  return node;
}

Node *primary() {
  debug("::::::start primary::::::");
  Node *node;

  if (consume(TK_LPAR)) {
    debug("::primary::expr");
    node = expr();
    consume(TK_RPAR);
  }

  else {
    Token *tok = consume_ident();
    if (tok) {
      if (consume(TK_LPAR)) {
        // function call
        debug("::primary::call: %.*s", tok->len, tok->str);
        node = new_call_node(tok->str, tok->len);

        if (!consume(TK_RPAR)) {
          append_child(expr(), node);

          while (!consume(TK_RPAR)) {
            expect(TK_COMMA);
            append_child(expr(), node);
          }
        }
      }

      else {
        // local variable
        debug("::primary::local_variable: %.*s", tok->len, tok->str);
        node = new_lvar_node(tok);
      }
    }

    else {
      debug("::primary::number");
      node = new_number_node(expect_number());
    }
  }

  if (consume(TK_LBRK)) {
    node = new_deref_node(new_add_sub_node(ND_ADD, node, primary()));
    expect(TK_RBRK);
  }

  debug("::::::end primary::::::");
  return node;
}
