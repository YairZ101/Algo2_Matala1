#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


// Create a random number between 0 to 1
double rnd_gnr() {
	return (double)rand() / (double)RAND_MAX;
}

// A structure to represent a queue.
typedef struct Queue {
	int val;
	struct Queue* next;
} Queue;


void enqueue(Queue** head, int val) {
	Queue* new_node = malloc(sizeof(Queue));
	if (!new_node) return;

	new_node->val = val;
	new_node->next = *head;

	*head = new_node;
}


int dequeue(Queue** head) {
	Queue* current, * prev = NULL;
	int retval = -1;

	if (*head == NULL) return -1;

	current = *head;
	while (current->next != NULL) {
		prev = current;
		current = current->next;
	}

	retval = current->val;
	free(current);

	if (prev)
		prev->next = NULL;
	else
		*head = NULL;

	return retval;
}

// A structure to represent a graph.
// A graph is an adjacency matrix.
// Size of matrix will be V^2 (number of vertices in graph)
typedef struct Graph
{
	int V;
	bool** matrix;
} Graph;

// Adds an edge to an undirected graph
void addEdge(Graph* graph, int src, int dest)
{
	graph->matrix[src][dest] = 1;
	graph->matrix[dest][src] = 1;
}

// A utility function that creates a graph of V vertices
Graph* build_random_graph(int V, double P)
{
	int i, j;

	struct Graph* graph = (Graph*)malloc(sizeof(Graph));
	graph->V = V;

	// Create an array of adjacency lists. Size of
	// array will be V
	graph->matrix = malloc(V * sizeof(bool*));

	// Initialize each adjacency list as empty by
	// making head as NULL

	for (i = 0; i < V; i++) {
		graph->matrix[i] = calloc(V, sizeof(bool));
	}

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

void freeGraph(Graph* graph) {
	for (int i = 0; i < graph->V; i++)
	{
		free(graph->matrix[i]);
	}
	free(graph->matrix);
	free(graph);
}

void printGraph(Graph* graph) {
	int V = graph->V;
	int i, j;
	for (i = 0; i < V; i++) {
		for (j = 0; j < V; j++) {
			printf("%d ", graph->matrix[i][j]);
		}
		printf("\n");
	}
}

bool is_isolated(Graph* graph) {
	int V = graph->V;
	int i, j;
	for (i = 0; i < V; i++) {
		for (j = 0; j < V; j++) {
			if (graph->matrix[i][j] == 1)
				break;
			if (j == V - 1 && graph->matrix[i][j] == 0)
				return 1;
		}
	}
	return 0;
}

void bfs(Graph* graph, int src, bool* visited) {
	// Set source as visited
	visited[src] = true;
	Queue* head = NULL;
	enqueue(&head, src);
	int vis;

	while (head) {
		vis = dequeue(&head);

		for (int i = 0; i < graph->V; i++) {
			if (graph->matrix[vis][i] && (!visited[i])) {
				// Push to the queue
				enqueue(&head, i);
				// Set as visited
				visited[i] = true;
			}
		}
	}
}

bool connectivity(Graph* graph) {
	int V = graph->V, i;

	bool* visited = malloc(V * sizeof(bool));

	for (i = 0; i < V; i++)
		visited[i] = false;

	bfs(graph, 0, visited);

	for (i = 0; i < V; i++) {
		if (!(visited[i])) {
			free(visited);
			return false;
		}
	}

	free(visited);
	return true;
}

// Driver program to test above functions
int main()
{
	// create the graph given in above fugure
	for (int i = 1; i <= 1000; i++)
	{
		int V = 1000;
		struct Graph *graph = build_random_graph(V, 0.009);
		printf("Graph number %d:\n", i);
		//printGraph(graph);
		printf("Is Isolated: %d\n", is_isolated(graph));
		printf("Connectivity: %d\n\n", connectivity(graph));
		freeGraph(graph);
	}
	
	// print the adjacency list representation of the above graph
	
	return 0;
}