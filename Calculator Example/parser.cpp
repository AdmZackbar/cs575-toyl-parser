#include <iostream>
#include <string>
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

int nextToken, depth=0;

const bool traceScanner=false, traceParser=true;

void accept(int expectedToken) {
        if (nextToken==expectedToken) {
                nextToken=scanner();
                if (traceScanner) {
                        cout << spelling[nextToken];
                        if (nextToken==Number) cout  << '\t' << value;
                        cout << endl;
                }
        }
        else
                cerr << "ERROR: TOKEN " << spelling[nextToken] << " FOUND WHERE TOKEN "
                                        << spelling[expectedToken] << " EXPECTED" << endl;
}

void apply(string leftSide, string rightSide) {
        if (traceParser) {
                for (int k=1; k<depth; k++) cout << "  ";
                cout << leftSide << " --> " << rightSide << endl;
        }
}

void Expr();            // Expr --> Term MoreExpr
void MoreExpr();        // MoreExpr --> + Term MoreExpr | - Term MoreExpr | empty
void Term();            // Term --> Factor MoreTerm
void MoreTerm();        // MoreTerm --> * Factor MoreTerm | / Factor MoreTerm | empty
void Factor();          // Factor --> ( Expr ) | number

void Expr() {
        depth++;
        apply ("Expr", "Term MoreExpr");
        Term();
        MoreExpr();
        depth--;
}

void MoreExpr() {
        depth++;
        if (nextToken==Plus) {
                apply ("MoreExpr", "+ Term MoreExpr");
                accept(Plus);
                Term();
                MoreExpr();
        }
        else if (nextToken==Minus) {
                apply ("MoreExpr", "- Term MoreExpr");
                accept(Minus);
                Term();
                MoreExpr();
        }
        else
                apply ("MoreExpr", "empty");
        depth--;
}

void Term() {
        depth++;
        apply ("Term", "Factor MoreTerm");
        Factor();
        MoreTerm();
        depth--;
}

void MoreTerm() {
        depth++;
        if (nextToken==Times) {
                apply ("MoreTerm", "* Factor MoreTerm");
                accept(Times);
                Factor();
                MoreTerm();
        }
        else if (nextToken==Divide) {
                apply ("MoreTerm", "/ Factor MoreTerm");
                accept(Divide);
                Factor();
                MoreTerm();
        }
        else
                apply ("MoreTerm", "empty");
        depth--;
}

void Factor() {
        depth++;
        if (nextToken==LeftPar) {
                apply ("Factor", "( Expr )");
                accept(LeftPar);
                Expr();
                accept(RightPar);
        }
        else if (nextToken==Number) {
                apply ("Factor", "<number>");
                accept(Number);
        }
        depth--;
}

int main() {
        accept(nextToken);
        Expr();
        accept(EndFile);
        return 0;
}
