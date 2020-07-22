#include "9cc.h"

////////////////////////////////////
// Code Generator
////////////////////////////////////
int get_size_of_type(Type *type) {
  switch (type->kind) {
    case INT:
      return 4;

    case PTR:
      return 8;
  }
}

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
  cur = cur->next;

  if (cur == NULL) return;
  gen(cur->node);
  printf("  pop r8\n");
  cur = cur->next;

  if (cur == NULL) return;
  gen(cur->node);
  printf("  pop r9\n");
  cur = cur->next;
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

  if (!cur) return;
  printf("  mov rax, rbp\n");
  printf("  sub rax, %d\n", cur->node->num);
  printf("  mov [rax], r8\n");
  cur = cur->next;

  if (!cur) return;
  printf("  mov rax, rbp\n");
  printf("  sub rax, %d\n", cur->node->num);
  printf("  mov [rax], r9\n");
  cur = cur->next;
}

void gen_lval(Node *node) {
  if (node->kind == ND_DEREF) {
    gen(get_child_at(0, node));
    return;
  }

  if (node->kind == ND_LVAR) {
    printf("  mov rax, rbp\n");
    printf("  sub rax, %d\n", node->num);
    printf("  push rax\n");
    return;
  }

  error("left value must be variable or dereference.");
}

void gen(Node *node) {
  debug(";;;;generate::kind -> %d", node->kind);
  if (node->kind == ND_NUM) {
    printf("  mov eax, %d\n", node->num);
    printf("  push rax\n");
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
    printf("pop rax\n");

    printf("  jmp .L_if_end_%d\n", node->num);
    printf(".L_if_else_%d:\n", node->num);

    if (get_child_at(2, node) != NULL) {  // else
      gen(get_child_at(2, node));
      printf("pop rax\n");
    }

    printf(".L_if_end_%d:\n", node->num);
    printf("push rax\n");  // push a dummy value

    return;
  }

  if (node->kind == ND_WHILE) {
    printf(".L_while_begin_%d:\n", node->num);

    gen(get_child_at(0, node));  // condition

    printf("  pop rax\n");
    printf("  cmp rax, 0\n");
    printf("  je .L_while_end_%d\n", node->num);

    gen(get_child_at(1, node));  // body
    printf("pop rax\n");

    printf("  jmp .L_while_begin_%d\n", node->num);
    printf(".L_while_end_%d:\n", node->num);

    printf("push rax\n");  // push a dummy value
    return;
  }

  if (node->kind == ND_FOR) {
    if (get_child_at(0, node) != NULL) {
      gen(get_child_at(0, node));  // initialization
      printf("  pop rax\n");
    }

    printf(".L_for_begin_%d:\n", node->num);

    if (get_child_at(1, node) != NULL) {
      gen(get_child_at(1, node));  // condition
      printf("  pop rax\n");
      printf("  cmp rax, 0\n");
      printf("  je .L_for_end_%d\n", node->num);
    }

    gen(get_child_at(3, node));  // body
    printf("  pop rax\n");

    if (get_child_at(2, node) != NULL) {
      gen(get_child_at(2, node));  // step
      printf("  pop rax\n");
    }

    printf("  jmp .L_for_begin_%d\n", node->num);
    printf(".L_for_end_%d:\n", node->num);
    printf("  push rax\n");
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
    printf("  push rax\n");
    return;
  }

  if (node->kind == ND_ADDR) {
    gen_lval(get_child_at(0, node));
    return;
  }

  if (node->kind == ND_DEREF) {
    Node *ptr_node = get_child_at(0, node);
    gen(ptr_node);                              // generate an address
    printf("  pop rax\n");                      // pop an address
    if (ptr_node->type->ptr_to->kind == INT) {  // get a value on an address
      printf("  mov eax, [rax]\n");
    } else if (ptr_node->type->ptr_to->kind == PTR) {
      printf("  mov rax, [rax]\n");
    }
    printf("  push rax\n");  // push a value
    return;
  }

  if (node->kind == ND_SIZEOF) {
    Node *child_node = get_child_at(0, node);
    printf("  push %d\n", get_size_of_type(child_node->type));
    return;
  }

  // binary operator
  Node *lhs = get_child_at(0, node);
  Node *rhs = get_child_at(1, node);
  gen(lhs);
  gen(rhs);

  printf("  pop rdi\n");
  printf("  pop rax\n");

  switch (node->kind) {
    case ND_ADD:
      if (lhs->type->kind == PTR && rhs->type->kind == INT) {
        printf("  imul rdi, %d\n", get_size_of_type(lhs->type->ptr_to));
      } else if (lhs->type->kind == INT && rhs->type->kind == PTR) {
        printf("  imul rdi, %d\n", get_size_of_type(rhs->type->ptr_to));
      }
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