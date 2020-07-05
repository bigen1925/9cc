#include "9cc.h"

////////////////////////////////////
// Tokenizer
////////////////////////////////////
// current token
Token *token;

bool startswith(char *p, char *q) { return memcmp(p, q, strlen(q)) == 0; }

bool is_alnum(char c) {
  return ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z') ||
         ('0' <= c && c <= '9') || (c == '_');
}

Token *new_token(TokenKind kind, Token *cur, char *str, int len) {
  Token *new_token = calloc(1, sizeof(Token));
  new_token->kind = kind;
  new_token->str = str;
  cur->next = new_token;
  new_token->len = len;
  return new_token;
}

Token *new_ident_token(Token *cur, char *str) {
  Token *new_token = calloc(1, sizeof(Token));
  new_token->kind = TK_IDENT;
  new_token->str = str;
  new_token->len = 1;
  str++;
  while (is_alnum(*str)) {
    new_token->len++;
    str++;
  }

  cur->next = new_token;
  return new_token;
}

Token *tokenize() {
  char *p = user_input;

  Token head;
  head.next = NULL;
  Token *cur = &head;

  while (*p) {
    if (isspace(*p)) {
      p++;
      continue;
    };

    if (startswith(p, "+")) {
      cur = new_token(TK_ADD, cur, p++, 1);
      continue;
    }
    if (startswith(p, "-")) {
      cur = new_token(TK_SUB, cur, p++, 1);
      continue;
    }
    if (startswith(p, "*")) {
      cur = new_token(TK_MUL, cur, p++, 1);
      continue;
    }
    if (startswith(p, "/")) {
      cur = new_token(TK_DIV, cur, p++, 1);
      continue;
    }
    if (startswith(p, "==")) {
      cur = new_token(TK_EQ, cur, p, 2);
      p += 2;
      continue;
    }
    if (startswith(p, "!=")) {
      cur = new_token(TK_NEQ, cur, p, 2);
      p += 2;
      continue;
    }
    if (startswith(p, "<=")) {
      cur = new_token(TK_LTE, cur, p, 2);
      p += 2;
      continue;
    }
    if (startswith(p, "<")) {
      cur = new_token(TK_LT, cur, p++, 1);
      continue;
    }
    if (startswith(p, ">=")) {
      cur = new_token(TK_GTE, cur, p, 2);
      p += 2;
      continue;
    }
    if (startswith(p, ">")) {
      cur = new_token(TK_GT, cur, p++, 1);
      continue;
    }
    if (startswith(p, "=")) {
      cur = new_token(TK_ASSIGN, cur, p++, 1);
      continue;
    }
    if (startswith(p, "(")) {
      cur = new_token(TK_LPAR, cur, p++, 1);
      continue;
    }
    if (startswith(p, ")")) {
      cur = new_token(TK_RPAR, cur, p++, 1);
      continue;
    }
    if (startswith(p, ",")) {
      cur = new_token(TK_COMMA, cur, p++, 1);
      continue;
    }
    if (startswith(p, ";")) {
      cur = new_token(TK_PUNC, cur, p++, 1);
      continue;
    }
    if (startswith(p, "{")) {
      cur = new_token(TK_LBRA, cur, p++, 1);
      continue;
    }
    if (startswith(p, "}")) {
      cur = new_token(TK_RBRA, cur, p++, 1);
      continue;
    }
    if ((strncmp(p, "return", 6) == 0) && !is_alnum(p[6])) {
      cur = new_token(TK_RETURN, cur, p, 6);
      p += 6;
      continue;
    }
    if ((strncmp(p, "if", 2) == 0) && !is_alnum(p[2])) {
      cur = new_token(TK_IF, cur, p, 2);
      p += 2;
      continue;
    }
    if ((strncmp(p, "else", 4) == 0) && !is_alnum(p[4])) {
      cur = new_token(TK_ELSE, cur, p, 4);
      p += 4;
      continue;
    }
    if ((strncmp(p, "while", 5) == 0) && !is_alnum(p[5])) {
      cur = new_token(TK_WHILE, cur, p, 5);
      p += 5;
      continue;
    }
    if ((strncmp(p, "for", 3) == 0) && !is_alnum(p[3])) {
      cur = new_token(TK_FOR, cur, p, 3);
      p += 3;
      continue;
    }
    if ('a' <= *p && *p <= 'z') {
      cur = new_ident_token(cur, p);
      p += cur->len;
      continue;
    }

    if (isdigit(*p)) {
      cur = new_token(TK_NUM, cur, p, 0);
      cur->val = strtol(p, &p, 10);
      continue;
    };

    error_at(p, "invalid token");
  }

  new_token(TK_EOF, cur, p, 0);
  return head.next;
}
