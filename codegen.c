#include "9cc.h"

////////////////////////////////////
// Code Generator
////////////////////////////////////

void gen_argument(Node *node) {
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

void gen_lval(Node *node) {
  if (node->kind != ND_LVAR) {
    error("left value of assignment is not local variable.");
  }

  printf("  mov rax, rbp\n");
  printf("  sub rax, %d\n", node->num);
  printf("  push rax\n");
}

void gen(Node *node) {
  debug("::::::generate::kind -> %d", node->kind);
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
    gen_argument(node);
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
    NodeLinkedListItem *cur = node->children->head;
    while (cur != NULL) {
      gen(cur->node);
      printf("  pop rax\n");
      cur = cur->next;
    }
    printf("  push rax\n");
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