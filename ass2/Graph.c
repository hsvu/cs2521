// Graph ADT interface for Ass2 (COMP2521)
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "Graph.h"

typedef struct GraphRep {
	// int nV;					// The size of the linked list
	int nE;					// Number of edges (TODO: May remove this)
	int maxV;				// Max number of nodes in graph
	AdjList *vertices;	// An array of pointers
} GraphRep;

static adjListNode *findEdge (Graph g, Vertex src, Vertex dest);
static AdjList findInIncident(Graph g, Vertex v);

// Initialises a new graph with 'noNodes' as max vertices
// noNodes = max number of nodes (TODO: may need to check this)
Graph newGraph(int noNodes) {

	Graph new = malloc(sizeof *new);
	if (!new) {
		fprintf(stderr, "An error occurred.\n");
		exit(1);
	}
	*new = (GraphRep){.nE = 0, .maxV = noNodes};
	new->vertices = malloc(noNodes * sizeof(adjListNode *));

	// Set the array indexes to NULL nodes (no nodes inserted yet)
	for (int i = 0; i < noNodes; i++) {
		new->vertices[i] = NULL;
	}
	return new;
}

// Return the current number of nodes in the graph
int numVerticies(Graph g) {
	assert (g != NULL);
	return g->maxV;
}

// Given src and dest, create an edge between them of weight 'weight'
void  insertEdge(Graph g, Vertex src, Vertex dest, int weight) {

	assert (g != NULL);

	// Allocate memory for a new edge representation and fill in members
	adjListNode *new = malloc(sizeof(adjListNode));
	new->w = dest;
	new->weight = weight;
	new->next = NULL;

	adjListNode *node = g->vertices[src];

	// If adjList is empty
	if (node == NULL) {
		g->vertices[src] = new;
	// Otherwise if not empty
	} else {
		// Traverse to the last element
		while (node->next != NULL) {
			node = node->next;
		}
		node->next = new;
	}
	g->nE++;
}

void removeEdge(Graph g, Vertex src, Vertex dest) {
	assert (g != NULL);

	// If dest is first in source array, v is a pointer to node to be deleted
	// Otherwise, a pointer to the node before
	adjListNode *v = findEdge(g, src, dest);

	// Do nothing if the edge does not exist
	if (v == NULL) return;

	// If dest is first in vertices[src]
	if (v->w == dest) {
		g->vertices[src] = g->vertices[src]->next;
		free(v);

	} else {
		adjListNode *tmp = v->next;	// pointer to node to be deleted
		// If dest is last in AdjList rep
		if (v->next->next == NULL) {
			v->next = NULL;
			free(tmp);
		// If dest is somewhere in the midde
		} else {
			v->next = tmp->next;
			free(tmp);
		}
	}
	g->nE--;
}

// Return whether or not two nodes are neighbours
bool adjacent(Graph g, Vertex src, Vertex dest) {
	assert (g != NULL);
	return findEdge(g, src, dest) ? true : false;
}

AdjList outIncident(Graph g, Vertex v) {
	assert (g != NULL);
	// No need to free this
	return g->vertices[v];
}

AdjList inIncident(Graph g, Vertex v) {
	assert (g != NULL);

	AdjList result = findInIncident(g, v);
	// Will need to free this
	return result;
}


// Print out the graph showing connections
void  showGraph(Graph g) {
	for (int i = 0; i < g->maxV; i++) {
		printf("[%d] ->", i);
		for (adjListNode *v = g->vertices[i]; v != NULL; v = v->next) {
			printf("%d ", (int)v->w);
		}
		printf("\n");
	}
}

// Free all allocated memory in the graph
void  freeGraph(Graph g) {

	if (g == NULL) return;

	// Free the adjListNodes (which represent the edges)
	adjListNode *temp;
	adjListNode *v;
	for (int i = 0; i < g->maxV; i++) {
	    if (g->vertices[i] != NULL){
		    for (v = g->vertices[i]; v->next != NULL; v = temp) {
		        temp = v->next;
			    free (v);
		    }
		    free(v);
		}
	}
	// Free the calloc'd array
	free(g->vertices);
	// Free the actual graph representation
	free (g);
	// TODO: May need to free some more things
}

// Locate dest in the src's linked list
// - Return a pointer to the node before dest if found, NULL if not found.
// - If dest is first, return pointer to first
static adjListNode *findEdge (Graph g, Vertex src, Vertex dest) {
	assert (g != NULL);
	adjListNode *curr = g->vertices[src];

	// If no outIncident vertices (i.e. dijointed)
	if (!outIncident(g, src)) return NULL;

	else {
		// If dest is first node in list
		if (curr->w == dest) {
			return curr;

		// Otherwise
		} else {
			while (curr->next != NULL) {
				if (curr->next->w == dest) return curr;
				curr = curr->next;
			}
			return NULL;
		}
	}
}

// Funtion which returns a list of incident edges
static AdjList findInIncident(Graph g, Vertex v) {
	assert (g != NULL);

	adjListNode *tmp = NULL;
	for (int index = 0; index < g->maxV; index++) {
		for (adjListNode *n = g->vertices[index]; n != NULL; n = n->next) {
			if (n->w == v) {
				adjListNode *new = malloc(sizeof(adjListNode));
				new->w = index;
				new->weight = n->weight;
				new->next = tmp;

				tmp = new;
				break;
			}
		}
	}
	return tmp;
}
