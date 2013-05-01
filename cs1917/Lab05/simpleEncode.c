/*
* simpleEncode.c
*
* reads in a permutation of the alphabet then encodes
* lower case letters using that permutation
* lab 5 template code- might need some cleaning up...
*
* Created by Julian Saknussemm on 01/04/11.
* Copyright 2011 Licensed under Creative Commons SA-BY-NC 3.0.
*
*/
     
#include <stdio.h>
#include <stdin.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
     
#define STOP -1
#define ALPHABET_SIZE 26
     
char encode (int plainChar, char *permuation);
void testEncode (void);
     
int main (int argc, char *argv[]) {
	testEncode();
    char permutation[ALPHABET_SIZE+1];
    fgets(permutation, APLHABET_SIZE+1, stdin);
    // getchar() reads and returns one character from the keyboard
    // returns -1 when it reads end-of-file character (^D in linux)
    int plainCharacter = getchar();
    while (plainCharacter != STOP) {
  		int encodedCharacter = encode (plainCharacter, permutation);
    	printf ("%c", encodedCharacter);
    	plainCharacter = getchar();
    }
    
	return EXIT_SUCCESS;
}
     
void testEncode (void) {
    assert (encode ('A',"abcdefghijklmnopqrstuvwxyz") == 'A');
    assert (encode ('?',"abcdefghijklmnopqrstuvwxyz") == '?');
    assert (encode (' ',"abcdefghijklmnopqrstuvwxyz") == ' ');
    assert (encode ('\n',"abcdefghijklmnopqrstuvwxyz") == '\n');
    assert (encode ('a',"abcdefghijklmnopqrstuvwxyz") == 'a');
    assert (encode ('m',"abcdefghijklmnopqrstuvwxyz") == 'm');
    assert (encode ('z',"abcdefghijklmnopqrstuvwxyz") == 'z');
    assert (encode ('a',"bcdefghijklmnopqrstuvwxyza") == 'b');
    assert (encode ('m',"bcdefghijklmnopqrstuvwxyza") == 'n');
    assert (encode ('z',"bcdefghijklmnopqrstuvwxyza") == 'a');
     
    assert (encode ('a',"qwertyuiopasdfghjklzxcvbnm") == 'q');
    assert (encode ('b',"qwertyuiopasdfghjklzxcvbnm") == 'w');
    assert (encode ('z',"qwertyuiopasdfghjklzxcvbnm") == 'm');
}
     
char encode (int plainChar, char *permutation) {
	char returnChar;
	int i = 0;
	int j = -1;
	char alphabet[ALPHABET_SIZE+1] = "abcdefghijklmnopqrstuvwxyz";

	while (i < strlen(alphabet)) {
		if (alphabet[i] == plainChar){
			j = i;
		}
	}
	if (j != -1){
		returnChar = permutation[j];
	} else {
		returnChar = plainChar;
	}

	return returnChar;
}
