// PQ ADT interface for Ass2 (COMP2521)
#include "PQ.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>

//static void swap (ItemPQ* array[], int a, int b);
static void swap (ItemPQ array[], int a, int b);
static void fixUp(ItemPQ array[], int a);

struct PQRep {
	ItemPQ *items; /* array of itemPQ */
	int nitems; /* number of items in array */
	int nslots; /* number of elements in array */
} PQRep;


/* taken from lectre wk6a, made by Ashesh Mahidadia */
PQ newPQ(void){
	struct PQRep *new = malloc (sizeof (PQRep));
	ItemPQ *array = malloc(sizeof(ItemPQ)); /* one item in array */
	assert ((new != NULL) && (array != NULL));
	new->nitems = 0; /* actual spots that are filled out in the array */
	new->nslots = 1; /* one available spot in array */
	new->items = array;
	return new;
}


int PQEmpty(PQ pq) {
	return (pq->nitems == 0);
}


void updatePQ(PQ pq, ItemPQ element) {
	assert(pq != NULL);

	for (int i = 0; i < pq->nitems; i++){
		if (element.key == pq->items[i].key){ /* if keys are the same */
			pq->items[i].value = element.value; /* replacing node with new node i.e. updating value */
			fixUp (pq->items, pq->nitems); /* moving everything to the right place */
		}
	}

}


void addPQ(PQ pq, ItemPQ element) {

	/* case 1: adding first element to PQ */
	if (pq->nitems == 0){ /* if there are no items in array */
		pq->items[0] = element; /* add item */
		pq->nitems++; /* increment item counter */
	}

	else {
		updatePQ(pq, element); /* checking if key is in priority queue and updating */

		/* case 2: default case */
		pq->nslots++; /* incrementing available spots in array */
		pq->items = realloc(pq->items, sizeof(ItemPQ)*(pq->nslots)); /* allocating memory */

		pq->items[pq->nitems] = element; /* inserting new item in pq */
		pq->nitems++; /* incrementing item counter */

		fixUp (pq->items, pq->nitems); /* moving everything to the right place */
	}
}


ItemPQ dequeuePQ(PQ pq) {
	assert(pq != NULL);
	fixUp(pq->items, pq->nitems); /* making sure that everythings in order */

	ItemPQ throwAway = pq->items[0]; /* giving a temp value to throwaway node */
	int position = 0; /* counter to keep track of where ItemPQ throwaway is in the array */

	for (int i = 0; i < pq->nitems; i++){
		ItemPQ new = pq->items[i];
		if (new.value <= throwAway.value){ /* if new value is smaller than the current throwaway value */
			throwAway = new; /* changing throwaway to new itemPQ */
			position = i; /* changing position to new position */
		}
	}

	/* deleting itemPQ  */
	for (int b = position; b < pq->nitems; b++){ /* going to the itemPQ thats about to be deleted */
		pq->items[b] = pq->items[b + 1]; /* rewriting over it with the next itemPQ in the array */
	}

	pq->nitems--; /* decreasing item counter */

	if (pq->nitems != 0){ /* if the array has one or more itemPQ in array */
		pq->nslots--; /* decreasing available slot counter */
		pq->items = realloc(pq->items, sizeof(ItemPQ)*(pq->nslots)); /* modifying array memory to exact remaining PQ */
	}

	fixUp (pq->items, pq->nitems); /* moving everything to the right place */
	return throwAway;
}


void  showPQ(PQ pq) {
	assert (pq != NULL);

	for (int i = 0; i < pq->nitems; i++){
		printf("%d", pq->items[i].value);
		if (i + 1 != pq->nitems) /* if the item is not the last item */
			printf(" > ");
	}
	printf ("\n");
}


void freePQ(PQ pq) {
	assert (pq != NULL);

	free(pq->items);
	free(pq);
}


static void swap (ItemPQ array[], int a, int b){
	ItemPQ temp = array[a];
	array[a] = array[b];
	array[b] = temp;
}


/* using a bubble sort */
static void fixUp(ItemPQ array[], int a){
	for (int i = 0; i < (a - 1); i++){ /* starting at the beginning of the array, time complexity O(n^2) */
		for (int j = 0; j < (a - i - 1); j++){ /* going through the array swapping in pairs */
			if (array[i].key > array[i + 1].key) /* checking which key has higher priority */
				swap(array, i, i + 1); /* swapping array values */
		}
	}
}
