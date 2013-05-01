#include <stdio.h>

int main(){
  char string[11];
  fgets(string, 11, stdin);
  printf("%s\n",string);
  return 0;
}
