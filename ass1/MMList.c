// MMList.c ... implementation of List-of-Mail-Messages ADT
// Written by John Shepherd, Feb 2019

#include <assert.h>
#include <err.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sysexits.h>

#include "MMList.h"
#include "MailMessage.h"

// data structures representing MMList

typedef struct MMListNode *Link;

typedef struct MMListNode {
	MailMessage data; // message associated with this list item
	Link next;		  // pointer to node containing next element
} MMListNode;

typedef struct MMListRep {
	Link first; // node containing first value
	Link last;  // node containing last value
	Link curr;  // current node (for iteration)
} MMListRep;

static Link newMMListNode (MailMessage mesg);

// create a new empty MMList
MMList newMMList (void)
{
	MMListRep *new = malloc (sizeof *new);
	if (new == NULL) err (EX_OSERR, "couldn't allocate MMList");
	*new = (MMListRep) { };
	return new;
}

// free up memory associated with list
// note: does not remove Mail Messages

void dropMMList (MMList L)
{
	assert (L != NULL);
	Link curr = L->first;
	while (curr != NULL) {
		Link next = curr->next;
		free (curr);
		curr = next;
	}
	free (L);
}

// display list as one integer per line on stdout
void showMMList (MMList L)
{
	assert (L != NULL);
	for (Link curr = L->first; curr != NULL; curr = curr->next)
		showMailMessage (curr->data, 0);
}


// insert mail message in order
// ordering based on MailMessageDateTime
void MMListInsert (MMList L, MailMessage mesg)
{
	assert (L != NULL);
	assert (mesg != NULL);
	assert(MMListIsOrdered (L));

	// turn mesg into a node
	Link newNode = newMMListNode(mesg);
	DateTime newNode_date = MailMessageDateTime(newNode->data);


	// start at the beginning of the list
	MMListStart(L);

	// if list is empty, add first node
	if (L->first == NULL){
		L->first = newNode;
		L->last = newNode;
		L->curr = newNode;
	}

	// adding second node
	else if (L->first == L->last){
		DateTime firstNode_date = MailMessageDateTime(L->first->data);

		// if newNode is before firstNode
		// making new node the first node
		if (DateTimeAfter(firstNode_date, newNode_date)){
			Link temp_firstNode = L->first;
			L->first = newNode;
			newNode->next = temp_firstNode;
			L->last = temp_firstNode;
		}

		// if newNode is after firstNode
		// making newnode the last node
		else {
			L->first->next = newNode;
			L->last = newNode;
		}
	}

	// default after adding first and second nodes
	// note we are already at the start of the list
	else {
		DateTime currNode_date = MailMessageDateTime(L->curr->data);
		DateTime nextcurrNode_date = MailMessageDateTime(L->curr->next->data);

		// create flag to check if we integrate new node in list
		int flag = 0;

		// if new node should be before curr (first node)
		if (DateTimeAfter(currNode_date, newNode_date)){
			newNode->next = L->first;
			L->first = newNode;
			flag = 1;
		}

		// while the next curr node is not the last node
		while ((L->curr->next != L->last) && (flag == 0)){
			currNode_date = MailMessageDateTime(L->curr->data);
			nextcurrNode_date = MailMessageDateTime(L->curr->next->data);

			// if new node is before the next curr node
			if (DateTimeAfter(nextcurrNode_date, newNode_date)){
				Link temp_nextcurrNode = L->curr->next;
				L->curr->next = newNode;
				newNode->next = temp_nextcurrNode;
				flag = 1;
				break;
			}

			// we move on to the next curr node
			else {
				MMListNext(L);
			}
		}

		// if the next curr node IS the last node
		// also checking if we already put newnode in list
		if ((L->curr->next == L->last) && (flag == 0)){

			// if the new node goes after the last node
			// making the newNode the last node
			if (DateTimeBefore(nextcurrNode_date, newNode_date)){
				L->last->next = newNode;
				L->last = newNode;
			}

			// if the new node goes just before the last node
			// last node stays the same
			else {
				L->curr->next = newNode;
				newNode->next = L->last;
			}
		}


	}

	MMListNext(L);

}

// create a new MMListNode for mail message
// (this function is used only within the ADT)
static Link newMMListNode (MailMessage mesg)
{
	Link new = malloc (sizeof *new);
	if (new == NULL) err (EX_OSERR, "couldn't allocate MMList node");
	*new = (MMListNode) { .data = mesg };
	return new;
}

// check whether a list is ordered (by MailMessageDate)
bool MMListIsOrdered (MMList L)
{
	DateTime prevDate = NULL;
	for (Link n = L->first; n != NULL; n = n->next) {
		DateTime currDate = MailMessageDateTime (n->data);
		if (prevDate != NULL && DateTimeAfter (prevDate, currDate))
			return false;
		prevDate = currDate;
	}
	return true;
}

// start scan of an MMList
void MMListStart (MMList L)
{
	assert (L != NULL);
	L->curr = L->first;
}

// get next item during scan of an MMList
MailMessage MMListNext (MMList L)
{
	assert (L != NULL);
	if (L->curr == NULL)
		// this is probably an error
		return NULL;

	MailMessage mesg = L->curr->data;
	L->curr = L->curr->next;
	return mesg;
}

// check whether MMList scan is complete
bool MMListEnd (MMList L)
{
	assert (L != NULL);
	return (L->curr == NULL);
}
