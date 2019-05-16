// removeValue.c 
// Written by Ashesh Mahidadia, August 2017

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "DLList.h"


/* 
    You will submit only this one file.

    Implement the function "removeValue" below. Read the exam paper for 
    detailed specification and description of your task.  

    - DO NOT modify code in the file DLList.h . 
    - You can add helper functions in this file.  
    - DO NOT add "main" function in this file. 
    
*/



void removeValue(DLList L, int value){
    
    DLListNode *free1 =L->first;
    
    while(free1->value==value){
        if(free1->next==NULL&&free1->value==value){
        L->nitems--;
        free(free1);
        L->first=NULL;
         
       
        
        return;
    }
         free1=L->first;
        L->first=L->first->next;
        L->first->prev=NULL;
           L->nitems--;
        free(free1);
        free1=L->first;
    
    }
    
    DLListNode *copy =L->first;
    DLListNode *runner=L->first->next;
    DLListNode *freer=L->first;
    while(runner!=NULL){
        if(runner==L->last&&runner->value==value){
            free(L->last);
            L->last=copy;
            L->last->next=NULL;
            L->nitems--;
            return;
        }
        
        
        if(runner->value==value){
            freer=runner;
            runner=copy->next->next;
            copy->next=runner;
            runner->prev=copy;
            free(freer);
            L->nitems--;
            
        }else{
            runner=runner->next;
            copy=copy->next;
        }
    
    }



	return;
}



