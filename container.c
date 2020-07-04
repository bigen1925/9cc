#include "9cc.h"

void append(Node *node, NodeLinkedList *list) {
  NodeLinkedListItem *item = calloc(1, sizeof(NodeLinkedListItem));
  item->node = node;

  if (list->head == NULL) {
    list->head = item;
  } else {
    list->tail->next = item;
  }
  list->tail = item;
}

Node *get_at(int n, NodeLinkedList *list) {
  NodeLinkedListItem *cur = list->head;
  for (int i = 0; i < n; i++) {
    cur = cur->next;
  }
  return cur->node;
}

void append_child(Node *child, Node *parent) {
  append(child, parent->children);
}

Node *get_child_at(int n, Node *node) { return get_at(n, node->children); }