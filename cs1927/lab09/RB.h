#define TRUE	1
#define FALSE	0

typedef int bool;

typedef struct _RBnode *RBTree;

RBTree RBemptyTree ();

void RBprint (RBTree tree);

void RBprintRec (RBTree tree, int level, int side);

RBTree RBinsert(RBTree tree, int item);

// return the height of the corresponding 2-3-4 tree
int RBheight (RBTree tree);

// check if tree is balanced
bool RBisBalanced (RBTree tree);
