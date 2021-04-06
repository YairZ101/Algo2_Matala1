// A C Program to demonstrate adjacency list
// representation of graphs
#include <stdio.h>
#include <stdlib.h>


// Create a random number between 0 to 1
double rnd_gnr()
{
	return (double)rand() / (double)RAND_MAX;
}


// A structure to represent an adjacency list node
struct AdjListNode
{
	int dest;
	struct AdjListNode* next;
};

// A structure to represent an adjacency list
struct AdjList
{
	struct AdjListNode* head;
};


// A structure to represent a graph. A graph
// is an array of adjacency lists.
// Size of array will be V (number of vertices
// in graph)
struct Graph
{
	int V;
	struct AdjList* array;
};


// A utility function to create a new adjacency list node
struct AdjListNode* newAdjListNode(int dest)
{
	struct AdjListNode* newNode =
		(struct AdjListNode*)malloc(sizeof(struct AdjListNode));
	newNode->dest = dest;
	newNode->next = NULL;
	return newNode;
}


// Adds an edge to an undirected graph
void addEdge(struct Graph* graph, int src, int dest)
{
	// Add an edge from src to dest. A new node is
	// added to the adjacency list of src. The node
	// is added at the beginning
	struct AdjListNode* newNode = newAdjListNode(dest);
	newNode->next = graph->array[src].head;
	graph->array[src].head = newNode;

	// Since graph is undirected, add an edge from
	// dest to src also
	newNode = newAdjListNode(src);
	newNode->next = graph->array[dest].head;
	graph->array[dest].head = newNode;
}


// A utility function that creates a graph of V vertices
struct Graph* build_random_graph(int V, float P)
{
	int i, j;

	struct Graph* graph =
		(struct Graph*)malloc(sizeof(struct Graph));
	graph->V = V;

	// Create an array of adjacency lists. Size of
	// array will be V
	graph->array =
		(struct AdjList*)malloc(V * sizeof(struct AdjList));

	// Initialize each adjacency list as empty by
	// making head as NULL
	for (i = 0; i < V; i++) 
		graph->array[i].head = NULL;

	if (P == 0)
		return graph;

	for (i = 0; i < V; i++) {
		for (j = i + 1; j < V; j++) {
			if (P >= rnd_gnr()) {
				addEdge(graph, i, j);
			}
		}
	}

	return graph;
}

// A utility function to print the adjacency list
// representation of graph
void printGraph(struct Graph* graph)
{
	int v;
	for (v = 0; v < graph->V; v++)
	{
		struct AdjListNode* pCrawl = graph->array[v].head;
		printf("\n Adjacency list of vertex %d\n head ", v);
		while (pCrawl)
		{
			printf("-> %d", pCrawl->dest);
			pCrawl = pCrawl->next;
		}
		printf("\n");
	}
}

void freeGraph(struct Graph* graph) {
	for (int v = 0; v < graph->V; v++)
	{
		struct AdjListNode* pCrawl = graph->array[v].head;
		struct AdjListNode* pCrawlTemp;
		while (pCrawl)
		{
			pCrawlTemp = pCrawl->next;
			free(pCrawl);
			pCrawl = pCrawlTemp;
		}
	}
	free(graph->array);
	free(graph);
}

// Driver program to test above functions
int main()
{
	// create the graph given in above fugure
	for (int i = 1; i <= 1000; i++)
	{
		int V = 10000;
		struct Graph* graph = build_random_graph(V, 0.1);
		printf("%d \n", i);
		freeGraph(graph);
	}

	// print the adjacency list representation of the above graph
	

	return 0;
}
