#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct n
{
    int val;
    struct n* link;
}Node;

typedef struct
{
    Node* list;
}Stack;

Node* CreateNode(int val)
{
    Node* new = (Node*)malloc(sizeof(Node));
    new->val = val;
    new->link = NULL;
    return new;
}

Stack* InitializeStack()
{
    Stack* new = (Stack*)malloc(sizeof(Stack));
    new->list = NULL;
}

bool IsEmpty(Stack* stack)
{
    return (stack->list == NULL);
}

void Push(Stack* stack, int val)
{
    Node* new = CreateNode(val);
    new->link = stack->list;
    stack->list = new;
}

int Pop(Stack* stack)
{
    Node* new = stack->list;
    int val = new->val;
    stack->list = new->link;
    free(new);
    return val;    
}

void PrintStack(Stack* stack)
{
    Node* cur = stack->list;
    while(cur != NULL)
    {
        printf("%d, ", cur->val);
        cur = cur->link;
    }
    printf("\n");
}

//testing
int main()
{
    Stack* S1 = InitializeStack();
    for(int i=0; i<10; i++)
    {
        Push(S1,i);
    }
    PrintStack(S1); 

    printf("%d\n", Pop(S1));
    printf("%d\n", Pop(S1));
    printf("%d\n", Pop(S1));
    printf("%d\n", Pop(S1));
    printf("%d\n", Pop(S1));

}