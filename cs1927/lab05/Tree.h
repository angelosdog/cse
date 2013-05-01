typedef int Item;
typedef struct treeNode *treeLink;

int count (treeLink tree);

//Creates a new tree with root a
//Already written for you, see tree.c
treeLink newTree(Item a);

//adds a node to the tree in no particularly defined way
//Already written for you, see tree.c
treeLink addToTree(treeLink t, Item a);

//frees memory and returns NULL if successful
//Already written for you, see tree.c
treeLink deleteTree(treeLink t);

void printTree(treeLink tree);
