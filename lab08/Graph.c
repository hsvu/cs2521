// Graph.c ... implementation of Graph ADT
// Written by John Shepherd, May 2013

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "Graph.h"
#include "Queue.h"

#define MAX 29

// graph representation (adjacency matrix)
typedef struct GraphRep {
	int nV;		 // #vertices
	int nE;		 // #edges
	int **edges; // matrix of weights (0 == no edge)
} GraphRep;

static void insertPath (int *distance, int *path, int *predec, int dest);


// check validity of Vertex
int validV (Graph g, Vertex v)
{
	return (g != NULL && v >= 0 && v < g->nV);
}

// make an edge
static Edge mkEdge (Graph g, Vertex v, Vertex w)
{
	assert (g != NULL && validV (g, v) && validV (g, w));
	Edge new = {v, w}; // struct assignment
	return new;
}

// insert an Edge
// - sets (v,w) and (w,v)
void insertEdge (Graph g, Vertex v, Vertex w, int wt)
{
	assert (g != NULL && validV (g, v) && validV (g, w));

	if (g->edges[v][w] != 0 && g->edges[w][v] != 0)
		// an edge already exists; do nothing.
		return;

	g->edges[v][w] = wt;
	g->edges[w][v] = wt;
	g->nE++;
}

// remove an Edge
// - unsets (v,w) and (w,v)
void removeEdge (Graph g, Vertex v, Vertex w)
{
	assert (g != NULL && validV (g, v) && validV (g, w));
	if (g->edges[v][w] == 0 && g->edges[w][v] == 0)
		// an edge doesn't exist; do nothing.
		return;

	g->edges[v][w] = 0;
	g->edges[w][v] = 0;
	g->nE--;
}

// create an empty graph
Graph newGraph (int nV)
{
	assert (nV > 0);

	GraphRep *new = malloc (sizeof *new);
	assert (new != NULL);
	*new = (GraphRep){ .nV = nV, .nE = 0 };

	new->edges = calloc ((size_t) nV, sizeof (int *));
	assert (new->edges != NULL);
	for (int v = 0; v < nV; v++) {
		new->edges[v] = calloc ((size_t) nV, sizeof (int));
		assert (new->edges[v] != 0);
	}

	return new;
}

// free memory associated with graph
void dropGraph (Graph g)
{
	assert (g != NULL);
	for (int v = 0; v < g->nV; v++)
		free (g->edges[v]);
	free (g->edges);
	free (g);
}

// display graph, using names for vertices
void showGraph (Graph g, char **names)
{
	assert (g != NULL);
	printf ("#vertices=%d, #edges=%d\n\n", g->nV, g->nE);
	int v, w;
	for (v = 0; v < g->nV; v++) {
		printf ("%d %s\n", v, names[v]);
		for (w = 0; w < g->nV; w++) {
			if (g->edges[v][w]) {
				printf ("\t%s (%d)\n", names[w], g->edges[v][w]);
			}
		}
		printf ("\n");
	}
}

// find a path between two vertices using breadth-first traversal
// only allow edges whose weight is less than "max"
int findPath (Graph g, Vertex src, Vertex dest, int max, int *path)
{
	assert (g != NULL);

	if (src == dest){
		path[0] = dest;
		return 1;
	}

	// Distance from src to every other node (represented as indexes)
	int distance[MAX] = {0};
	int predec[MAX];
	int visited[MAX];

	// Initialising the visited and predecessor array to -1
	for (int i = 0; i <= MAX ; i++) {
		visited[i] = predec[i] = -1;
	}
	visited[src] = src;

	bool found = false;
	Queue q = newQueue();
	QueueJoin(q, src);	// Enqueue src into the new queue

	while (!found && !QueueIsEmpty(q)) {

		Vertex curr = QueueLeave(q);

		// If we reached the destination
		if (ItemEQ(curr, dest)) {
			found = true;
		}

		// Else, traverse deq'd (curr) row in adjaceny matrix
		else {
			for (int v = 0; v <= MAX ; v++) {

				// If v is not itself, AND unvisited, AND a neighbour, AND within max
				if ((g->edges[v][curr] > 0) && (visited[v] == -1)
					&& (g->edges[v][curr] < max)) {

					// Mark v as visited
					visited[v] = 1;
					// Set the predecessor of v to curr
					predec[v] = curr;
					// Distance from src to v = distance to the curr + 1
					distance[v] = distance[curr] + 1;

					QueueJoin(q, v);
				}
			}
		}
	}
	insertPath(distance, path, predec, dest);

	// return num vertices stored in path array, or 0
	// since distance[dest] represents number of edges between src and dest;
	dropQueue(q);
	return distance[dest] == 0 ? distance[dest]: distance[dest] + 1;
}

// Private function that backwardly fills the path array by stepping through the
// predecessor array
static void insertPath (int *distance, int *path, int *predec, int dest)
{
		int j = distance[dest]-1;
		for (int i = dest; predec[i] != -1; i = predec[i]) {
			if (predec[i] != -1) path[distance[dest]] = dest;
			path[j] = predec[i];
			j--;
		}
}
