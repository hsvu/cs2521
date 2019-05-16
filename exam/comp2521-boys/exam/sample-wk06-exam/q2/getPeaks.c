// getPeaks.c 
// Written by Ashesh Mahidadia, August 2017

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "DLList.h"


/* 
    You will submit only this one file.

    Implement the function "getPeaks" below. Read the exam paper for 
    detailed specification and description of your task.  

    - DO NOT modify code in the file DLList.h . 
    - You can add helper functions in this file.  
    - DO NOT add "main" function in this file. 
    
*/

DLList getPeaks(DLList L){
    DLList peaksL = newDLList();
    if(L->first->next==NULL){
        return peaksL;
    }
    if(L->first->next->next==NULL){
        return peaksL;
    }
    DLListNode* copy=L->first->next;
    DLListNode* prev=L->first;
    DLListNode* next=L->first->next->next;
    DLListNode* hold;
	int i =0;
    while(next!=NULL){
        if(prev->value < copy -> value && copy->value > next->value && i==0){
            peaksL->first = newDLListNode(copy->value);
            hold=peaksL->first;
            i=1;
        }
        else if(prev->value < copy -> value && copy->value > next->value){
            hold->next= newDLListNode(copy->value);
            hold=hold->next;
        }
            copy=copy->next;
            prev=prev->next;
            next=next->next;
            
    
    }
    
    peaksL->last=hold;
    peaksL->curr=L->first;

	return peaksL;

}



