#include <stdlib.h>
#include <stdio.h>
#include "GRAPH.h"

#define exch(A, B) { Edge t = A; A = B; B = t; } 
#define max(A,B)(A>B?A:B)
#define min(A,B)(A<B?A:B)


typedef struct node *link;
struct node { 
    int v; 
    int weight;
    link next; 
};

struct graph { 
    int V; 
    int E; 
    link *adj; 
};

static void sortEdges (Edge *edges, int noOfEdges);
static void updateConnectedComponent (Graph g, int from, int to, int newVal, int *connectedComponent);

Edge EDGE (int v, int w, int weight) {
    Edge e = {v, w, weight};
    return e;
}

link NEW (int v, int weight, link next) { 
    link x = malloc (sizeof *x);

    x->v = v; 
    x->next = next;     
    x->weight = weight;
    return x;                         
}



Graph GRAPHinit (int V) { 
    int v;
    Graph G = malloc (sizeof *G);

    G->V = V; 
    G->E = 0;

    G->adj = malloc (V * sizeof(link));
    for (v = 0; v < V; v++){
        G->adj[v] = NULL;
    }
    return G;
}

void GRAPHdestroy (Graph g) {
    // not implemented yet
}

void GRAPHinsertE(Graph G, Edge e){ 
    int v = e.v;
    int w = e.w;
    int weight = e.weight;

    G->adj[v] = NEW (w, weight, G->adj[v]);
    G->adj[w] = NEW (v, weight, G->adj[w]); 
    G->E++;
}

void GRAPHremoveE(Graph G, Edge e){ 
    int v = e.v;
    int w = e.w;
    link *curr;

    curr = &G->adj[w]; 
    while (*curr != NULL){
        if ((*curr)->v == v) {
            (*curr) = (*curr)->next;
            G->E--;
            break;
        }
        curr= &((*curr)->next);
    }
    curr = &G->adj[v]; 
    while (*curr != NULL){
        if ((*curr)->v == w) {
            (*curr) = (*curr)->next;
            break;
        }
        curr= &((*curr)->next);

    }
}

int GRAPHedges (Edge edges[], Graph g) {
    int v, E = 0; 
    link t;  

    for (v = 0; v < g->V; v++) {
        for (t = g->adj[v]; t != NULL; t = t->next) {
            if (v < t->v) {
                edges[E++] = EDGE(v, t->v, t->weight); 
            }
        }
    }
    return E;
}



void GRAPHEdgePrint (Edge edge) {
    printf ("%d -- (%d) -- %d", edge.v, edge.weight, edge.w);
}



int GRAPHedgeScan (Edge *edge) {
    if (edge == NULL) {
        printf ("GRAPHedgeScan: called with NULL \n");
        abort();
    }

    if ((scanf ("%d", &(edge->v)) == 1) &&
            (scanf ("%d", &(edge->w)) == 1) &&
            (scanf ("%d", &(edge->weight)) == 1)) {
        return 1;
    } else {
        return 0;
    }
}  

// Update the CC label for all the nodes in the MST reachable through the edge from-to
// Assumes graph is a tree, will not terminate otherwise.
void updateConnectedComponent (Graph g, int from, int to, int newVal, int *connectedComponent) {
    link currLink = g->adj[to];
    connectedComponent[to] = newVal;

    while (currLink != NULL) {
        if (currLink->v != from) {
            updateConnectedComponent (g, to, currLink->v, newVal, connectedComponent);
        }
        currLink = currLink->next;
    }
}

// insertion sort, replace with O(n * lon n) alg to get 
// optimal work complexity for Kruskal
void sortEdges (Edge *edges, int noOfEdges) {
    int i;
    int l = 0;
    int r = noOfEdges-1;

    for (i = r-1; i >= l; i--) {
        int j = i;
        while ((j < r) && (edges[j].weight > edges[j+1].weight)) {
            exch (edges[j], edges[j+1]);
            j++;
        }
    }

}



Graph GRAPHmst (Graph g) {
    Edge *edgesSorted;
    int i;
    int *connectedComponent = malloc (sizeof (int) * g->V);
    int *sizeOfCC = malloc (sizeof (int) * g->V);
    Graph mst = GRAPHinit (g->V);

    edgesSorted = malloc (sizeof (*edgesSorted) * g->E);
    GRAPHedges (edgesSorted, g);
    sortEdges (edgesSorted, g->E);

    // keep track of the connected component each vertex belongs to
    // in the current MST. Initially, MST is empty, so no vertex is
    // in an MST CC, therefore all are set to -1.
    // We also keep track of the size of each CC, so that we're able 
    // to identify the CC with fewer vertices when merging two CCs
    for (i = 0; i < g->V; i++) {
        connectedComponent[i] = -1;
        sizeOfCC[i] = 0;
    }

    int currentEdge = 0; // the shortest edge not yet in the mst
    int mstCnt = 0;      // no of edges currently in the mst
    int v, w;

    // The MST can have at most min (g->E, g->V-1) edges
    while ((currentEdge < g->E) && (mstCnt < g->V)) {
        v = edgesSorted[currentEdge].v;
        w = edgesSorted[currentEdge].w;
        printf ("Looking at Edge ");
        GRAPHEdgePrint (edgesSorted[currentEdge]);
        if ((connectedComponent[v] == -1) ||
                (connectedComponent[w] == -1)) {
            GRAPHinsertE (mst, edgesSorted[currentEdge]);
            mstCnt++;
            if (connectedComponent[v] == connectedComponent[w]) {
                connectedComponent[v] = mstCnt;
                connectedComponent[w] = mstCnt;
                sizeOfCC[mstCnt] = 2;  // initialise a new CC
            } else {
                connectedComponent[v] = max (connectedComponent[w],  connectedComponent[v]);
                connectedComponent[w] = max (connectedComponent[w],  connectedComponent[v]);
                sizeOfCC[connectedComponent[w]]++;
            }
            printf ("  is in MST\n");
        } else if (connectedComponent[v] == connectedComponent[w]) {
            printf ("  is not in MST\n");
        } else {
            printf ("  is in MST, connecting two msts\n");
            GRAPHinsertE (mst, edgesSorted[currentEdge]);
            mstCnt++;
            // update the CC label of all the vertices in the smaller CC
            // (size is only important for performance, not correctness)
            if (sizeOfCC[connectedComponent[w]] > sizeOfCC[connectedComponent[v]]) {
                updateConnectedComponent (mst, v, v, connectedComponent[w], connectedComponent);
                sizeOfCC[connectedComponent[w]] += sizeOfCC[connectedComponent[v]];
            } else {
                updateConnectedComponent (mst, w, w, connectedComponent[v], connectedComponent);
                sizeOfCC[connectedComponent[v]] += sizeOfCC[connectedComponent[w]];
            }
        }
        currentEdge++;
    }
    free (edgesSorted);
    free (connectedComponent);
    free (sizeOfCC);
    return mst;
}
/*
Graph GRAPHmstPrim (Graph g){
    int i;
    int *connectedComponent = malloc (sizeof (int) * g->V);
    int *sizeOfCC = malloc (sizeof (int) * g->V);
    Graph mst = GRAPHinit (g->V);

    edges = malloc (sizeof (*edges) * g->E);
    GRAPHedges (edges, g);
    sortEdges (edges, g->E);

    // keep track of the connected component each vertex belongs to
    // in the current MST. Initially, MST is empty, so no vertex is
    // in an MST CC, therefore all are set to -1.
    // We also keep track of the size of each CC, so that we're able 
    // to identify the CC with fewer vertices when merging two CCs
    for (i = 0; i < g->V; i++) {
        connectedComponent[i] = -1;
        sizeOfCC[i] = 0;
    }

    int currentEdge = 0; // the shortest edge not yet in the mst
    int mstCnt = 0;      // no of edges currently in the mst
    int v, w;

    // The MST can have at most min (g->E, g->V-1) edges
    while ((currentEdge < g->E) && (mstCnt < g->V)) {
        v = edges[currentEdge].v;
        w = edges[currentEdge].w;
        printf ("Looking at Edge ");
        GRAPHEdgePrint (edges[currentEdge]);
        if ((connectedComponent[v] == -1) ||
                (connectedComponent[w] == -1)) {
            GRAPHinsertE (mst, edgesSorted[currentEdge]);
            mstCnt++;
            if (connectedComponent[v] == connectedComponent[w]) {
                connectedComponent[v] = mstCnt;
                connectedComponent[w] = mstCnt;
                sizeOfCC[mstCnt] = 2;  // initialise a new CC
            } else {
                connectedComponent[v] = max (connectedComponent[w],  connectedComponent[v]);
                connectedComponent[w] = max (connectedComponent[w],  connectedComponent[v]);
                sizeOfCC[connectedComponent[w]]++;
            }
            printf ("  is in MST\n");
        } else if (connectedComponent[v] == connectedComponent[w]) {
            printf ("  is not in MST\n");
        } else {
            printf ("  is in MST, connecting two msts\n");
            GRAPHinsertE (mst, edgesSorted[currentEdge]);
            mstCnt++;
            // update the CC label of all the vertices in the smaller CC
            // (size is only important for performance, not correctness)
            if (sizeOfCC[connectedComponent[w]] > sizeOfCC[connectedComponent[v]]) {
                updateConnectedComponent (mst, v, v, connectedComponent[w], connectedComponent);
                sizeOfCC[connectedComponent[w]] += sizeOfCC[connectedComponent[v]];
            } else {
                updateConnectedComponent (mst, w, w, connectedComponent[v], connectedComponent);
                sizeOfCC[connectedComponent[v]] += sizeOfCC[connectedComponent[w]];
            }
        }
        currentEdge++;
    }
    free (edgesSorted);
    free (connectedComponent);
    free (sizeOfCC);
    return mst;

}*/
