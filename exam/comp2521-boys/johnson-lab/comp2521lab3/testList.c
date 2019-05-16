// testList.c - testing DLList data type
// Written by John Shepherd, March 2013

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "DLList.h"
#include <string.h>
int main(int argc, char *argv[])
{
    FILE* infile=fopen("text","r");
	DLList myList;
	myList = getDLList(infile);
	
	putDLList(stdout,myList);
	assert(validDLList(myList));
	// TODO: more tests needed here
	
	
	
	DLList tester=myList;
	DLList test1=myList;
	
	printf("1\n\n");
	//checks if u can delete the first one
	putDLList(stdout, tester);
	DLListMove(test1,1);
	DLListDelete(tester);
	assert(strcmp(DLListCurrent(test1),DLListCurrent(test1))==0);
    assert(validDLList(tester));
    putDLList(stdout, tester);
    
    
    infile=fopen("text","r");
    tester=getDLList(infile);
    test1=tester;
    putDLList(stdout, tester);
    printf("2\n\n");
    //checks if you can delete the middle one
    putDLList(stdout, tester);
    printf("fds");
    DLListMove(tester,1);
	DLListDelete(tester);
	DLListMove(test1,2);
	assert(validDLList(tester));
	assert(strcmp(DLListCurrent(test1),DLListCurrent(test1))==0);
	putDLList(stdout, tester);
    
    infile=fopen("text","r");
    tester=getDLList(infile);
    test1=tester;
    printf("3\n\n");
     //checks if you can delete the last one
    putDLList(stdout, tester);
    DLListMove(tester,3);
	DLListDelete(tester);
	DLListMove(test1,2);
	assert(validDLList(tester));
	assert(strcmp(DLListCurrent(test1),DLListCurrent(test1))==0);
    putDLList(stdout, tester);
    infile=fopen("text","r");
    tester=getDLList(infile);
    test1=tester;
    
    
    
    printf("4\n\n");
     //checks if you can insert before the first one
    putDLList(stdout, tester);
    DLListBefore(tester, "first");
	assert(validDLList(tester));
	assert(strcmp(DLListCurrent(tester),"first")==0);
    putDLList(stdout, tester);
    infile=fopen("text","r");
    tester=getDLList(infile);
    test1=tester;
    
    
    printf("5\n\n");
    //checks if you can insert before the middle one
    putDLList(stdout, tester);
    DLListMove(tester,1);
    DLListBefore(tester, "middle");
	assert(validDLList(tester));
	assert(strcmp(DLListCurrent(tester),"middle")==0);
	putDLList(stdout, tester);
	infile=fopen("text","r");
    tester=getDLList(infile);
    test1=tester;
    
    
    
    printf("6\n\n");
      //checks if you can insert before the last one
 	putDLList(stdout, tester);
    DLListMove(tester,3);
    DLListBefore(tester, "last");
	assert(validDLList(tester));
	assert(strcmp(DLListCurrent(tester),"last")==0);
	putDLList(stdout, tester);
	infile=fopen("text","r");
    tester=getDLList(infile);
    test1=tester;
    
    printf("7\n\n");
         //checks if you can insert after the first one
	putDLList(stdout, tester);
    DLListAfter(tester, "first");
	assert(validDLList(tester));
	assert(strcmp(DLListCurrent(tester),"first")==0);
    putDLList(stdout, tester);
    infile=fopen("text","r");
    tester=getDLList(infile);
    test1=tester;
    
    printf("8\n\n");
    //checks if you can insert after the middle one
    putDLList(stdout, tester);
    DLListMove(tester,1);
    DLListAfter(tester, "middle");
	assert(validDLList(tester));
	assert(strcmp(DLListCurrent(tester),"middle")==0);
    putDLList(stdout, tester);
    infile=fopen("text","r");
    tester=getDLList(infile);
    test1=tester;
    
    printf("9\n\n");
      //checks if you can insert after the last one
    putDLList(stdout, tester);
    DLListMove(tester,3);
    DLListAfter(tester, "last");
	assert(validDLList(tester));
	assert(strcmp(DLListCurrent(tester),"last")==0);
    putDLList(stdout, tester);
    tester=getDLList(infile);
    test1=tester;
    

	return 0;
}
