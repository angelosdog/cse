#include <stdio.h>
#include <stdlib.h>
#include "GRAPH.h"

//u might want to keep this
#define TRUE (1==1)
#define FALSE (1==0)

#define INIT -1
Edge EDGE (int v, int w) {
    Edge e = {v, w};
    return e;
}

struct graph {
    int V;
    int E;
    int **adj;
};

int depthFirstSearch (Graph g, int currVert, int searchVert, int *seen){
    //printf("curr at %d, looking for %d\n", currVert, searchVert);
    if(currVert == searchVert) return TRUE;
    int i;
    int loop = 1;
    int found = FALSE;
    if(seen[currVert] == 0){
        loop = 0;
    }
    if(loop == 1) {
        //printf("LOOP DETECTED at vertex %d\n",currVert);
        return FALSE;
    }
    seen[currVert] = 1;
    for(i=0; i < g->V; i++){
        if(g->adj[currVert][i] > 0 && found != TRUE && currVert != i){
            //printf("Travelling to %d\n", i);
            found = depthFirstSearch(g,i,searchVert,seen);
        }
    }

    return found;
}

Graph GRAPHinit (int V) {
    int i, j;
    Graph g = malloc (sizeof (*g));
    g->V = V;
    g->E = 0;
    // allocate space for V pointer
    g->adj = malloc (V * sizeof (*(g->adj)));
    for (i=0; i < V; i++) {
        // allocate space for V edges
        g->adj[i] = malloc (V * sizeof (*(g->adj[i])));
        for (j = 0; j < V; j++) {
            g->adj[i][j] = 0;
        }
    }
    return g;
}

void GRAPHinsertE (Graph g, Edge e){
    if (g->adj[e.v][e.w] == 0) {
        g->E++;
        g->adj[e.v][e.w] = 1;
        g->adj[e.w][e.v] = 1;
    }
}


void GRAPHremoveE (Graph g , Edge e){
    if (g->adj[e.v][e.w] == 1) {
        g->E--;
        g->adj[e.v][e.w] = 0;
        g->adj[e.w][e.v] = 0;
    }
}


int GRAPHedges (Edge edges[], Graph g) {
    int i,j, noOfEdges;
    noOfEdges = 0;
    for (i = 1; i < g->V; i++) {
        for (j = 0; j < i; j++) {
            if (g->adj[i][j] == 1) {
                edges[noOfEdges].v = i;
                edges[noOfEdges].w = j;
                noOfEdges++;
            }
        }
    }
    return noOfEdges;
}


Graph GRAPHcopy (Graph g) {
    return g;
}

void GRAPHdestroy (Graph g) {
}


int GRAPHedgeScan (Edge *edge) {
    if (edge == NULL) {
        printf ("GRAPHedgeScan: called with NULL \n");
        abort();
    }
    if ((scanf ("%d", &(edge->v)) == 1) &&
            (scanf ("%d", &(edge->w)) == 1)) {
        return 1;
    } else {
        return 0;
    }
}


void GRAPHEdgePrint (Edge edge) {
    printf ("%d -- %d", edge.v, edge.w);
}

//which returns TRUE if the two vertices v and w are connected in g.
//For this, you can define an auxilliary function which gets as additional parameter an array of boolean values
//with one entry for each vertex in the graph, which keeps track of all the vertices you already visited
//to make sure you don't run in circles.

bool GRAPHpath (Graph g, int v, int w) {
    int seen[g->V];
    int i;
    for (i=0;i < g->V;i++){
        seen[i] = 0;
    }
    if(depthFirstSearch(g,v,w,seen)) return TRUE;
    return FALSE;
}
