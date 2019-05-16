// Dijkstra ADT interface for Ass2 (COMP2521)
#include "Dijkstra.h"
#include "Graph.h"
#include "PQ.h"
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
//#include <math.h>

#define INFINITY 10000000

static ShortestPaths newShortestPaths (Graph g, Vertex v);
static ItemPQ newItemPQ(int distance, Vertex v);
static void addingToArray(PredNode* array, PredNode* pred);
static PredNode* newPredNode(int v);


ShortestPaths dijkstra(Graph g, Vertex v) {
	assert(g != NULL);
	ShortestPaths path = newShortestPaths(g, v);

	bool visited[numVerticies(g)];

	/* initialising the distance array to 0
	 initialising the predecessor array to -1 */
	for (int a = 0; a < path.noNodes ; a++) {
		path.dist[a] = INFINITY;
		path.pred[a]->v = -1;
		visited[a] = false;
	}
	visited[v] = true;
	path.dist[v] = 0;

	ItemPQ itemV = newItemPQ(0, v); /* turning vertex into an ItemPQ */

	PQ pq = newPQ(); /* making a priority queue */
	addPQ(pq, itemV);	/* Enqueue v into the new queue */

	while (!PQEmpty(pq)){
		ItemPQ curr = dequeuePQ(pq); /* removing minPQ */

		AdjList list = outIncident(g, curr.key); /* getting list of neighbours for curr node */

		while (list != NULL){ /* while there is neighbours */

			int tempDistance = curr.value + list->weight; /* getting distance between v and neigbour node */

			/*  if the new distance is shorter than the old distance
				OR if the distance has not been set yet
				set new distance and set predecessor  */
			if (path.dist[list->w] > tempDistance){

				/* clearing out predecessor array and adding new element */
				while (path.pred[list->w]->next != NULL){
					path.pred[list->w]->v = -1;
					path.pred[list->w] = path.pred[list->w]->next;
				}
				path.dist[list->w] = tempDistance;
				path.pred[list->w]->v = curr.key;

			/* if the new distance is = to old distance i.e. 2 shortest paths
				add both to predecessor array  */
			} else if (path.dist[list->w] == tempDistance){
				PredNode* new = newPredNode(curr.key);
				addingToArray(path.pred[list->w], new);
			}

			itemV = newItemPQ(path.dist[list->w], list->w);
			if (!visited[list->w]){ /* check if they have been visited */
				visited[list->w] = true; /* marked as visited */
				addPQ(pq, itemV);
			} else {
				updatePQ(pq, itemV);
			}

			list = list->next;
	 	}
	}

	/* turning all unvisited nodes into null */
	for (int i = 0; i < path.noNodes; i++){
		if (path.pred[i]->v == -1){
			path.pred[i] = NULL;
		}
	}

	/* going through the list and checking what distances are still infinity
	 	if its still infinity therefore does not connect to v */
	for (int j = 0; j < path.noNodes; j++){
		if (path.dist[j] == INFINITY){
			path.dist[j] = 0;
		}
	}


	freePQ(pq);
	return path;
}


void showShortestPaths(ShortestPaths paths) {
	for (int i = 0; i < paths.noNodes; i++){
		printf("%d: ", i);
		while (paths.pred[i] != NULL){
			printf("[%d]->", paths.pred[i]->v);
			paths.pred[i] = paths.pred[i]->next;
		}
		printf("NULL\n");
	}
}


void freeShortestPaths(ShortestPaths paths) {
	for (int a = 0; a < paths.noNodes; a++)
	 	free(paths.pred[a]);
	free(paths.pred);
	free(paths.dist);
}


static ShortestPaths newShortestPaths (Graph g, Vertex v){

	int nV = numVerticies(g); //initialising noNodes
	assert (nV > 0);

	ShortestPaths *path = malloc(sizeof(ShortestPaths)); //allocating memory to ShortestPaths
	assert (path != NULL);
	*path = (ShortestPaths){ .noNodes = nV, .src = v };
	path->dist = calloc (nV, sizeof(int)); //allocating memory to dist array
	assert(path->dist != NULL);

	path->pred = calloc (nV, sizeof(PredNode)); //allocating memory to pred array
	for (int a = 0; a < nV; a++){
		path->pred[a] = calloc(nV, sizeof(int));
		assert(path->pred[a] != 0);
	}
	return *path;
}


static ItemPQ newItemPQ(int distance, Vertex v){
	ItemPQ item;
	item.value = distance;
	item.key = v;
	return item;
}

static PredNode* newPredNode(int v){
	PredNode* node = malloc(sizeof(PredNode));
	node->v = v;
	node->next = NULL;
	return node;
}

static void addingToArray(PredNode* array, PredNode* pred){
	if (array->next != NULL){
		array = array->next;
	}
	array->next = pred;
}
