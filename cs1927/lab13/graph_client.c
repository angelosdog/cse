#include <stdlib.h>
#include <stdio.h>
#include "GRAPH.h"


// call with graph_e1.txt as input, for example.
//
int main (int argc, char *argv[]) { 

  Edge e, *edges;
  Graph g, mst;
  int graphSize, i, noOfEdges;

  if (argc < 2) {
    printf ("No size provided - setting max. no of vertices to %d\n", maxV);
    graphSize = maxV;
  } else  { 
    graphSize = atoi (argv[1]);
  }
  g =   GRAPHinit (graphSize);    

  printf ("Reading graph edges (format: v w weight) from stdin\n");
  while (GRAPHedgeScan (&e)) {
    GRAPHinsertE (g, e);
  }
  
  edges = malloc (sizeof (*edges) * graphSize * graphSize);
  noOfEdges = GRAPHedges (edges, g);
  printf ("Edges of the graph:\n");
  for (i = 0; i < noOfEdges; i++) {
    GRAPHEdgePrint (edges[i]);
    printf ("\n");
  }

  mst = GRAPHmst (g);
  noOfEdges = GRAPHedges (edges, mst);

  printf ("\n MST \n");
  for (i = 0; i < noOfEdges; i++) {
    GRAPHEdgePrint (edges[i]);
    printf ("\n");
  }

  GRAPHdestroy (g);
  GRAPHdestroy (mst);
  free (edges);  
  return EXIT_SUCCESS;
}
