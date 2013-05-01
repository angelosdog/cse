#include <stdio.h>

int main(int argc, char* argv[]){
    int n;
    int i;
    int sum = 0;
    scanf("%d", &n);
    for(i = 1; i <= n, i++){
        sum += i;
    }
    printf("%d", sum);
    return 0;
}
