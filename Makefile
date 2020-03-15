CC = gcc
OBJ = lexer.o types.o parser.o

.PHONY: all clean test
all : parser

types.o : types.c types.h
	$(CC) -g -c types.c

lexer.o : lexer.c types.h
	$(CC) -g -c lexer.c

parser.o : parser.c lexer.h types.h
	$(CC) -g -c parser.c

parser : $(OBJ)
	$(CC) $(OBJ) -o parser

test : parser
	@echo "prog1 output:" > output.txt
	./parser ./ToyL\ Examples/prog1.ToyL >> output.txt
	@echo "prog2 output:" >> output.txt
	./parser ./ToyL\ Examples/prog2.ToyL >> output.txt
	@echo "prog3 output:" >> output.txt
	./parser ./ToyL\ Examples/prog3.ToyL >> output.txt
	@echo "prog4 output:" >> output.txt
	./parser ./ToyL\ Examples/prog4.ToyL >> output.txt
	@echo "prog5 output:" >> output.txt
	./parser ./ToyL\ Examples/prog5.ToyL >> output.txt
	@echo "prog6 output:" >> output.txt
	./parser ./ToyL\ Examples/prog6.ToyL >> output.txt
	@echo "prog7 output:" >> output.txt
	./parser ./ToyL\ Examples/prog7.ToyL >> output.txt
	@echo "prog8 output:" >> output.txt
	./parser ./ToyL\ Examples/prog8.ToyL >> output.txt
	@echo "prog9 output:" >> output.txt
	./parser ./ToyL\ Examples/prog9.ToyL >> output.txt
	@echo "prog10 output:" >> output.txt
	./parser ./ToyL\ Examples/prog10.ToyL >> output.txt
	@echo "prog11 output:" >> output.txt
	./parser ./ToyL\ Examples/prog11.ToyL >> output.txt
	@echo "prog12 output:" >> output.txt
	./parser ./ToyL\ Examples/prog12.ToyL >> output.txt
	@echo "prog13 output:" >> output.txt
	./parser ./ToyL\ Examples/prog13.ToyL >> output.txt
	@echo "prog14 output:" >> output.txt
	./parser ./ToyL\ Examples/prog14.ToyL >> output.txt
	@echo "prog15 output:" >> output.txt
	./parser ./ToyL\ Examples/prog15.ToyL >> output.txt

clean :
	rm -f $(OBJ) parser output.txt
