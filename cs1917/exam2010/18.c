#include <stdio.h>

int main(int argc, char* argv[]){
    int days = 0;
    int total = 0;
    int wetDays = 0;
    int rainfall;
    while(scanf("%d", &rainfall) == 1){
        days++;
        total += rainfall;
        if (rainfall > 5){
            wetDays++;
        }
    }
    float average = total/days;
    printf("Days: %d Total rainfall: %dmm Average rainfall: %2fmm, Wet days: %d\n", days, total, average, wetDays);
    return 0;
}
