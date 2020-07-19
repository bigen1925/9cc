#include "9cc.h"

////////////////////////////////////
// Code Generator
////////////////////////////////////

void gen_set_parameters(Node *node) {
  NodeLinkedListItem *cur = node->children->head;
  if (cur == NULL) return;
  gen(cur->node);
  printf("  pop rdi\n");
  cur = cur->next;

  if (cur == NULL) return;
  gen(cur->node);
  printf("  pop rsi\n");
  cur = cur->next;

  if (cur == NULL) return;
  gen(cur->node);
  printf("  pop rdx\n");
  cur = cur->next;

  if (cur == NULL) return;
  gen(cur->node);
  printf("  pop rcx\n");
}

void gen_assign_arguments(Node *node) {
  NodeLinkedListItem *cur = node->children->head->next;
  if (!cur) return;
  printf("  mov rax, rbp\n");
  printf("  sub rax, %d\n", cur->node->num);
  printf("  mov [rax], rdi\n");
  cur = cur->next;

  if (!cur) return;
  printf("  mov rax, rbp\n");
  printf("  sub rax, %d\n", cur->node->num);
  printf("  mov [rax], rsi\n");
  cur = cur->next;

  if (!cur) return;
  printf("  mov rax, rbp\n");
  printf("  sub rax, %d\n", cur->node->num);
  printf("  mov [rax], rdx\n");
  cur = cur->next;

  if (!cur) return;
  printf("  mov rax, rbp\n");
  printf("  sub rax, %d\n", cur->node->num);
  printf("  mov [rax], rcx\n");
  cur = cur->next;
}

void gen_lval(Node *node) {
  if (node->kind != ND_LVAR) {
    error("left value of assignment is not local variable.");
  }

  printf("  mov rax, rbp\n");
  printf("  sub rax, %d\n", node->num);
  printf("  push rax\n");
}

void gen(Node *node) {
  debug(";;;;generate::kind -> %d", node->kind);
  if (node->kind == ND_NUM) {
    printf("  push %d \n", node->num);
    return;
  }

  if (node->kind == ND_LVAR) {
    gen_lval(node);
    printf("  pop rax\n");
    printf("  mov rax, [rax]\n");
    printf("  push rax\n");
    return;
  }

  if (node->kind == ND_CALL) {
    gen_set_parameters(node);
    printf("  call %.*s\n", node->num, node->str);
    printf("  push rax\n");
    return;
  }

  if (node->kind == ND_ASSIGN) {
    gen_lval(get_child_at(0, node));
    gen(get_child_at(1, node));

    printf("  pop rdi\n");
    printf("  pop rax\n");
    printf("  mov [rax], rdi\n");
    printf("  push rdi\n");
    return;
  }

  if (node->kind == ND_RETURN) {
    gen(get_child_at(0, node));

    printf("  pop rax\n");
    printf("  mov rsp, rbp\n");
    printf("  pop rbp\n");
    printf("  ret\n");
    return;
  }

  if (node->kind == ND_IF) {
    gen(get_child_at(0, node));  // condition

    printf("  pop rax\n");
    printf("  cmp rax, 0\n");
    printf("  je .L_if_else_%d\n", node->num);

    gen(get_child_at(1, node));  // body

    printf("  jmp .L_if_end_%d\n", node->num);
    printf(".L_if_else_%d:\n", node->num);

    if (get_child_at(2, node) != NULL) gen(get_child_at(2, node));  // else

    printf(".L_if_end_%d:\n", node->num);
    return;
  }

  if (node->kind == ND_WHILE) {
    printf(".L_while_begin_%d:\n", node->num);

    gen(get_child_at(0, node));  // condition

    printf("  pop rax\n");
    printf("  cmp rax, 0\n");
    printf("  je .L_while_end_%d\n", node->num);

    gen(get_child_at(1, node));  // body

    printf("  jmp .L_while_begin_%d\n", node->num);
    printf(".L_while_end_%d:\n", node->num);
    return;
  }

  if (node->kind == ND_FOR) {
    if (get_child_at(0, node) != NULL) {
      gen(get_child_at(0, node));  // initialization
    }

    printf(".L_for_begin_%d:\n", node->num);

    if (get_child_at(1, node) != NULL) {
      gen(get_child_at(1, node));  // condition
      printf("  pop rax\n");
      printf("  cmp rax, 0\n");
      printf("  je .L_for_end_%d\n", node->num);
    }

    gen(get_child_at(3, node));

    if (get_child_at(2, node) != NULL) {
      gen(get_child_at(2, node));  // step
    }

    printf("  jmp .L_for_begin_%d\n", node->num);
    printf(".L_for_end_%d:\n", node->num);
    return;
  }

  if (node->kind == ND_BLOCK) {
    debug("hoge:::::::");
    NodeLinkedListItem *cur = node->children->head;
    while (cur != NULL) {
      debug("fuga:::::::");

      gen(cur->node);
      printf("  pop rax\n");
      cur = cur->next;
    }
    printf("  push rax\n");
    return;
  }

  if (node->kind == ND_PROGRAM) {
    // generate header codes
    printf(".intel_syntax noprefix\n");
    printf(".globl main\n");

    NodeLinkedListItem *cur = node->children->head;
    for (; cur != NULL; cur = cur->next) {
      gen(cur->node);
    }
    return;
  }

  if (node->kind == ND_FUNC) {
    // label
    printf("%.*s:\n", node->num, node->str);
    // prologue
    printf("  push rbp\n");
    printf("  mov rbp, rsp\n");
    printf("  sub rsp, 208\n");

    // assign arguments
    gen_assign_arguments(node);

    // body
    gen(get_child_at(0, node));
    printf("  pop rax\n");

    // epilogue
    printf("  mov rsp, rbp\n");
    printf("  pop rbp\n");
    printf("  ret\n");
    return;
  }

  if (node->kind == ND_VAR_DEC) {
    printf("  push 0\n");  // for convinience, push a dummy value.
    return;
  }

  if (node->kind == ND_ADDR) {
    gen_lval(get_child_at(0, node));
    return;
  }

  if (node->kind == ND_DEREF) {
    gen(get_child_at(0, node));    // generate an address
    printf("  pop rax\n");         // pop an address
    printf("  mov rax, [rax]\n");  // get a value on an address
    printf("  push rax\n");        // push a value
    return;
  }

  gen(get_child_at(0, node));
  gen(get_child_at(1, node));

  printf("  pop rdi\n");
  printf("  pop rax\n");

  switch (node->kind) {
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