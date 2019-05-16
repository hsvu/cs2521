// Graph ADT interface for Ass2 (COMP2521)
#include "CentralityMeasures.h"
#include "Dijkstra.h"
#include "Graph.h"
#include "PQ.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "GraphVis.h"

static NodeValues newNodeValues(int noNodes);
static void updateBetweeness(ShortestPaths path, int src, int dest, NodeValues *betweeness, int *count, int **appearances);
static void scaleAppearances(NodeValues *betweeness, int *appearances, int total);
static int numPredec(ShortestPaths path, int index);

NodeValues outDegreeCentrality(Graph g){

	int nV = numVerticies(g); /* getting no of vertices */
	NodeValues nodes = newNodeValues(nV); /* creating new nodevalues */
	nodes.noNodes = nV; /* assigning relevant values to new nodevalues */
	for (int i = 0; i < nV; i++){
		AdjList list = outIncident(g, i); /*getting list of neighbours */
		while (list != NULL){
			nodes.values[i]++; /* adding it to counter */
			list = list->next;
		}
	}
	return nodes;

}


NodeValues inDegreeCentrality(Graph g){

	int nV = numVerticies(g); /* getting no of vertices */
	NodeValues nodes = newNodeValues(nV); /* creating new nodevalues */
	nodes.noNodes = nV; /* assigning relevant values to new nodevalues */
	for (int i = 0; i < nV; i++){
		AdjList list = inIncident(g, i); /*getting list of neighbours */
		while (list != NULL){
			nodes.values[i]++; /* adding it to counter */
			list = list->next;
		}
	}
	return nodes;

}


NodeValues degreeCentrality(Graph g) {

	NodeValues inDegree = inDegreeCentrality(g);
	NodeValues outDegree = outDegreeCentrality(g);
	int nV = numVerticies(g);

	NodeValues new = newNodeValues(nV);

	new.noNodes = nV;
	for (int i = 0; i < nV; i++){

		/* summing out indegree and out degree */
		new.values[i] = inDegree.values[i] + outDegree.values[i];

	}
	return new;

}

NodeValues closenessCentrality(Graph g){
	int nV = numVerticies(g);
	NodeValues new = newNodeValues(nV);
	new.noNodes = nV;
	double distance[nV][nV];

	/* initialising everything to 0 */
	for (int a = 0; a < nV; a++){
		for (int e = 0; e < nV; e++){
			distance[a][e] = 0;
		}
	}

	/* filling in the matrix */
	for (int i = 0; i < nV; i++){
		ShortestPaths path = dijkstra(g, i);
		for (int b = 0; b < nV; b++){
			distance[i][b] = (double) path.dist[b];
		}
	}

	/* summing up closeness of node */
	for (int c = 0; c < nV; c++){
		double sum = 0;
		double nodesReached = 0; /* counting how many nodes that vertex c can reach */
		for (int d = 0; d < nV; d++){ /* calculating sum of shortest distances to nodes */
			sum = sum + distance[c][d];
			if (distance[c][d] > 0) nodesReached++;
		}

		/* applying Wasserman and Faust formula */
		double formula_Part1 = nodesReached/(nV - 1);
		double formula_Part2 = nodesReached/sum;
		double formula = formula_Part1 * formula_Part2;

		/* appending it to nodevalues */
			if (sum == 0){ /* if there is no connection */
				new.values[c] = 0;
			} else {
				new.values[c] = formula;
			}
	}

	return new;
}

NodeValues betweennessCentrality(Graph g){

	int nV = numVerticies(g);
	NodeValues betweeness = newNodeValues(nV);
	// For every src-dest pair in graph
	for (int src  = 0; src < nV; src++) {
		ShortestPaths path = dijkstra(g, src);	// shortest path from src
		for (int dest = 0; dest < nV; dest++) {
			if (src == dest) continue;
			else {
				int count = numPredec(path, dest);
				int *appearances = calloc(betweeness.noNodes, sizeof(int));

				updateBetweeness(path, src, dest, &betweeness, &count, &appearances);
				scaleAppearances(&betweeness, appearances, count);

				free(appearances);
			}
		}
	}
	return betweeness;
}


NodeValues betweennessCentralityNormalised(Graph g){

	int nV = numVerticies(g);
	NodeValues normalised = betweennessCentrality(g);

	double denominator = (nV - 1)*(nV - 2);
	double factor = (double)1 / denominator;

	for (int i = 0; i < normalised.noNodes; i++) {
		normalised.values[i] = normalised.values[i] * (double)factor;
	}

	return normalised;
}

void showNodeValues(NodeValues values){
	for (int i = 0; i < values.noNodes; i++){
		printf("%d: %f\n", i, values.values[i]);
	}
}

void freeNodeValues(NodeValues values){
	free(values.values);
}

static NodeValues newNodeValues(int noNodes){
	NodeValues new;
	new.noNodes = noNodes;
	new.values = malloc(sizeof(double)*noNodes);
	for (int i = 0; i < noNodes; i++){
		new.values[i] = 0;
	}
	return new;
}

static int numPredec(ShortestPaths path, int index) {

	int count = 0;
	for (PredNode *node = path.pred[index]; node != NULL; node = node->next) {
		count++;
	}
	return count;
}

static void updateBetweeness(ShortestPaths path, int src, int dest, NodeValues *betweeness, int *count, int **appearances) {

	for (PredNode *node = path.pred[dest]; node != NULL; node = node->next) {
		// Incrementing total number of paths
		int num = numPredec(path, node->v);
		if (num > 1) {
			(*count) += num - 1;
		}

		if (node->v == src) {
			continue;
		} else {
			(*appearances)[node->v]++;
			updateBetweeness(path, src, node->v, betweeness, count, appearances);
		}
	}

}



// Updates the values array by scaling the appearance magnitude of a vertex by
// the number of overall shortest paths to give the betweeness.
static void scaleAppearances(NodeValues *betweeness, int *appearances, int total) {

	for (int i = 0; i < betweeness->noNodes; i++) {
		if (total != 0) {
			betweeness->values[i] += appearances[i] / (double)total;
		}
	}

}
