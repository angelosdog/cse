#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
int main(int argc, char* argv[]){
   int cost = 0;
   int highestCost = 0;
   char sign[101];
   char signTemp[101];
   char highestSign[101];
   int numWords = 0;
   int i;
   while (fgets(sign, 100, stdin) && numWords < 100){
      numWords++;
      cost = 0;
      strcpy(signTemp, sign);
      for(i=0;sign[i]!='\0' && sign[i] != '\n';i++){
         if(isupper(sign[i])){
             sign[i] -= 64;
             cost += sign[i];
         } else if(islower(sign[i])){
             sign[i] -= 96;
             cost += sign[i];
         } else if (isdigit(sign[i])){
             cost += 5;
        } else if (isprint(sign[i])) {
             cost += 2;
        }
      }
      if (cost > highestCost){
         highestCost = cost;
         strcpy(highestSign, signTemp);
      }
   }
   i = 0;
   while (highestSign[i] != '\n'){
      i++;
   }
   highestSign[i] = '\0';
   printf("Dearest sign is %s with cost $%d\n", highestSign, highestCost);
   return 0;
}
