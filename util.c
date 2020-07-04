#include "9cc.h"

////////////////////////////////////
// Utilities
////////////////////////////////////
LinkedList *new_node_list()
{
    LinkedList *list = calloc(1, sizeof(LinkedList));
    list->head = NULL;
    list->tail = NULL;

    return list;
}

void append(LinkedList *list, Node *node)
{
    ListItem *item = calloc(1, sizeof(ListItem));
    item->node = node;
    item->next = NULL;

    if (list->head == NULL)
    {
        list->head = item;
    }
    else
    {
        list->tail->next = item;
    }

    list->tail = item;
}

// output debug log to stderr
void debug(char *fmt, ...)
{
    if (!debug_mode)
        return;

    va_list ap;
    va_start(ap, fmt);

    vfprintf(stderr, fmt, ap);
    fprintf(stderr, "\n");
}
// error output with a position
void error(char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);

    vfprintf(stderr, fmt, ap);
    fprintf(stderr, "\n");
    exit(1);
}

// error output with a position
void error_at(char *loc, char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);

    int pos = loc - user_input;
    fprintf(stderr, "%s\n", user_input);
    fprintf(stderr, "%*s", pos, "");
    fprintf(stderr, "^ ");
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, "\n");
    exit(1);
}