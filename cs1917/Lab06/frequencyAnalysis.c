#include <string.h>
#include <stdio.h>
#include <ctype.h>

int main(int argc, char* argv[]){
	int numberOfChars = 0;
	int letters[26];
	char c;
	int i;
	
	for(i = 0; i < 26; i++){
		letters[i] = 0;
	}
	c = getchar();

	while(c != EOF){
		if(isalpha(c)){
			c = tolower(c);
			c = c - 'a';
			letters[(int)c]++;
			numberOfChars++;
		}
		c = getchar();
	}
	
	for(i = 0; i < 26; i++){
		printf("%c: %f%%\n", i + 'a', letters[i]/(float)numberOfChars*100);
	}
	return 0;
}
