#include <stdlib.h>
#include <stdio.h>
#include "GRAPH.h"

#define MAX_VERT 100

int main (int argc, char *argv[]) {

    Edge e, *edges;
    Graph g;
    int graphSize, i, noOfEdges;
    if (argc < 2) {
        printf ("Setting max. no of vertices to %d\n", MAX_VERT);
        graphSize = MAX_VERT;
    } else  {
        graphSize = atoi (argv[1]);
    }
    g =   GRAPHinit (graphSize);

    while (GRAPHedgeScan (&e)) {
        GRAPHinsertE (g, e);
    }

    edges = malloc (sizeof (*edges) * MAX_VERT * MAX_VERT);
    noOfEdges = GRAPHedges (edges, g);
    printf ("Edges of the graph:\n");
    for (i = 0; i < noOfEdges; i++) {
        GRAPHEdgePrint (edges[i]);
        printf ("\n");
    }
    int j;
    for(i=0; i < 7; i++){
        for(j=0;j<7;j++){
            printf("%d connected to %d?\n",i,j);
            if(GRAPHpath(g,i,j)){
                printf("TRUE\n");
            } else {
                printf("FALSE\n");
            }
        }
    }
    return EXIT_SUCCESS;
}
