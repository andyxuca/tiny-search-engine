/*
* word.h     Andy Xu     May 2, 2023
*
* Description: This file contains the word module.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/*
* normalizeWord() - see word.h for description
*/
char* 
normalizeWord(char* word)
{
    if (word != NULL) {
        for (int i = 0; word[i]; i++) {
            word[i] = tolower(word[i]);
        }
    }   

    return word;
}
