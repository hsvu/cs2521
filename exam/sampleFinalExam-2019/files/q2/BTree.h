// Binary Tree ADT  ...

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>


#define data(tree)  ((tree)->data)
#define left(tree)  ((tree)->left)
#define right(tree) ((tree)->right)


typedef struct Node *BTree;
typedef struct Node {
   int  data;
   BTree left, right;
} Node;


int isHeap(BTree t){
  if (t == NULL) return 1;

  if (t != NULL){
    if (data(t) > data(left(t) && data(t) > data(right(t)))){
      isHeap(left(t));
      isHeap(right(t));
    } else {
      return 0;
    }
    return 1;
  }
}
