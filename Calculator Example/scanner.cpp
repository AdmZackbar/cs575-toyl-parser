#include <iostream>
#include <cctype>
using namespace std;

enum Token {EndFile,Plus,Minus,Times,Divide,LeftPar,RightPar,Number};

string spelling[]={"EOF", "+", "-", "*", "/", "(", ")", "<number>"};

int value;

int scanner() {
        char next;
        next=cin.get();
        while (!cin.eof()) {
                switch (next) {
                        case ' ': case '\t': case '\r': case '\n':
                                next=cin.get();
                                break;
                        case '+':
                                return Plus;
                        case '-':
                                return Minus;
                        case '*':
                                return Times;
                        case '/':
                                return Divide;
                        case '(':
                                return LeftPar;
                        case ')':
                                return RightPar;
                        default:
                                if (isdigit(next)) {
                                        value=0;
                                        do {
                                                value=10*value+next-'0';
                                                next=cin.get();
                                        }
                                        while (isdigit(next));
                                        cin.putback(next);
                                        return Number;
                                }
                                else {
                                        cerr << "ERROR AT CHARACTER " << next << endl;
                                        next=cin.get();
                                }
                }
        }
        return EndFile;
}

// main() is used to test scanner(), but is not part of final parser

int main () {
        int token;
        do {
                token=scanner();
                cout << spelling[token];
                if (token==Number) cout << '\t' << value;
                cout << endl;
        }
        while (token!=EndFile);
        return 0;
}
