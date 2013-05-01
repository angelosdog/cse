#include <stdlib.h>
#include <stdio.h>
#include "RB.h"

struct _RBnode
{ 
    int key; 
    RBTree left, right; 
    int red; 	//link to this node is red
} RBnode;

static RBTree new(int key, RBTree l, RBTree r, int colour); 

static RBTree emptyTree ();

static RBTree emptyTree () {
    // This will allocate space for the dummy node and set emptyT to the dummy 
    // node the first time this function is called. Every call after that will 
    // just return the pointer to the dummy node
    static RBTree emptyT = NULL;
    if (emptyT == NULL) 
        emptyT = new(-1, NULL, NULL, 0);
    return emptyT;
}

static RBTree new(int key, RBTree l, RBTree r, int colour)
{ 
    RBTree newRBTree = malloc(sizeof *newRBTree); 
    newRBTree -> key  = key; 
    newRBTree -> left  = l; 
    newRBTree -> right = r;
    newRBTree -> red = colour;
    return newRBTree;
}



RBTree rotateR(RBTree currentTree)
{
    RBTree rotRTree = currentTree -> left;
    currentTree -> left = rotRTree -> right;
    rotRTree -> right = currentTree;
    return rotRTree;
}

RBTree rotateL(RBTree currentTree)
{
    RBTree rotLTree = currentTree -> right;
    currentTree -> right = rotLTree -> left;
    rotLTree -> left = currentTree;
    return rotLTree;
}

RBTree RBinsertRec(RBTree currTree, int key, int inRightSubtree)
{
    if (currTree == emptyTree())
        // always part of existing node if not root, therefore we set red = TRUE
        return new(key, emptyTree(), emptyTree(), TRUE);  

    // node is a 4-node, lift it
    if ((currTree->left->red) && (currTree->right->red))
    { 
        currTree->red = TRUE; 
        currTree->left->red = FALSE; 
        currTree->right->red = FALSE; 
    }

    if (key < currTree->key) // insert into the left subtree
    { 
        currTree->left = RBinsertRec(currTree->left, key, FALSE); 
        if (currTree->red && currTree->left->red && inRightSubtree)
            currTree = rotateR(currTree); 

        if (currTree->left->red && currTree->left->left->red)
        { 
            currTree = rotateR(currTree); 
            currTree->red = FALSE; 
            currTree->right->red = TRUE; 
        }
    }
    else // insert into right subtree .......
    {
        currTree->right = RBinsertRec(currTree->right, key, TRUE); 
        if (currTree->red && currTree->right->red && !inRightSubtree)
            currTree = rotateL(currTree); 

        if (currTree->right->red && currTree->right->right->red)
        { 
            currTree = rotateL(currTree); 
            currTree->red = FALSE; 
            currTree->left->red = TRUE; 
        }
    }
    return currTree;
}

RBTree RBinsert(RBTree tree, int item)
{ 
    tree = RBinsertRec(tree, item, FALSE); 
    tree->red = FALSE;
    return tree;
}

void RBprintRec (RBTree tree, int level, int side)
{
    int i;
    if (tree == emptyTree())
        return;

    printf("\n");
    for (i = level; i != 0; i--)
        printf("  ");

    printf("%d", tree -> key);
    if (tree->red)
        printf(" *");
    RBprintRec (tree -> left, level+1, 0);
    RBprintRec(tree -> right, level+1, 1);
}

void RBprint (RBTree tree) 
{
    RBprintRec (tree, 0, 0);
}

RBTree RBemptyTree () {
    return emptyTree ();
}

// return the height of the corresponding 2-3-4 tree
int RBheight (RBTree tree) {
    int height = 0;
    while(tree != RBemptyTree()){
        if(!(tree->red) && tree != RBemptyTree()){
            height++;
        }
        tree = tree->left;
    }
    return height;
}

bool RBBalanced(RBTree tree){
    if(tree == RBemptyTree()){
        return TRUE;
    }
    if(tree->red && (tree->left->red || tree->right->red)){
        return FALSE;
    }
    return RBheight(tree->left) == RBheight(tree->right) && RBBalanced(tree->left) && RBBalanced(tree->right);
    
}

// check if tree is balanced
bool RBisBalanced (RBTree tree) {
   if(tree->red){
       return FALSE;
   }   
   return RBBalanced(tree);
}


