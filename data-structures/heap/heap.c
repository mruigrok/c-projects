#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX 1000
typedef int HeapArray[MAX+1];

typedef struct
{
    int size;
    HeapArray items;
}Heap;

Heap* InitializeHeap()
{
    Heap* new = (Heap*)malloc(sizeof(Heap));
    new->size = 0;
    return new;
}

bool IsEmpty(Heap* heap)
{
    return (heap->size == 0);
}

bool IsFull(Heap* heap)
{
    return (heap->size == MAX);
}

//Helper function to heap a 3 node tree
void HeapSwap(Heap* heap, int index)
{
    int root = heap->items[index];
    int left = heap->items[2*index];
    int right = heap->items[2*index + 1];

    if(left > right && left > root)
    {
        heap->items[index] = left;
        heap->items[2*index] = root;
    }
    else if(right > root)
    {
        heap->items[index] = right;
        heap->items[2*index + 1] = root;
    }
}

void Heapify(Heap* heap, int index)
{
    if(!IsEmpty(heap))
    {
        if(2*index <= heap->size)
        {
            //Next internal node
            Heapify(heap, index+1);

            //Only has left node
            if(index == heap->size/2 && heap->size%2 == 0)
            {
                int root = heap->items[index];
                int left = heap->items[2*index];
                if(left > root)
                {
                    heap->items[index] = left;
                    heap->items[2*index] = root;
                }
            }
            else
            {
                //Has left and right
                HeapSwap(heap, index);
            }

            Heapify(heap, 2*index);
            Heapify(heap, 2*index + 1);
        }
        else
            return;
    }
}

void Reheapify(Heap* heap)
{
    if(!IsEmpty(heap))
    {
        int index = 1;
        while(2*index <= heap->size)
        {
            HeapSwap(heap, index);
            index++;
        }
    }
}

void Insert(Heap* heap, int val)
{
    if(!IsFull(heap))
    {
        heap->items[heap->size+1] = val;
        heap->size++;
        Heapify(heap, 1);
    }
}

int Remove(Heap* heap)
{
    int ret = -1;
    if(!IsEmpty(heap))
    {
        ret = heap->items[1];
        heap->items[1] = heap->items[heap->size];
        heap->size--;
        Reheapify(heap);
        
    }
    return ret;
}

void HeapSort(int* heap, int length)
{
    if(length <= MAX)
    {
        Heap* new = InitializeHeap();
        for(int i = 0; i < length; i++)
        {
            Insert(new, heap[i]);
        }

        for(int i = 0; i < length; i++)
        {
            heap[i] = Remove(new);
        }
        free(new);
    }
}


int main()
{
    //Testing out heapsort (can be used as a priority queue)
    int A[] = {4,5,7,12,23,45,1,24,67};
    HeapSort(A,9);
    for(int i=0;i<9;i++)
    {
        printf("%d\n",A[i] );
    }
}