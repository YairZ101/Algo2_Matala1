#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <windows.h>


// Create a random number between 0 to 1
double rnd_gnr() {
	return (double)rand() / (double)RAND_MAX;
}

typedef struct Param {
	int V;
	double P;
} Gparam;

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

void freeQueue(Queue* head) {
	Queue* temp;
	while (head) {
		temp = head->next;
		free(head);
		head = temp;
	}
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

void bfs(Graph* graph, int src, int dest, bool* visited, int* distance) {
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
				// If dst != -1, which means we search for a distance from the source
				if (dest != -1) {
					distance[i] = distance[vis] + 1;

					if (i == dest) {
						freeQueue(head);
						return;
					}
				}
			}
		}
	}
	freeQueue(head);
}

bool connectivity(Graph* graph) {
	int V = graph->V, i;

	bool* visited = malloc(V * sizeof(bool));

	for (i = 0; i < V; i++)
		visited[i] = false;

	bfs(graph, 0, -1, visited, NULL);

	for (i = 0; i < V; i++) {
		if (!(visited[i])) {
			free(visited);
			return false;
		}
	}

	free(visited);
	return true;
}

int diameter(Graph* graph) {
	int V = graph->V, i, j, h, diam = 0;

	bool* visited = malloc(V * sizeof(bool));
	int* distance = calloc(V, sizeof(int));

	for (i = 0; i < V; i++)
		visited[i] = false;

	for (i = 0; i < V; i++) {
		for (j = i + 1; j < V; j++) {
			bfs(graph, i, j, visited, distance);
			if (distance[j] > diam)
				diam = distance[j];

			for (h = 0; h < V; h++) {
				visited[h] = false;
				distance[h] = 0;
			}
		}
	}

	free(visited);
	free(distance);

	return diam;
}


DWORD WINAPI edrosRenyiModel(Gparam* Param) {
	int isolated = 0, connect = 0, diam = -1;
	int V = Param->V;
	double P = Param->P;
	struct Graph* graph = build_random_graph(V, P);
	//printf("Graph number %d:\n", i);
	isolated = is_isolated(graph);
	if (!isolated)
		connect = connectivity(graph);
	if (connect)
		diam = diameter(graph);
	printf("Is Isolated: %d\n", isolated);
	printf("Connectivity: %d\n", connect);
	printf("Diameter: %d\n\n", diam);
	freeGraph(graph);
}

void main()
{
	DWORD ThreadId;
	HANDLE * ThreadHandle = malloc(10*sizeof(HANDLE));
	
	Gparam Param;
	Param.P = 0.12;
	Param.V = 500;
	for (int i = 0; i < 10; i++) {
		ThreadHandle[i] = CreateThread(NULL, /* default security attributes */ 0, /* default stack size */
			edrosRenyiModel, /* thread function */ &Param, /* parameter to thread function */ 0, /* default creation    flags */ &ThreadId);
		/* returns the thread identifier */
	}
	WaitForMultipleObjects(10, ThreadHandle, TRUE, INFINITE);

	for (int i = 0; i < 10; i++) {
		CloseHandle(ThreadHandle[i]);

	}


}