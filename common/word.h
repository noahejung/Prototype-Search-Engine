#ifndef WORD_H
#define WORD_H

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>


/**************** word_normalize ****************/
/* Reads string and returns it in lowercase 
* Caller provides:
*   String of characters
* We guarantee:
*   string is returned in lowercase
*/
char* word_normalize(char* word);

#endif // WORD_H