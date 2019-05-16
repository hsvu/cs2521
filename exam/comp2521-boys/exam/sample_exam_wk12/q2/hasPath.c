
/* hasPath.c 
   Written by Ashesh Mahidadia, October 2017
*/

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "Graph.h"



/* 
    You will submit only this one file.

    Implement the function "hasPath" below. Read the exam paper for 
    detailed specification and description of your task.  

    - You can add helper functions in this file, if required.  

    - DO NOT modify code in the file BSTree.h . 
    - DO NOT add "main" function in this file. 
*/
int dfsPathCheck(Graph g,int src,int dest);

int *visited;  // array of visited



int hasPath(Graph g, Vertex src, Vertex dest)
{
    visited = calloc(g->nV,sizeof(int));
    return dfsPathCheck(g,src,dest);
	// Implement this function, 
	// also change the return value below!

   
}

int dfsPathCheck(Graph g,int src,int dest){
    if(src==dest){
        return 1;
    }
    if(visited[src]==1){
        return 0;
    }

    visited[src]=1;
    int i = 0;
    for(i=0;i<g->nV;i++){
        if(!g->edges[src][i]||visited[i]){
            continue;
        }
        if(dfsPathCheck(g,i,dest)){
            return 1;
        }
    }
    return 0;
}
