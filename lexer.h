#ifndef __LEXER_INCLUDED__
#define __LEXER_INCLUDED__

#include <stdio.h>

typedef struct lexer LEXER;

extern LEXER *newLEXER(FILE *fp);
extern char *getTokenString(LEXER *l);

extern char *getToken(LEXER *l);

#endif
