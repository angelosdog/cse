#include <stdlib.h>
#include <stdio.h>
#include "RB.h"

int main(int argc, const char * argv[])
{
	int test[] = {22, 12, 8, 15, 11, 19, 43, 44, 45, 42, 41, 40, 39};
        int i;
	RBTree rbt = RBemptyTree();
	
	for (i = 0; i < (sizeof test)/(sizeof (int)); i++)
	{
	  rbt = RBinsert(rbt, test[i]);
	  //printf("(%4d) Insert %4d: ", i, test[i]);
	}
    RBprint (rbt);

    printf("\n");
    printf("Height = %d\n", RBheight(rbt));
    printf("Isbalanced = %d\n", RBisBalanced(rbt));
	return 0;
}
