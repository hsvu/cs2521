// Graph.c ... implementation of Graph ADT

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "Graph.h"

static int edgesCount (Graph g, Vertex v);
static void swap (Connects *matrix, int a, int b);
static void fixUP(Connects *matrix, int a);


// type for small +ve int values
typedef unsigned char bool;

// graph representation (adjacency matrix)
typedef struct _graph_rep {
   int    nV;    // #vertices
   int    nE;    // #edges
   bool **edges; // matrix of booleans
} GraphRep;

// validV ... check validity of Vertex
int validV(Graph g, Vertex v)
{
   return (g != NULL && v >= 0 && v < g->nV);
}

// mkEdge ... create an Edge value
Edge mkEdge(Graph g, Vertex v, Vertex w)
{
   assert(validV(g,v) && validV(g,w));
   Edge e; e.v = v; e.w = w;
   return e;
}

// newGraph ... create an empty graph
Graph newGraph(int nV)
{
   assert(nV > 0);
   int i, j;
   bool **e = malloc(nV * sizeof(bool *));
   assert(e != NULL);
   for (i = 0; i < nV; i++) {
      e[i] = malloc(nV * sizeof(bool));
      assert(e[i] != NULL);
      for (j = 0; j < nV; j++)
         e[i][j] = 0;
   }
   Graph g = malloc(sizeof(GraphRep));
   assert(g != NULL);
   g->nV = nV;  g->nE = 0;  g->edges = e;
   return g;
}

// readGraph ... read graph contents from file
static void readError()
{
   fprintf(stderr,"Bad graph data file\n");
   exit(1);
}
Graph readGraph(FILE *in)
{
   Graph g;
   char line[100];
   // get #vertices and create graph
   int nV = 0;
   if (fgets(line,100,in) == NULL) readError();
   if (sscanf(line,"%d",&nV) != 1) readError();
   if (nV < 2) readError();
   g = newGraph(nV);
   // read edge data and add edges
   Vertex v, w;
   while (fgets(line,100,in) != NULL) {
      sscanf(line,"%d-%d",&v,&w);
      insertE(g, mkEdge(g,v,w));
   }
   return g;
}

// showGraph ... display a graph
void showGraph(Graph g)
{
   assert(g != NULL);
   printf("# vertices = %d, # edges = %d\n\n",g->nV,g->nE);
   int v, w;
   for (v = 0; v < g->nV; v++) {
      printf("%d is connected to",v);
      for (w = 0; w < g->nV; w++) {
         if (g->edges[v][w]) printf(" %d",w);
      }
      printf("\n");
   }
}

// insertE ... add a new edge
void  insertE(Graph g, Edge e)
{
   assert(g != NULL);
   assert(validV(g,e.v) && validV(g,e.w));
   if (g->edges[e.v][e.w]) return;
   g->edges[e.v][e.w] = 1;
   g->edges[e.w][e.v] = 1;
   g->nE++;
}

// removeE ... delete an edge
void  removeE(Graph g, Edge e)
{
   assert(g != NULL);
   assert(validV(g,e.v) && validV(g,e.w));
   if (!g->edges[e.v][e.w]) return;
   g->edges[e.v][e.w] = 0;
   g->edges[e.w][e.v] = 0;
   g->nE--;
}

// wellConnected ... list of vertices
// - ordered on #connections, most connected first
Connects *wellConnected(Graph g, int *nconns)
{
   assert(g != NULL && nconns != NULL);

   Connects *adjMatrix = malloc(g->nV*sizeof(Connects));
   *nconns = 0;
   int nconnCounter = 0;

   for (int i = 0; i < g->nV; i++){
     int count = edgesCount(g, i);
     if (count >= 2){
       nconnCounter++;
       adjMatrix[i].nconn = count;
       adjMatrix[i].vertx = i;
     } else {
       adjMatrix[i].nconn = 0;
       adjMatrix[i].vertx = i;
     }
   }
   *nconns = nconnCounter;

   // organising it
   fixUP(adjMatrix, g->nV);

   return adjMatrix;
}

static int edgesCount (Graph g, Vertex v){
  int count = 0;
    for (int j = 0; j < g->nV; j++){
      if (g->edges[v][j] || g->edges[j][v]){
        count++;
      }
    }
  return count;
}

static void swap (Connects *matrix, int a, int b){
  int tempVertex = matrix[a].vertx;
  int tempConn = matrix[a].nconn;
  matrix[a].vertx = matrix[b].vertx;
  matrix[a].nconn = matrix[b].nconn;
  matrix[b].vertx = tempVertex;
  matrix[b].nconn = tempConn;
}

static void fixUP(Connects *matrix, int a){
  for (int i = 0; i < a; i++){
    for (int b = i; b < a; b++){
      if (matrix[i].nconn < matrix[b].nconn)
        swap(matrix, i, b);
    }
  }
}
