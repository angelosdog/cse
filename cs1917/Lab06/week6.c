// Program to read in from console and print out palindromes
// Week 6 prac exam
// Created by Vincent Tran on 17/4/2012

#include <stdio.h>
#include <string.h>
int main(int argc, char* argv[]){
	int i;
	int medianOfWord;
	char word[27];
	while (fgets(word,sizeof(word),stdin)!=NULL) { 
    	word[strlen(word)-1] = '\0';
		medianOfWord = strlen(word)/2;
	//	printf("word = %s\nmedian = %d\n", word, medianOfWord);
		i = 0;
    	if(word[1] == '\0'){
			printf("%s\n",word);
		} else {
			while (i < medianOfWord && word[i] == word[strlen(word) - 1 - i]){
				i++;
			}
			if (i == medianOfWord){
				printf("%s\n", word);
			}
		}
	}	
	return 0;
}
