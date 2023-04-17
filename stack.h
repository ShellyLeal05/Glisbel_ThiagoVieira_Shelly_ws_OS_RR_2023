typedef struct node {
    long data;
    struct node* next;
} Node;

typedef struct stack {
    Node* top;
    long height;
} Stack;

Node* createNode(long data);
Stack* createStack();
void destroyStack(Stack** s_ref);
int stackIsEmpty(Stack* s);
void printStack(Stack* s);
void push(Stack* s, long element);
long pop(Stack* s);