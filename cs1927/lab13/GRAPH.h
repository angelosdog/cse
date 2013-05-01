
typedef struct { 
  int v; 
  int w; 
  int weight;
} Edge;

Edge EDGE (int, int, int);

typedef struct graph *Graph;

Graph GRAPHinit (int);
 void GRAPHinsertE (Graph, Edge);
 void GRAPHremoveE (Graph, Edge);
  int GRAPHedges (Edge [], Graph g);
Graph GRAPHcopy (Graph);
 void GRAPHdestroy (Graph);
int GRAPHedgeScan (Edge *);
void GRAPHEdgePrint (Edge);
int GRAPHsearch (Graph, int[]);
Graph GRAPHmst (Graph);
Graph GRAPHmstPrim (Graph);
#define maxV 8
