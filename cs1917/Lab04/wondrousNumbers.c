#include <stdio.h>
int wondrous (int start);


int main(int argc, char* argv[]){
    int number;
    scanf("%d", &number);
    printf("%d\n", wondrous(number));
    return 0;
}

int wondrous(int start){
    int counter = 0;
    int number = start;
    if (number == 1){
	printf("1\n");
	counter++;
    } else {
        while (number != 1){
            if (number % 2 == 0){
	        number /= 2;
            } else {
	        number = number*3 + 1;
	    }
	
	    counter++;
	    if (number != 1){
	        printf("%d ", number);
	    } else {
	        printf("%d\n", number);
	    }
        }
    }
    return counter;
}
