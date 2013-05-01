#ifndef GRAPH_H
#define GRAPH_H

#include "bool.h"
typedef struct {
    int v;
    int w;
} Edge;

Edge EDGE (int, int);

typedef struct graph *Graph;

Graph GRAPHinit (int);

void GRAPHinsertE (Graph, Edge);

void GRAPHremoveE (Graph, Edge);

int GRAPHedges (Edge [], Graph G);
//return the number of edges

Graph GRAPHcopy (Graph);

void GRAPHdestroy (Graph);

bool GRAPHedgeScan (Edge *);
// return true if succesful, false otherwise

void GRAPHEdgePrint (Edge);

bool GRAPHpath (Graph g, int v, int w);

#define maxV 100

#endif // GRAPH_H
