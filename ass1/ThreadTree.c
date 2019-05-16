// ThreadTree.c ... implementation of Tree-of-Mail-Threads ADT
// Written by John Shepherd, Feb 2019

#include <assert.h>
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sysexits.h>
#include <limits.h>

#include "MMList.h"
#include "MMTree.h"
#include "MailMessage.h"
#include "ThreadTree.h"

// Representation of ThreadTree's

typedef struct ThreadTreeNode *Link;

typedef struct ThreadTreeNode {
	MailMessage mesg;
	Link next, replies;
} ThreadTreeNode;

typedef struct ThreadTreeRep {
	Link messages;
} ThreadTreeRep;

// Auxiliary data structures and functions

// Add any new data structures and functions here ...
static Link SearchThreadTree (Link root, Link new);
static Link newThreadTreeNode(MailMessage message);
static void doDropThreadTree (Link t);
static void doShowThreadTree (Link t, int level);
// END auxiliary data structures and functions

// create a new empty ThreadTree
ThreadTree newThreadTree (void)
{
	ThreadTreeRep *new = malloc (sizeof *new);
	if (new == NULL) err (EX_OSERR, "couldn't allocate ThreadTree");
	*new = (ThreadTreeRep) { };
	return new;
}

void dropThreadTree (ThreadTree tt)
{
	assert (tt != NULL);
	doDropThreadTree (tt->messages);
}

// free up memory associated with list
static void doDropThreadTree (Link t)
{
	if (t == NULL)
		return;

	for (Link curr = t, next; curr != NULL; curr = next) {
		next = curr->next;
		doDropThreadTree (curr->replies);
		// don't drop curr->mesg, in case referenced elsehwere
		free (curr);
	}
}

void showThreadTree (ThreadTree tt)
{
	assert (tt != NULL);
	doShowThreadTree (tt->messages, 0);
}

// display thread tree as hiearchical list
static void doShowThreadTree (Link t, int level)
{
	if (t == NULL)
		return;
	for (Link curr = t; curr != NULL; curr = curr->next) {
		showMailMessage (curr->mesg, level);
		doShowThreadTree (curr->replies, level + 1);
	}
}

// insert mail message into ThreadTree
// if a reply, insert in appropriate replies list
// whichever list inserted, must be in timestamp-order
ThreadTree ThreadTreeBuild (MMList mesgs, MMTree msgids)
{
	ThreadTree newTree = newThreadTree();

	MMListStart(mesgs);
	MailMessage curr = MMListNext(mesgs);

	// inserting first node into tree
	Link newNode = newThreadTreeNode(curr);
	newTree->messages = newNode;
	Link root = newTree->messages;
	curr = MMListNext(mesgs);

	// inserting nodes into Tree
	while (curr != NULL){
		newNode = newThreadTreeNode(curr);
		Link currNode = SearchThreadTree(root, newNode);

		char* id_newnode_reply = MailMessageRepliesTo(newNode->mesg);

		// case 1 where if there is no reply
		if (id_newnode_reply == NULL){

			// adding it next where there is no next node
			while (currNode->next != NULL){
				currNode = currNode->next;
			}
			currNode->next = newNode;

		// case two where there is a reply
		} else {
			// if there is already a reply node linked, transverse until next
			// available spot in
			if (currNode->replies != NULL){
				currNode = currNode->replies;

				// adding it next if there is no next node
				while (currNode->next != NULL){
					currNode = currNode->next;
				}
				currNode->next = newNode;

			// if there is no reply node linked, link it as a reply node
			} else {
				currNode->replies = newNode;
			}
		}
		curr = MMListNext(mesgs);
	}

	return newTree; // change this line
}

static Link newThreadTreeNode(MailMessage message){
	assert (message != NULL);
	Link new = malloc (sizeof (ThreadTreeNode));
	assert (new != NULL);
	new->mesg = message;
	new->replies = new->next = NULL;
	return new;
	}

static Link SearchThreadTree(Link root, Link new){
	if (root == NULL){
		return NULL;
	}

	char* id_root = MailMessageID(root->mesg);
	char* id_new_reply = MailMessageRepliesTo(new->mesg);

	// if there is no reply
	if (id_new_reply == NULL){
		return root;

	// if there is a reply
	} else if (strcmp(id_root, id_new_reply) == 0){
		return root;
	}

	// transverse down reply nodes
	if (root->replies != NULL){
		Link result = SearchThreadTree(root->replies, new);

		// checking if result is legit
		if (result != NULL){
			return result;
		}
	}

	// else, transverse down next nodes
	return SearchThreadTree(root->next, new);
}
