#include <stdio.h>
#include <stdlib.h>
#include "lexer.h"
#include "types.h"

FILE *fp;
char *current;

// Normal helper functions
static void advance();
static int check(char *type);
static void match(char *type);
static void parse(char *filename);
static void failParse(char *component);
// Grammar helper functions
static void program();
static int programPending();

int main(int argc, char **argv)
{
    fp = fopen(argv[1], "r");

    char *token = getToken(fp);

    while (token != END_OF_FILE)
    {
        printf("%s\n", token);
        token = getToken(fp);
    }

    return 0;
}

static void advance()
{
    char *prev = current;
    current = getToken(fp);
}
static int check(char *type)
{
    return current == type;
}
static void match(char *type)
{
    if(check(type))
        return advance();
        
    printf("SYNTAX ERROR: expected %s got %s", type, current);
    printf("Illegal\n");
    exit(-2);
}
static void parse(char *filename)
{
    // Initialize globals
    current = getToken(fp);
    
    // Highest level of program
    program();
    
    // Check if everything has been properly parsed
    match(END_OF_FILE);
}
static void failParse(char *component)
{
    printf("Incorrect structure in grammar module %s\n", component);
    printf("Illegal\n");
    exit(-404);
}

// GRAMMAR PARSING FUNCTIONS
static void program()
{
    // TODO
}
