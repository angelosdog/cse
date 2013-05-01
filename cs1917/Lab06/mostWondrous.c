#include <stdio.h>
int wondrous (unsigned long long int number, unsigned long long int previousHighestNumber, int previousCounter);


int main(int argc, char* argv[]){
    unsigned long long int number = 1;
    int counter = 0;
    unsigned long long int previousHighestNumber=0;
    int previousCounter = 0;

    while (counter < 1000){
      //  printf("number = %lld\nhighestNumber = %lld\ncounter = %d\npreviousCounter = %d\n", number,previousHighestNumber,counter,previousCounter);
        counter = wondrous(number, previousHighestNumber, previousCounter);
        if (counter >= previousCounter){
            previousCounter = counter;
            previousHighestNumber = number;
            printf("%lld has length %d\n", number, counter);
         }

         number++;
        
    }
    return 0;
}

int wondrous(unsigned long long int number, unsigned long long int previousHighestNumber, int previousCounter){
//    printf("%lld\n", number);
    int counter = 1;
    while (number != 1){
        if (number == previousHighestNumber){
            counter += previousCounter-1;
            number = 1;
        } else if (number % 2 == 0){
	    number /= 2;
	    counter++;
        } else {
	    number = number*3 + 1;
	    counter++;
	}
    }
 //   printf("%d", counter); 
    return counter;
}
