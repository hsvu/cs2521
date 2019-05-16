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
  int counterArray = 0;
  int counterLoop = 0;

  if (L->nitems < 3){
    return peaksL;
  }

  L->first = L->curr;

  while (counterLoop < L->nitems - 2){
    L->curr = L->curr->next;
    if ((L->curr->prev->value < L->curr->value) && (L->curr->next->value < L->curr->value)){

      peaksL->nitems++;
      DLListNode *newNode = newDLListNode(L->curr->value);

      if (counterArray == 0){
        peaksL->first = newNode;
        peaksL->curr = newNode;
        newNode->prev = NULL;
        newNode->next = NULL;
      }

      if (counterArray != 0){
        peaksL->curr->next = newNode;
        newNode->prev = peaksL->curr;
        peaksL->curr = peaksL->curr->next;
      }
      peaksL->last = newNode;
      counterArray++;

    }
    counterLoop++;
  }

  if (counterArray == 0){
    return peaksL;
  }

	return peaksL;


}
