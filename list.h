#ifndef __LIST_H
#define __LIST_H

struct node
{
    char data[1000];
    struct node* next;
};

typedef struct node* list;

list create_list();
void add(list l,char* data);

#endif