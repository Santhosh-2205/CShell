#include "headers.h"

list create_list()
{
    list l = (list)malloc(sizeof(struct node));
    l->data[0] = '\0';
    l->next = NULL;
}

void add(list l,char* data)
{
    list x = (list)malloc(sizeof(struct node));
    strcpy(x->data,data);
    x->next = l->next;
    l->next = x;
}
