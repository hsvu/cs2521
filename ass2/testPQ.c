#include "PQ.h"
#include <stdlib.h>
#include <stdio.h>


ItemPQ newItemPQ(int a, int b){

  ItemPQ *p = malloc(sizeof(struct ItemPQ)) ;
  p->key = a;
  p->value = b;

  return *p;
}


int main(){

  PQ pq = newPQ();
  printf("\naddPQ: {1,100}\n");
  addPQ(pq,newItemPQ(1,100));
  //showPQ(pq);
  printf("addPQ: {2,15}\n");
  addPQ(pq,newItemPQ(2,15));
  //showPQ(pq);
  printf("addPQ: {3,1}\n");
  addPQ(pq,newItemPQ(3,1));
  //showPQ(pq);
  printf("addPQ: {4,4}\n");
  addPQ(pq,newItemPQ(4,4));
  //showPQ(pq);
  printf("addPQ: {5,3}\n");
  addPQ(pq,newItemPQ(5,3));
  //showPQ(pq);

  ItemPQ v;
  for(int i=0; i<5; i++){
      v = dequeuePQ(pq);
      printf("Dequeued: {%d,%d}\n",v.key,v.value);
      //showPQ(pq);
  }

  printf("addPQ: {2,12}\n");
  addPQ(pq,newItemPQ(2,12) );
  //showPQ(pq);
  printf("addPQ: {4,8}\n");
  addPQ(pq,newItemPQ(4,8));
  //showPQ(pq);
  printf("addPQ: {3,10}\n");
  addPQ(pq,newItemPQ(3,10));
  //showPQ(pq);

  printf("Update {4,8} to {4,11}\n");
  updatePQ(pq, newItemPQ(4,11)) ;
  //showPQ(pq);

  for(int i=0; i<3; i++){
      v = dequeuePQ(pq);
      printf("Dequeued: {%d,%d}\n",v.key,v.value);
      //showPQ(pq);
  }

  printf("\n");

  freePQ(pq);
  return EXIT_SUCCESS;
}
