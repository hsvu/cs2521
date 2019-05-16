#include "BTree.h"


int isHeap(BTree t){
    if(t == NULL){
        return 1;
    }
    int left = 0;
    int right = 0;
    if(t->left!=NULL && t->data > t->left){
        return 0;
    }
    
    if(t->right!=NULL && t->data > t->right){
        return 0;
    }
    
    left = isHeap(t->left);
    right = isHeap(t->right);
    if(left ==1 && right == 1){
        return 1;
    }
    return 0;
}
