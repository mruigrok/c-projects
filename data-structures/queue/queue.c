#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

//Nodes and Queue definition
typedef struct n
{
    int val;
    struct n* prev;
    struct n* next;
}Node;

typedef struct
{
    int length;
    Node* front;
    Node* back;
}Queue;

Node* CreateNode(int val)
{
    Node* new = (Node*)malloc(sizeof(Node));
    new->val = val;
    new->prev = NULL;
    new->next = NULL;
    return new;
}

Queue* InitializeQueue()
{
    Queue* new = (Queue*)malloc(sizeof(Queue));
    new->length = 0;
    new->front = NULL;
    new->back = NULL;
    return new;
}

bool IsEmpty(Queue* queue)
{
    return queue->length == 0;
}

void Push(Queue* queue, int val)
{
    if(!queue->front)
    {
        queue->front = CreateNode(val);
        queue->back = queue->front;
    }
    else
    {
        Node* new = CreateNode(val);
        new->next = queue->back;
        queue->back->prev = new;
        queue->back = new;
    }
    queue->length++;
}

int Pop(Queue* queue)
{
    if(!IsEmpty(queue))
    {
        Node* new = queue->front->prev;
        int val = queue->front->val;
        free(queue->front);
        queue->front = new;
        return val;
    }
    else
    {
        return -1;
    }
}

void PrintQueue(Queue* queue)
{
    if(queue->front)
    {
        Node* cur = queue->front;
        while(cur->prev != NULL)
        {
            printf("%d->", cur->val);
            cur = cur->prev;
        }
         printf("%d\n", cur->val);
    }
}

int main()
{
    Queue* Q1 = InitializeQueue();
    for(int i=0; i<10; i++)
    {
        Push(Q1,i);
    }

    PrintQueue(Q1); 
}
