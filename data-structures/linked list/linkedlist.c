#include <stdio.h>
#include <stdlib.h>

typedef struct n
{
    int val;
    struct n* link;
}Node;

typedef struct
{
    int length;
    Node* head;
}LinkedList;

Node* CreateNode(int value)
{
    Node* new = (Node*)malloc(sizeof(Node));
    new->val = value;
    new->link = NULL;
    return new;
}

LinkedList* InitializeList()
{
    LinkedList* new = (LinkedList*)malloc(sizeof(LinkedList));
    new->length = 0;
    new->head = NULL;
    return new;
}

void InsertHead(LinkedList* list, int value)
{
    Node* new = CreateNode(value);
    new->link = list->head;
    list->head = new;
    list->length ++;
}

void DeleteHead(LinkedList* list)
{
    if(list->head)
    {
        Node* new = list->head;
        list->head = list->head->link;
        free(new);
        list->length--;
   }
}

void InsertTail(LinkedList* list, int value)
{   
    Node* new = CreateNode(value);
    if(list->head)
    {
        Node* cur = list->head;
        while(cur->link != NULL)
        {
            cur = cur->link;
        }
        cur->link = new;
    }
    else
    {
        list->head = new;
    }
    list->length++;
}

void DeleteTail(LinkedList* list)
{
    if(list->head)
    {
        Node* cur = list->head;
        Node* prev = NULL;

        while(cur->link != NULL)
        {
            prev = cur;
            cur = cur->link;
        }

        free(cur);
        prev->link = NULL;
        list->length--;
    }
}

void ReverseList(LinkedList* list)
{
    Node* cur = list->head;
    Node* prev = NULL;
    Node* hold = NULL;
    while(cur != NULL)
    {
        prev = cur;
        cur = cur->link;
        prev->link = hold;
        hold = prev;
    }
    list->head = prev;
}

void PrintList(LinkedList* list)
{
    if(list->head)
    {
        Node* cur = list->head;
        if(cur->link)
        {
          while(cur->link!= NULL)
            {
            printf("%d->", cur->val);
            cur = cur->link;
            }   
        }
        printf("%d\n", cur->val);
    }
}

int main(void)
{
    LinkedList* List1 = InitializeList();
    for(int i=0; i < 10; i++)
    {
        InsertHead(List1, i);
    }

    PrintList(List1);
    ReverseList(List1);
    PrintList(List1);

}



