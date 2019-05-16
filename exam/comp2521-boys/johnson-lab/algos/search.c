#include "Graph.h"
#include "item.h"
#include "Queue.h"
#include "set.h"
#include "stack.h"

Graph maketree(){
    Graph test = newGraph(7);
    addEdge(test, "zero", "one");
    addEdge(test, "zero", "two");
    addEdge(test, "zero", "three");
    addEdge(test, "one", "two");
    addEdge(test, "one", "five");
    addEdge(test, "one", "six");
    addEdge(test, "two", "three");
    addEdge(test, "two", "four");
    addEdge(test, "two", "five");
    addEdge(test, "three", "four");
    addEdge(test, "four", "five");
    addEdge(test, "four", "seven");
    addEdge(test, "five", "six");
    addEdge(test, "five", "seven");
    addEdge(test, "six", "seven");
    return test;

}

int depth_first_search(Graph test, char* start , char* find){
    Stack stack = newStack();
    pushOnto(stack,start);
    while(!emptyStack(stack)){
        char * find = *popFrom(Stack);
    
    
    }

}
