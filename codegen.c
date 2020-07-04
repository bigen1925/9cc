#include "9cc.h"

////////////////////////////////////
// Code Generator
////////////////////////////////////

void gen_lval(Node *node)
{
    if (node->kind != ND_LVAR)
    {
        error("left value of assignment is not local variable.");
    }

    printf("  mov rax, rbp\n");
    printf("  sub rax, %d\n", node->offset);
    printf("  push rax\n");
}

void gen(Node *node)
{
    if (node->kind == ND_NUM)
    {
        printf("  push %d \n", node->val);
        return;
    }
    if (node->kind == ND_LVAR)
    {
        gen_lval(node);
        printf("  pop rax\n");
        printf("  mov rax, [rax]\n");
        printf("  push rax\n");
        return;
    }
    if (node->kind == ND_ASSIGN)
    {

        gen_lval(node->first);
        gen(node->second);

        printf("  pop rdi\n");
        printf("  pop rax\n");
        printf("  mov [rax], rdi\n");
        printf("  push rdi\n");
        return;
    }
    if (node->kind == ND_RETURN)
    {
        gen(node->first);

        printf("  pop rax\n");
        printf("  mov rsp, rbp\n");
        printf("  pop rbp\n");
        printf("  ret\n");
        return;
    }
    if (node->kind == ND_IF)
    {
        gen(node->first); // condition

        printf("  pop rax\n");
        printf("  cmp rax, 0\n");

        printf("  je .L_if_else_%d\n", node->seq);

        gen(node->second); // body

        printf("  jmp .L_if_end_%d\n", node->seq);

        printf(".L_if_else_%d:\n", node->seq);

        if (node->third != NULL)
            gen(node->third); // else

        printf(".L_if_end_%d:\n", node->seq);
        return;
    }
    if (node->kind == ND_WHILE)
    {
        printf(".L_while_begin_%d:\n", node->seq);
        gen(node->first); // condition

        printf("  pop rax\n");
        printf("  cmp rax, 0\n");

        printf("  je .L_while_end_%d\n", node->seq);

        gen(node->second); // body
        printf("  jmp .L_while_begin_%d\n", node->seq);

        printf(".L_while_end_%d:\n", node->seq);
        return;
    }
    if (node->kind == ND_FOR)
    {
        if (node->first != NULL)
            gen(node->first); // initialization
        printf(".L_for_begin_%d:\n", node->seq);

        if (node->second != NULL)
        {
            gen(node->second); // condition
            printf("  pop rax\n");
            printf("  cmp rax, 0\n");
            printf("  je .L_for_end_%d\n", node->seq);
        }

        gen(node->fourth); // body
        if (node->third != NULL)
            gen(node->third); // step
        printf("  jmp .L_for_begin_%d\n", node->seq);

        printf(".L_for_end_%d:\n", node->seq);
        return;
    }

    gen(node->first);
    gen(node->second);

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