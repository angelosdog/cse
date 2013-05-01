#include <stdio.h>

int main(int argc, char* argv[]){
	int sideLength;
	int i,j;
	scanf("%d", &sideLength);
	sideLength -= 1;

	for(i=0; i <= sideLength; i++){
		for(j=0; j <= sideLength; j++){
			if (j == i || j == sideLength - i){
				printf("#");
			} else {
				printf("-");
			}
		}
		printf("\n");
	}

	return 0;
}
