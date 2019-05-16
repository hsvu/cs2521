// MMTree.c ... implementation of Mail Message search tree ADT
// Each node contains an (ID,message) pair; search is based on ID
// Written by John Shepherd, Feb 2019

#include <assert.h>
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sysexits.h>

#include "MMTree.h"
#include "MailMessage.h"

// data structures representing MMTree

typedef struct MMTNode *Link;

typedef struct MMTNode {
	char *msgid;		 // unique ID for message
	MailMessage message; // details of message
	Link left, right;	// sub-trees
} MMTNode;

typedef struct MMTreeRep {
	Link root;
} MMTreeRep;

static Link newMMTNode (char *id, MailMessage message);
static void doDropMMTree (Link t);
static void doShowMMTree (Link t, int level);
static MailMessage doMMTreeFind (Link t, char *id);
static bool isNodeinTree(Link t, char *id);

// create a new empty MMTree
MMTree newMMTree (void)
{
	MMTreeRep *new = malloc (sizeof *new);
	if (new == NULL) err (EX_OSERR, "couldn't allocate MMTree");
	*new = (MMTreeRep) { };
	return new;
}

void dropMMTree (MMTree mmt)
{
	assert (mmt != NULL);
	doDropMMTree (mmt->root);
	free (mmt);
}

// free memory associated with MMTree
// note: does not free ID or MailMessage
static void doDropMMTree (Link t)
{
	if (t == NULL) return;
	doDropMMTree (t->left);
	doDropMMTree (t->right);
	free (t);
}

void showMMTree (MMTree mmt)
{
	assert (mmt != NULL);
	doShowMMTree (mmt->root, 0);
}

// display a MMTree (sideways)
static void doShowMMTree (Link t, int level)
{
	if (t == NULL) return;
	doShowMMTree (t->right, level + 1);
	for (int i = 0; i < level; i++)
		printf ("   ");
	printf ("%s\n", t->msgid);
	doShowMMTree (t->left, level + 1);
}

MailMessage MMTreeFind (MMTree mmt, char *id)
{
	assert (mmt != NULL);
	return doMMTreeFind (mmt->root, id);
}

// check whether a message with ID is in a MMTree
static MailMessage doMMTreeFind (Link t, char *id)
{
	if (t == NULL)
		return NULL;
	else if (strcmp (id, t->msgid) <  0)
		return doMMTreeFind (t->left, id);
	else if (strcmp (id, t->msgid)  > 0)
		return doMMTreeFind (t->right, id);
	else // (strcmp (id, t->msgid) == 0)
		return t->message;
}

// checking if message is in tree
// taken from doMMTreeFind() and modified to produce new output
static bool isNodeinTree(Link t, char *id)
{
	if (t == NULL)
		return false;
	else if (strcmp (id, t->msgid) <  0)
		return isNodeinTree (t->left, id);
	else if (strcmp (id, t->msgid)  > 0)
		return isNodeinTree (t->right, id);
	else // (strcmp (id, t->msgid) == 0)
		return true;
}

// insert a new message into a MMTree
// message is indexed by a string ID
MMTree MMTreeInsert (MMTree t, char *id, MailMessage mesg)
{
	// making a new node
	Link newNode = newMMTNode(id, mesg);

	// checking first message, inserting at root
	if (t->root == NULL){
		t->root = newNode;

	} else {
		Link rootNode = t->root;

		// loop while the new message isnt in tree
		while (!isNodeinTree(rootNode, id)){
			char* rootID = MailMessageID(rootNode->message);

			// if rootid is bigger than id
			if (strcmp(rootID, id) > 0){
				if (rootNode->left != NULL){
					rootNode = rootNode->left;
				} else {
					rootNode->left = newNode;
					/* ~~~~~~~~~~ EXIT LOOP ~~~~~~~~~~~ */
				}

			// if rootid is smaller than id
			} else {
				if (rootNode->right != NULL){
					rootNode = rootNode->right;
				} else {
					rootNode->right = newNode;
					/* ~~~~~~~~~~ EXIT LOOP ~~~~~~~~~~~ */
				}
			}
		}
	}

	return NULL;
}


// make a new node containing a value
// (this function is used only within the ADT)
static Link newMMTNode (char *id, MailMessage message)
{
	Link new = malloc (sizeof (MMTNode));
	assert (new != NULL);
	new->msgid = id;
	new->message = message;
	new->left = new->right = NULL;
	return new;
}
