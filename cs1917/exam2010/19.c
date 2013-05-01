#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[]){
   char string1[101];
   char string2[101];
   char stringTemp1[101];
   char stringTemp2[101]; 
   printf("Enter string1: ");
   fgets(string1, 100, stdin);
   printf("\n");
   printf("Enter string2: ");
   fgets(string2, 100, stdin);
   printf("\n");
   strcpy(stringTemp1, string1);
   strcpy(stringTemp2, string2);
   int isAnagram = 0;
   int stringLength;
   int i,j;
   if (strlen(string1) == strlen(string2)){
      stringLength = strlen(string1);
      for(i=0;i<stringLength;i++){
         j=0;
         while(j<=stringLength && string1[i] != string2[j]){
            j++;
         }
         if(j < stringLength){
            string2[j] = ' ';
         }
         string1[i] = ' ';
      }
      i = 0;
      while (i < stringLength && string2[i] == ' '){
         i++;
      }
      if (i < stringLength){
         isAnagram = 0;
      } else {
         isAnagram = 1;
      }
   
   } else {
       isAnagram = 0;
   }
   strcpy(string1, stringTemp1);
   strcpy(string2, stringTemp2);
   if (isAnagram == 0){
      printf("\"%s\" is not an anagram of \"%s\"\n", string1, string2);
   } else {
      printf("\"%s\" is an anagram of \"%s\"\n", string1, string2);
   }
   return 0;
}
