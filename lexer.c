#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "types.h"
#include "lexer.h"

#define MAX_RESERVED_LEN 10
#define NUM_RESERVED 28

static void checkComment(FILE *fp);
static void skipBlockComment(FILE *fp);

static void skipWhitespace(FILE *fp)
{
    int ch = fgetc(fp);
    while (ch != EOF && isspace(ch))
    {
        ch = fgetc(fp);
    }
    if (ch != EOF)
        ungetc(ch, fp);
    checkComment(fp);
}

static void checkComment(FILE *fp)
{
    int ch = fgetc(fp);
    
    if (ch == '/')
    {
        ch = fgetc(fp);
        if (ch == '*') // is a block comment
            skipBlockComment(fp);
        else
        {
            // not a comment: need to re-add those characters
            ungetc(ch, fp);
            ungetc('/', fp);
        }
    }
    else if (ch != EOF)
        ungetc(ch, fp);
}

/*
 * Reads in and ignores all characters until the end string is found.
 * The end string is consumed at the end.
 */
static void skipBlockComment(FILE *fp)
{
    int ch = fgetc(fp);
    while(ch != EOF)
    {
        if (ch == '*')
        {
            ch = fgetc(fp);
            if (ch == '/')
                break;
			else if (ch == '*')
				ungetc(ch, fp);
        }
        ch = fgetc(fp);
    }
    skipWhitespace(fp);
}

static int isReserved(char *str, char *reserved)
{
    return !strcmp(str, reserved);
}

static char *checkIdentifier(char *str)
{
    char *reserved[NUM_RESERVED] = {
        PROCEDURE, IS, BEGIN, END, FUNCTION, RETURN,
        IN, OUT, PACKAGE, PRIVATE, TYPE, INTEGER, BOOLEAN,
        ARRAY, OF, RECORD, NULL_WORD, IF, THEN, ELSIF,
        ELSE, WHILE, LOOP, TRUE, FALSE, NOT, LOGICAL_AND, LOGICAL_OR};
    for (int i=0; i<NUM_RESERVED; i++)
    {
        if (isReserved(str, reserved[i]))
            return reserved[i];
    }
    return IDENTIFIER;
}

struct lexer
{
    FILE *fp;
    char *tokenString;
};

LEXER *newLEXER(FILE *fp)
{
    LEXER *l = malloc(sizeof(LEXER));
    assert(l != 0);

    l->fp = fp;
    l->tokenString = NULL;

    return l;
}
char *getTokenString(LEXER *l)
{
    if (l->tokenString)
        return l->tokenString;
    return "";
}

char *getToken(LEXER *l)
{
    skipWhitespace(l->fp);
    int ch = fgetc(l->fp);
    switch (ch)
    {
        case EOF:   return END_OF_FILE;
        case ';':   return SEMICOLON;
        case '(':   return OPAREN;
        case ')':   return CPAREN;
        case '[':   return OBRACKET;
        case ']':   return CBRACKET;
        case ',':   return COMMA;
        case ':':
            ch = fgetc(l->fp);
            if (ch == '=')
                return ASSIGNMENT;
            if (ch != EOF)
                ungetc(ch, l->fp);
            return COLON;
        case '.':
            ch = fgetc(l->fp);
            if (ch == '.')
                return DOUBLE_DOT;
            if (ch != EOF)
                ungetc(ch, l->fp);
            return DOT;
        case '-':   return MINUS;
        case '+':   return PLUS;
        case '*':   return TIMES;
        case '/':   return DIVIDE;
        case '%':   return MODULUS;
        case '=':   return EQUALS;
        case '<':
            ch = fgetc(l->fp);
            if (ch == '=')
                return LESS_THAN_EQUAL;
            if (ch == '>')
                return NOT_EQUALS;
            if (ch != EOF)
                ungetc(ch, l->fp);
            return LESS_THAN;
        case '>':
            ch = fgetc(l->fp);
            if (ch == '=')
                return GREATER_THAN_EQUAL;
            if (ch != EOF)
                ungetc(ch, l->fp);
            return GREATER_THAN;
        default:
            if (isdigit(ch))
            {
                while (isdigit(ch))
                    ch = fgetc(l->fp);
                if (ch != EOF)
                    ungetc(ch, l->fp);
                return NUMBER;
            }
            if (isalpha(ch))
            {
                char *id = malloc(sizeof(char)*(MAX_RESERVED_LEN+1));
                int len = 0;
                while (isalnum(ch) || ch == '_')
                {
                    if (len < MAX_RESERVED_LEN)
                        id[len++] = tolower(ch);
                    else
                        id = NULL;
                    ch = fgetc(l->fp);
                }
                if (ch != EOF)
                    ungetc(ch, l->fp);
                if (id)
                    id[len] = '\0';
                else
                    return IDENTIFIER;
                l->tokenString = id;
                return checkIdentifier(id);
            }
    }
    return ERROR;
}
