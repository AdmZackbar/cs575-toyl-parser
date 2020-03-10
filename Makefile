CC = gcc
OBJ = lexer.o types.o parser.o

all : parser

types.o : types.c types.h
	$(CC) -g -c types.c

lexer.o : lexer.c types.h
	$(CC) -g -c lexer.c

parser.o : parser.c lexer.h types.h
	$(CC) -g -c parser.c

parser : $(OBJ)
	$(CC) $(OBJ) -o parser

clean :
	rm -f $(OBJ) parser
