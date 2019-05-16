/*Lance-Williams Algorithm for Agglomerative Clustering
  Written by
  COMP2521 2019 T1
*/
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <assert.h>

#include "LanceWilliamsHAC.h"
#include "Graph.h"

#define numVertices numVerticies
#define SOME_LARGE_POSITIVE_NUMBER 100000000

static Dendrogram newDendrogram(int vertex, Dendrogram left, Dendrogram right);
static double max (double a, double b);
static double min (double a, double b);
//static bool isInArray(int visited[],int a);
//static void swap(Dendrogram array[], int a, int b);
//static void printDendrogram (Dendrogram d);

// typedef struct DNode *Dendrogram;
// typedef struct DNode {
//    int  vertex;
//    Dendrogram left, right;
// } DNode;


/*
 * Finds Dendrogram using Lance-Williams algorithm (as discussed in the specs)
   for the given graph g and the specified method for agglomerative clustering.
   Method value 1 represents 'Single linkage', and 2 represents 'Complete linkage'.
   For this assignment, you only need to implement the above two methods.

   The function returns 'Dendrogram' structure (binary tree) with the required information.
 *
 */
Dendrogram LanceWilliamsHAC(Graph g, int method) {

  assert(g != NULL);
  if ((method == 1) || (method == 2)){
    //continue;
  } else {
    fprintf(stderr, "Not given a legit method. Exiting...\n");
    exit(-1);
  }

  int nV = numVertices(g);
  double distance[nV][nV];
  int weight[nV][nV];

  /* initialising weight matrix to 0 and distance matrix to infinity */
  for (int a = 0; a < nV; a++){
		for (int b = 0; b < nV; b++){
			distance[a][b] = INFINITY;
      weight[a][b] = 0;
		}
	}

  /* filling in weight matrix */
  for (int x = 0; x < nV; x++){
    AdjList list = outIncident(g, x);
    while (list != NULL){ /* while there are neighbours to consider */
      weight[x][list->w] = list->weight;
      list = list->next;
    }
  }

  /* filling in distance matrix */
  for (int e = 0; e < nV; e++){
    for (int f = 0; f < nV; f++){
      if (adjacent(g, e, f)){ /* if the two nodes are neighbours */
        int finalWeight = 0;
        if (weight[e][f] != weight[f][e]){ /* checking if there is two paths */
          finalWeight = max(weight[e][f], weight[f][e]); /* finding max weight */
        } else {
          finalWeight = (double) weight[e][f];
        }
        distance[e][f] = distance[f][e] = (double) 1/finalWeight;
      }
    }
  }

  /* blocking up the replicated data in matrix with an invalid number */
  for (int g = 0; g < nV; g++){
    for (int h = g; h < nV; h++){
      distance[g][h] = -1;
    }
  }

  /*printinf out the matrix*/
  // for (int a = 0; a < nV; a++){
  //   for (int b = 0; b < nV; b++){
  //     printf("[%f]->", distance[a][b]);
  //   }
  //   printf("\n");
  // }
  // printf("\n");

  Dendrogram dendA = malloc(sizeof(Dendrogram)*nV);
    for (int i = 0; i < nV; i++){
    dendA[i] = *newDendrogram(i, NULL, NULL);
  }
  //int visited[nV];
  //int counterForVisited = 0;

  /* while the dendogram hasnt been fully formed */
  int noNodes = nV;
  for (int counter = 1; counter < noNodes - 1; counter++){

    double minDist = SOME_LARGE_POSITIVE_NUMBER;
    int cluster1 = 0;
    int cluster2 = 0;
    for (int j = 0; j < nV; j++){
      for (int k = 0; k < nV; k++){
        if (distance[j][k] > 0){ /* if the distance is valid */
          minDist = min(distance[j][k], minDist); /*finding closest clusters */
          if (minDist == distance[j][k]){ /* if there is a new minDist */
            cluster1 = j; /* storing the clusters for future use */
            cluster2 = k;
          }
        } /* end if statement for distance[j][k] */
      } /* end for loop for k */
    } /* end for loop for j */

    /* merging the two clusters together and adding it to the array*/
    /* allocating memory for the merged cluster */
    dendA = realloc(dendA, sizeof(Dendrogram)*(nV+1));
    dendA[nV + 1] = *newDendrogram(-1, &dendA[cluster1], &dendA[cluster2]); /* root vertex is irrelevant */
    nV++;

    /* removing cluster1 and 2 from dendA array by rewriting
    over it with the next element in the array */
    for (int position = cluster1; position < nV; position++){
      dendA[position] = dendA[position + 1];
    }
    nV--; /* decrease counter for number of elements in array */

    for (int position = cluster2; position < nV; position++){
      dendA[position] = dendA[position + 1];
    }
    nV--; /* decrease counter for number of elements in array */

    /* reallocing memory for dendA to remove both elements*/
    dendA = realloc(dendA, sizeof(Dendrogram)*nV);

    /* adding cluster 1 and 2 to visited array */
    // visited[counterForVisited] = cluster1;
    // counterForVisited++;
    // visited[counterForVisited] = cluster2;
    // counterForVisited++;


    /* grabbing necessary values from distance array */
    // for (int i = 0; i < nV + 1; i++){
    //   if (!isInArray(visited, i)){
    //
    //   }
    // }

    /* updating distance array by applying the Lance-Williams method */
    //if (method == 1){ /* 'Single linkage' */

    ///* add distances involving new merged cluster */
    // for (int n = 0; n < noNodes; n++){
    //   distance["the new cluster"][n] = min(distance[cluster2][n], distance[cluster2][n]);
    // }
    //
    // } else { /* 'Complete linkage' */
    //
    // /* add distances involving new merged cluster */
    // for (int p = 0; p < noNodes; p++){
    //   distance["the new cluster"][p] = max(distance[cluster2][p], distance[cluster2][p]);
    // }
    //
    // }

    // /* delete distances at involving cluster 1 and 2 in the distance array
    // by replacing value with an invalid distance */
    // for (int m = 0; m < noNodes; m++){
    //   distance[m][cluster1] = -1;
    //   distance[cluster1][m] = -1;
    //   distance[m][cluster2] = -1;
    //   distance[cluster2][m] = -1;
    // }

  }  /* end for loop */

  return dendA;
}


void freeDendrogram(Dendrogram d) {
  if (d != NULL) {
     freeDendrogram(d->left);
     freeDendrogram(d->right);
     free(d);
  }
}


static Dendrogram newDendrogram(int vertex, Dendrogram left, Dendrogram right){
  Dendrogram new = malloc(sizeof(Dendrogram));
  new->vertex = vertex;
  new->left = left;
  new->right = right;
  return new;
}

static double max (double a, double b){
  if (a <= b) return b;
  return a;
}

static double min (double a, double b){
  if (a <= b) return a;
  return b;
}

// static bool isInArray(int visited[],int a){
//
//   int length = sizeof(visited)/sizeof(int);
//   for (int i = 0; i < length; i++){
//     if (a == visited[i]) return true;
//   }
//   return false;
//
// }

// static void swap(Dendrogram array[], int a, int b){
//   Dendrogram temp = array[a];
// 	array[a] = array[b];
// 	array[b] = temp;
// }

// static void printDendrogram (Dendrogram d){
//   if (d == NULL) { return; }
//
//   printDendrogram(d->left);
//
//   if(d->left != NULL) { printf(", "); }
//   printf("%d", d->vertex);
//   if(d->right != NULL) { printf(", "); }
//
//   printDendrogram(d->right);
// }
