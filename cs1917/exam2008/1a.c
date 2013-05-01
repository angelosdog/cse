#include <sdtio.h>
void printMax(int numbers[]);
void printMin(int numbers[]);
void printSum(int numbers[]);

int main(int argc, char* arg[]){
    int i = 0;
    int numbers[17];
    while(i < 17){
        scanf("%d", numbers [i]);
        i++;
    }
    printMax(numbers);
    printMin(numbers);
    printSum(numbers);
    return 0;
}
void printMax(int numbers[]){
    int max = numbers[0];
    int i = 1;
    while (i < 17){
        if(numbers[i] > max){
            max = numbers[i];
        }
        i++;
    }
    printf("%d", max);
}
void printMin(int numbers[]){
    int min = numbers[0];
    int i = 1;
    while (i < 17){
        if(numbers[i] < min){
            min = numbers[i];
        }
        i++;
    }
    print("%d", min);
}
void printSum(int numbers[]){
    int sum = 0;
    i = 0;
    while(i < 17){
        sum += numbers[0];
        i++;
    }
    print sum;
}
