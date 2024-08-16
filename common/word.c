#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "word.h"

char* 
word_normalize(char* word){
    for (int i = 0; word[i] != '\0'; i++){
        word[i] = tolower(word[i]);
    }
    return word;
}
