#include <stdio.h>
#include <stdlib.h>
#include "NextThursday.h"


int main(int argc, char* argv[]) {
    int days;
    days = daysToNextThursday (2012, 2, 12);
    printf("There are %d days until the next Thursday\n", days);
    return 0;
}
