Purpose:
This ToyL parser runs lexical and syntactic analysis on given ToyL programs, printing out the tree until completion or on error. The ToyL grammar that was given has been slightly modified (see grammar.txt for modified version) to fit LR1 parsing standards.

Building:
Simply use the command 'make' to build the parser (or alternatively, 'make parser').

Usage:
Run the executable ('parser') with the filename of the program to be parsed as the first and only command line argument.
E.G. ./parser example.ToyL
By default all output goes to stdout; to get output in a textfile, use pipes.
E.G. ./parser example.ToyL > output.txt

Cleaning:
Use the command 'make clean' to remove all built files (executable, object files)

Notes:
Given test cases are in the directory 'Examples'
My created test cases are in the directory 'Tests'
Correct output files are in the directory 'Output'
'make test' does not work well in its current state, one should avoid using it right now.
