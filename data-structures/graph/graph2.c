#include <stdio.h>
#include <stdlib.h>

//Maximum number of nodes in the graph (nodes numbered 0-999)
#define N 1000

typedef struct n{
    int dest;
    struct n* next;
}Node;

typedef struct{
    int source;
    int dest;
}Edge;

typedef struct{
    int node_count;
    Node* head[N];
}Graph;

Graph* InitializeGraph()
{
    Graph* new = (Graph*)malloc(sizeof(Graph));

    for(int i = 0; i < N; i++)
    {
        new->head[i] = NULL;
    }
    new->node_count = 0;

    return new;    
}

Node* CreateNode(int dest)
{
    Node* new = (Node*)malloc(sizeof(Node));
    new->dest = dest;
    new->next = NULL;
    return new;
}

Edge* CreateEdge(int source, int dest)
{
    Edge* new = (Edge*)malloc(sizeof(Edge));
    new->source = source;
    new->dest = dest;
    return new; 
}

void AddDirectedEdge(Graph* graph, int source, int dest)
{
    //Create the node then add to adjacency list
    if(source < 1000 && dest < 1000)
    {
        Node* node = CreateNode(dest);
        node->next = graph->head[source];
        graph->head[source] = node;
    }
    else
    {
        printf("Error: Source or destination is out of range\n");
    } 
}

void PrintGraph(Graph* graph)
{
    printf("\tAdjancency List\n");

    for(int i = 0; i < N; i++)
    {
        Node* cur = graph->head[i];
        if(cur == NULL)
            continue;
        else
        {
            while(cur != NULL)
            {
                printf("(%d -> %d) ", i, cur->dest);
                cur = cur->next;
            }
            printf("\n");  
        }
    }
}

int main(void)
{
    Graph* G1 = InitializeGraph();
    
    for(int i = 0; i < 10; i++)
    {
        AddDirectedEdge(G1, i, i%5);
    }

      for(int i = 0; i < 10; i++)
    {
        AddDirectedEdge(G1, i%5, i);
    }

    PrintGraph(G1);

    return 0;
}