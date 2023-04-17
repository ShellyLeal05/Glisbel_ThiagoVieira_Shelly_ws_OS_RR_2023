#include <stdio.h>
#include <stdlib.h>
#include "stack.h"

Node *createNode(long data)
{
    Node *n = (Node *)malloc(sizeof(Node));
    n->data = data;
    n->next = NULL;
}

Stack *createStack()
{
    Stack *s = (Stack *)malloc(sizeof(Stack));
    s->top = NULL;
    s->height = 0;
}

void destroyStack(Stack **s_ref)
{
    Stack *s = *s_ref;
    free(s->top);
    free(s);
    s_ref = NULL;
}

int stackIsEmpty(Stack *s)
{
    return s->height == 0;
}

void printStack(Stack *s)
{
    if (stackIsEmpty(s))
    {
        printf("Stack is empty!\n");
    }
    else
    {
        Node *n = s->top;
        while (n->next != NULL)
        {
            printf("%ld|", n->data);
            n = n->next;
        }
        printf("%ld\n", n->data);
    }
}

void push(Stack *s, long element)
{
    Node *n = createNode(element);
    n->next = s->top;
    s->top = n;
    s->height++;
}

long pop(Stack *s)
{
    if (stackIsEmpty(s))
    {
        printf("Stack is empty!\n");
    }
    else
    {
        Node *n = s->top;
        s->top = s->top->next;
        long x = n->data;
        free(n);
        s->height--;
        return x;
    }
}