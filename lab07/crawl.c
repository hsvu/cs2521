// crawl.c ... build a graph of part of the web
// Written by John Shepherd, September 2015
// Uses the cURL library and functions by Vincent Sanders
// <vince@kyllikki.org>

#include <ctype.h>
#include <curl/curl.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <unistd.h>

#include "graph.h"
#include "html.h"
#include "set.h"
#include "stack.h"
#include "queue.h"
#include "url_file.h"

#define BUFSIZE 1024

static void setFirstURL (char *, char *);
// static bool checkQueueEmpty (Queue q);
// static bool maxGraph (Graph g);

int main (int argc, char **argv)
{
	URL_FILE *handle;
	char buffer[BUFSIZE];
	char baseURL[BUFSIZE];
	char firstURL[BUFSIZE];
	char next[BUFSIZE];
	int maxURLs;

	if (argc > 2) {
		strcpy (baseURL, argv[1]);
		setFirstURL (baseURL, firstURL);
		maxURLs = atoi (argv[2]);
		if (maxURLs < 40)
			maxURLs = 40;
	} else {
		fprintf (stderr, "Usage: %s BaseURL MaxURLs\n", argv[0]);
		exit (1);
	}

	// You need to modify the code below to implement:
	//
		// add firstURL to the ToDo list
		// initialise Graph to hold up to maxURLs
		// initialise set of Seen URLs
	// while (ToDo list not empty and Graph not filled) {
	//    grab Next URL from ToDo list
	//    if (not allowed) continue
	//    foreach line in the opened URL {
	//       foreach URL on that line {
	//          if (Graph not filled or both URLs in Graph)
	//             add an edge from Next to this URL
	//          if (this URL not Seen already) {
	//             add it to the Seen set
	//             add it to the ToDo list
	//          }
	//       }
	//    }
	//    close the opened URL
	//    sleep(1)
	// }
	if (!(handle = url_fopen (firstURL, "r"))) {
		fprintf (stderr, "Couldn't open %s\n", next);
		exit (1);
	}

	Queue q = newQueue();			// ToDo list of URLs
	enterQueue(q, firstURL);	// add firstURL to the ToDo list

	Set seen_set = newSet();				// initialise set of Seen URLs

	Graph graph = newGraph((size_t)maxURLs);	//initialise Graph to hold up to maxURLs

	while (!emptyQueue(q) && (nVertices(graph) < maxURLs)) {
		char* currURL = leaveQueue(q);
		//opening top of queue url
		handle = url_fopen(currURL, "r");

		//adding to seen set
		insertInto(seen_set, currURL);
		//showSet(seen_set);
		//showQueue(q);

		// read webpage line-by-line into a buffer
		while (!url_feof (handle)){
			url_fgets (buffer, sizeof (buffer), handle);

			// collect every URL from the line
			int pos = 0;
			char result[BUFSIZE];
			memset (result, 0, BUFSIZE);
			while ((pos = GetNextURL(buffer, currURL, result, pos)) > 0) {
				//if (Graph not filled or both URLs in Graph)
				if ((nVertices(graph) < maxURLs) || !(isConnected(graph, currURL, result))){
					addEdge(graph, currURL, result);
				}

				if (!isElem(seen_set, result)){
					printf ("Found: '%s'\n", result);
					insertInto(seen_set, result);
					enterQueue(q, result);
				}
				memset (result, 0, BUFSIZE);
			}
		}
		url_fclose(handle);
		sleep(1);
		free(currURL);
	}

	showGraph(graph, 1);
	showGraph(graph, 0);
	dropSet(seen_set);
	dropQueue(q);
	dropGraph(graph);
	return 0;
}

// setFirstURL(Base,First)
// - sets a "normalised" version of Base as First
// - modifies Base to a "normalised" version of itself
static void setFirstURL (char *base, char *first)
{
	char *c;
	if ((c = strstr (base, "/index.html")) != NULL) {
		strcpy (first, base);
		*c = '\0';
	} else if (base[strlen (base) - 1] == '/') {
		strcpy (first, base);
		strcat (first, "index.html");
		base[strlen (base) - 1] = '\0';
	} else {
		strcpy (first, base);
		strcat (first, "/index.html");
	}
}

// static bool isInGraph (Graph g, char *url){
// 	assert (g != NULL);
// 	for (char* link = g->)
// }
