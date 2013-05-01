#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "Tree.h"

struct treeNode {
    Item item;
    treeLink left, right;
};

//Fix this function so it counts the number of leaves on the tree (in total)
//Note that a leaf is any node on the tree with ZERO children
//this function should return 0 if tree is NULL
int count (treeLink tree) {
    if(tree == NULL){
        return 0;
    }
    if(tree->left == NULL && tree->right == NULL){
        return 1;
    }
    return count(tree->left) + count(tree->right);
}

//Creates a new tree with root a
//Already written for you, see tree.c
treeLink newTree(Item a) {
    treeLink new = malloc (sizeof(struct treeNode));
    new ->item = a;
    new->left = NULL;
    new->right = NULL;
    return new;
}

//adds a node to the tree in no particularly defined way
//Already written for you, see tree.c
treeLink addToTree(treeLink t, Item a) {
    if (t == NULL) {
        t = newTree(a);
    } else {
        if (t->item < a) {
            t->right = addToTree(t->right, a);
        } else {
            t->left = addToTree(t->left, a);
        }
    }
    return t;
}

//frees memory and returns NULL if successful
//Already written for you, see tree.c
treeLink deleteTree(treeLink t) {
    if (t != NULL) {
        t->right = deleteTree(t->right);
        t->left = deleteTree(t->left);
        free(t);
    }
    return NULL;
}

static void printTreeA(treeLink tree, int height) {
    if (tree != NULL) {
        printTreeA(tree->left, height +1);
        int i;
        for (i = 0; i < height; i++){
            printf("   ");
        }
        printf("%d\n", tree->item);
        printTreeA(tree->right, height + 1);
    }
}

void printTree(treeLink tree) {
    printTreeA(tree, 0);
}
