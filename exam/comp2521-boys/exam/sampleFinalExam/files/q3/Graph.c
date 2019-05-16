// Graph.c ... implementation of Graph ADT

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "Graph.h"

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


// compare two Connects pairs
static int cmp(Connects a, Connects b)
{
	if (a.nconn < b.nconn)
		return -1;
	else if (a.nconn > b.nconn)
		return 1;
	else { // (a.conn == b.nconn)
		if (a.vertx < b.vertx)
			return 1;
		else if (a.vertx > b.vertx)
			return -1;
	}
	return 0; // never happens in this context
}

Connects *wellConnected(Graph g, int *nconns)
{
   assert(g != NULL && nconns != NULL);
   // TODO: replace the two lines below
   int * stack = malloc(sizeof(int)*g->nV);
   int i =0;
   for(i=0;i<g->nV;i++){
    stack[i]=-1;
   
   }
   int *visited = calloc(g->nV,sizeof(int));
   Connects* connects = malloc(sizeof(Connects));
   int pop=0;
  // int iterator = 0;
   int counter = 0;
   int vertexIT = 0;
   stack[counter]=0;
   visited[0]=1;
   int vertexcounter =0;
   while(counter>-1){
    
        pop = stack[counter];
        counter--;
        i=0;
        for(i=0;i<g->nV;i++){
            if(g->edges[pop][i]==1){
                
                vertexcounter++;
                if(visited[i]==0){
             //       printf("ENTER");
                    counter++;
                    stack[counter]=i;
                    visited[i]=1;
                }
            }
            
        
        }
        
        if(vertexcounter>=2){
            
            connects[vertexIT].vertx = pop;
            connects[vertexIT].nconn = vertexcounter;
           // printf("%d FINAL %d\n" ,vertexIT,vertexcounter);
            vertexIT++;
        }
        vertexcounter=0;
   
   }
    int nswaps;
    int j;
	for (i = 0; i < g->nV; i++) {
		nswaps = 0;
		for (j = 0; j < g->nV; j++) {
			if (cmp(connects[j],connects[j-1]) > 0) {
				Connects tmp;
				tmp = connects[j];
				connects[j] = connects[j-1];
				connects[j-1] = tmp;
				nswaps++;
			}
		}
		if (nswaps == 0) break;
	}
	//printf("%d pOPP",vertexIT);
   *nconns = vertexIT;
   return connects;
}
