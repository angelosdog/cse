#include <stdlib.h>

int replace_val(int *p, int n, int old_val, int new_val){
    int replaceIndex;
    int i = 0;
    int replaced = 0;
    while (i < n && replaced == 0){
        if (p[i] == old_val){
            p[i] = new_val;
            replaced = 1;
        }
    }
    if (i == n){
        replaceIndex = -1;
    } else {
        replaceIndex = i;
    }
    return replaceIndex;
}
