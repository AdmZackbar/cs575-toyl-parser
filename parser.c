#include <stdio.h>
#include <stdlib.h>
#include "lexer.h"
#include "types.h"

LEXER *l;
char *current;
int depth = 0;

// Normal helper functions
static void advance();
static int check(char *type);
static void match(char *type);
static void parse(char *filename);
static void failParse(char *component);
static void printInfo(char *symbol, char *definition);
// Grammar parse functions
static void program();
static void procedure();
static void procedureEnd();
static int procedurePending();
static void function();
static void functionEnd();
static int functionPending();
static void optFormalList();
static void formalList();
static void formal();
static void idList();
static int idListPending();
static void optMode();
static void declList();
static void declaration();
static int declarationPending();
static void package();
static int packagePending();
static void typeDecl();
static int typeDeclPending();
static void typeDesc();
static int typeDescPending();
static void varDeclList();
static void varDecl();
static int varDeclPending();
static void typeID();
static int typeIDPending();
static void stmtList();
static void statement();
static int statementPending();
static void optElsifList();
static void elsifList();
static void elsif();
static int elsifPending();
static void optElse();
static void rExprList();
static void lExpr();
static int lExprPending();
static void lExpr2();
static int lExpr2Pending();
static void rExpr();
static int rExprPending();
static void rExpr2();
static void rExpr3();
static void rExpr4();
static void rExpr5();
static void rExpr6();
static void rExpr7();
static void unary();
static int unaryPending();
static void optRExprElsifList();
static void rExprElsifList();
static void rExprElsif();
static void constant();
static int constantPending();
static void funcCall();
static int funcCallPending();
static void optRExprList();

int main(int argc, char **argv)
{
    parse(argv[1]);

    return 0;
}

static void advance()
{
    char *prev = current;
    current = getToken(l);
}
static int check(char *type)
{
    return current == type;
}
static void match(char *type)
{
    //if (type == IDENTIFIER)
    //    printf("ID: %s\n", getTokenString(l));

    if(check(type))
        return advance();
        
    printf("SYNTAX ERROR: expected %s got %s(%s)\n", type, current, getTokenString(l));
    exit(-2);
}
static void parse(char *filename)
{
    FILE *fp = fopen(filename, "r");
    l = newLEXER(fp);

    // Initialize globals
    current = getToken(l);
    
    // Highest level of program
    program();
    
    // Check if everything has been properly parsed
    match(END_OF_FILE);
}
static void failParse(char *component)
{
    printf("Incorrect structure in grammar module %s\n", component);
    exit(-404);
}
static void printInfo(char *symbol, char *definition)
{
    for (int i=0; i<depth; i++)
    {
        printf(" ");
    }
    printf("%s -> %s\n", symbol, definition);
}

// GRAMMAR PARSING FUNCTIONS
static void program()
{
    printInfo("program", "procedure");
    procedure();
}
static void procedure()
{
    depth++;
    printInfo("procedure", "PROCEDURE ID optFormalList procedureEnd");
    match(PROCEDURE);
    match(IDENTIFIER);
    optFormalList();
    procedureEnd();
    depth--;
}
static void procedureEnd()
{
    depth++;
    if (check(IS))
    {
        printInfo("procedureEnd", "IS declList BEGIN stmtList END ID SEMICOLON");
        advance();
        declList();
        match(BEGIN);
        stmtList();
        match(END);
        match(IDENTIFIER);
        match(SEMICOLON);
    }
    else
    {
        printInfo("procedureEnd", "SEMICOLON");
        match(SEMICOLON);
    }
    depth--;
}
static int procedurePending()
{
    return check(PROCEDURE);
}
static void function()
{
    depth++;
    printInfo("function", "FUNCTION ID optFormalList RETURN typeID functionEnd");
    match(FUNCTION);
    match(IDENTIFIER);
    optFormalList();
    match(RETURN);
    typeID();
    functionEnd();
    depth--;
}
static void functionEnd()
{
    depth++;
    if (check(IS))
    {
        printInfo("functionEnd", "IS declList BEGIN stmtList END ID SEMICOLON");
        advance();
        declList();
        match(BEGIN);
        stmtList();
        match(END);
        match(IDENTIFIER);
        match(SEMICOLON);
    }
    else
    {
        printInfo("functionEnd", "SEMICOLON");
        match(SEMICOLON);
    }
    depth--;
}
static int functionPending()
{
    return check(FUNCTION);
}
static void optFormalList()
{
    depth++;
    if (check(OPAREN))
    {
        printInfo("optFormalList", "OPAREN formalList CPAREN");
        advance();
        formalList();
        match(CPAREN);
    }
    else
    {
        printInfo("optFormalList", "EMPTY");
    }
    depth--;
}
static void formalList()
{
    depth++;
    printInfo("formalList", "formal formalListEnd");
    formal();
    if (check(SEMICOLON))
    {
        depth++;
        printInfo("formalListEnd", "SEMICOLON formalList");
        advance();
        formalList();
        depth--;
    }
    depth--;
}
static void formal()
{
    depth++;
    printInfo("formal", "idList COLON optMode typeID");
    idList();
    match(COLON);
    optMode();
    typeID();
    depth--;
}
static void idList()
{
    depth++;
    printInfo("idList", "ID idListEnd");
    match(IDENTIFIER);
    if (check(COMMA))
    {
        depth++;
        printInfo("idListEnd", "COMMA idList");
        advance();
        idList();
        depth--;
    }
    depth--;
}
static int idListPending()
{
    return check(IDENTIFIER);
}
static void optMode()
{
    depth++;
    if (check(IN))
    {
        advance();
        if (check(OUT))
        {
            printInfo("optMode", "IN OUT");
            advance();
        }
        else
        {
            printInfo("optMode", "IN");
        }
    }
    if (check(OUT))
    {
        printInfo("optMode", "OUT");
        advance();
    }
    else
    {
        printInfo("optMode", "EMPTY");
    }
    depth--;
}
static void declList()
{
    depth++;
    printInfo("declList", "declaration declListEnd");
    declaration();
    if (declarationPending())
    {
        depth++;
        printInfo("declListEnd", "declList");
        declList();
        depth--;
    }
    depth--;
}
static void declaration()
{
    depth++;
    if (typeDeclPending())
    {
        printInfo("declaration", "typeDecl");
        typeDecl();
    }
    else if (varDeclPending())
    {
        printInfo("declaration", "varDecl");
        varDecl();
    }
    else if (procedurePending())
    {
        printInfo("declaration", "procedure");
        procedure();
    }
    else if (functionPending())
    {
        printInfo("declaration", "function");
        function();
    }
    else if (packagePending())
    {
        printInfo("declaration", "package");
        package();
    }
    depth--;
}
static int declarationPending()
{
    return typeDeclPending() || varDeclPending() || procedurePending() || functionPending() || packagePending();
}
static void package()
{
    depth++;
    printInfo("package", "PACKAGE ID IS declList PRIVATE declList END ID SEMICOLON");
    match(PACKAGE);
    match(IDENTIFIER);
    match(IS);
    declList();
    match(PRIVATE);
    declList();
    match(END);
    match(IDENTIFIER);
    match(SEMICOLON);
    depth--;
}
static int packagePending()
{
    return check(PACKAGE);
}
static void typeDecl()
{
    depth++;
    printInfo("typeDecl", "TYPE ID typeDeclEnd");
    match(TYPE);
    match(IDENTIFIER);
    if (check(IS))
    {
        depth++;
        printInfo("typeDeclEnd", "IS typeDesc SEMICOLON");
        advance();
        typeDesc();
        match(SEMICOLON);
        depth--;
    }
    else
    {
        depth++;
        printInfo("typeDesclEnd", "SEMICOLON");
        match(SEMICOLON);
        depth--;
    }
    depth--;
}
static int typeDeclPending()
{
    return check(TYPE);
}
static void typeDesc()
{
    depth++;
    if (typeIDPending())
    {
        printInfo("typeDesc", "typeID");
        typeID();
    }
    else if (check(ARRAY))
    {
        printInfo("typeDesc", "ARRAY OBRACKET rExpr DOUBLE_DOT rExpr CBRACKET OF typeDesc");
        advance();
        match(OBRACKET);
        rExpr();
        match(DOUBLE_DOT);
        rExpr();
        match(CBRACKET);
        match(OF);
        typeDesc();
    }
    else
    {
        printInfo("typeDesc", "RECORD varDeclList END RECORD");
        match(RECORD);
        varDeclList();
        match(END);
        match(RECORD);
    }
    depth--;
}
static int typeDescPending()
{
    return typeIDPending() || check(ARRAY) || check(RECORD);
}
static void varDeclList()
{
    depth++;
    printInfo("varDeclList", "varDecl varDeclListEnd");
    varDecl();
    if (varDeclPending())
    {
        depth++;
        printInfo("varDeclListEnd", "varDeclList");
        varDeclList();
        depth--;
    }
    depth--;
}
static void varDecl()
{
    depth++;
    printInfo("varDecl", "idList COLON typeDesc SEMICOLON");
    idList();
    match(COLON);
    typeDesc();
    match(SEMICOLON);
    depth--;
}
static int varDeclPending()
{
    return idListPending();
}
static void typeID()
{
    depth++;
    if (lExprPending())
    {
        printInfo("typeID", "lExpr");
        lExpr();
    }
    else if (check(INTEGER))
    {
        printInfo("typeID", "INTEGER");
        advance();
    }
    else
    {
        printInfo("typeID", "BOOLEAN");
        match(BOOLEAN);
    }
    depth--;
}
static int typeIDPending()
{
    return lExprPending() || check(INTEGER) || check(BOOLEAN);
}
static void stmtList()
{
    depth++;
    printInfo("stmtList", "statement stmtListEnd");
    statement();
    if (statementPending())
    {
        depth++;
        printInfo("stmtListEnd", "stmtList");
        stmtList();
        depth--;
    }
    depth--;
}
static void statement()
{
    depth++;
    if (check(NULL))
    {
        printInfo("statement", "NULL SEMICOLON");
        advance();
        match(SEMICOLON);
    }
    else if (check(RETURN))
    {
        advance();
        if (check(SEMICOLON))
        {
            printInfo("statement", "RETURN SEMICOLON");
            advance();
        }
        else
        {
            printInfo("statement", "RETURN rExpr SEMICOLON");
            rExpr();
            match(SEMICOLON);
        }
    }
    else if (check(IF))
    {
        printInfo("statement", "IF rExpr THEN stmtList optElsifList optElse END IF SEMICOLON");
        advance();
        rExpr();
        match(THEN);
        stmtList();
        optElsifList();
        optElse();
        match(END);
        match(IF);
        match(SEMICOLON);
    }
    else if (check(WHILE))
    {
        printInfo("statement", "WHILE rExpr LOOP stmtList END LOOP SEMICOLON");
        advance();
        rExpr();
        match(LOOP);
        stmtList();
        match(END);
        match(LOOP);
        match(SEMICOLON);
    }
    else
    {
        printInfo("statement", "lExpr statementEnd");
        lExpr();
        if (check(OPAREN))
        {
            printInfo("statementEnd", "OPAREN rExprList CPAREN SEMICOLON");
            advance();
            rExprList();
            match(CPAREN);
            match(SEMICOLON);
        }
        else
        {
            printInfo("statementEnd", "ASSIGNMENT rExpr SEMICOLON");
            match(ASSIGNMENT);
            rExpr();
            match(SEMICOLON);
        }
    }
    depth--;
}
static int statementPending()
{
    return check(NULL) || check(RETURN) || check(IF) || check(WHILE) || lExprPending();
}
static void optElsifList()
{
    depth++;
    if (elsifPending())
    {
        printInfo("optElsifList", "elsifList");
        elsifList();
    }
    depth--;
}
static void elsifList()
{
    depth++;
    printInfo("elsifList", "elsif elsifListEnd");
    elsif();
    if (elsifPending())
    {
        depth++;
        printInfo("elsifListEnd", "elsifList");
        depth--;
    }
    depth--;
}
static void elsif()
{
    depth++;
    printInfo("elsif", "ELSIF rExpr THEN stmtList");
    match(ELSIF);
    rExpr();
    match(THEN);
    stmtList();
    depth--;
}
static int elsifPending()
{
    return check(ELSIF);
}
static void optElse()
{
    depth++;
    if (check(ELSE))
    {
        printInfo("optElse", "ELSE stmtList");
        advance();
        stmtList();
    }
    else
    {
        printInfo("optElse", "EMPTY");
    }
    depth--;
}
static void rExprList()
{
    depth++;
    printInfo("rExprList", "rExpr rExprListEnd");
    rExpr();
    if (check(COMMA))
    {
        depth++;
        printInfo("rExprListEnd", "COMMA rExprList");
        advance();
        rExprList();
        depth--;
    }
    depth--;
}
static void lExpr()
{
    depth++;
    match(IDENTIFIER);
    if (lExpr2Pending())
    {
        printInfo("lExpr", "ID lExpr2");
        lExpr2();
    }
    else
    {
        printInfo("lExpr", "ID");
    }
    depth--;
}
static int lExprPending()
{
    return check(IDENTIFIER);
}
static void lExpr2()
{
    depth++;
    if (check(OBRACKET))
    {
        printInfo("lExpr2", "OBRACKET rExpr CBRACKET lExpr2End");
        advance();
        rExpr();
        match(CBRACKET);
        if (lExpr2Pending())
        {
            printInfo("lExpr2End", "lExpr2");
            lExpr2();
        }
        else
        {
            printInfo("lExpr2End", "EMPTY");
        }
    }
    else
    {
        printInfo("lExpr2", "DOT lExpr");
        match(DOT);
        lExpr();
    }
    depth--;
}
static int lExpr2Pending()
{
    return check(OBRACKET) || check(DOT);
}
static void rExpr()
{
    depth++;
    printInfo("rExpr", "rExpr2 rExprEnd");
    rExpr2();
    if (check(LOGICAL_OR))
    {
        printInfo("rExprEnd", "LOGICAL_OR rExpr");
        advance();
        rExpr();
    }
    else
    {
        printInfo("rExprEnd", "EMPTY");
    }
    depth--;
}
static int rExprPending()
{
    return unaryPending();
}
static void rExpr2()
{
    depth++;
    printInfo("rExpr2", "rExpr3 rExpr2End");
    rExpr3();
    if (check(LOGICAL_AND))
    {
        printInfo("rExpr2End", "LOGICAL_AND rExpr2");
        advance();
        rExpr2();
    }
    else
    {
        printInfo("rExpr2End", "EMPTY");
    }
    depth--;
}
static void rExpr3()
{
    depth++;
    if (check(NOT))
    {
        printInfo("rExpr3", "NOT rExpr3");
        advance();
        rExpr3();
    }
    else
    {
        printInfo("rExpr3", "rExpr4");
        rExpr4();
    }
    depth--;
}
static void rExpr4()
{
    depth++;
    printInfo("rExpr4", "rExpr5 rExpr4End");
    rExpr5();
    if (check(EQUALS))
    {
        printInfo("rExpr4End", "EQUALS rExpr4");
        advance();
        rExpr4();
    }
    else if (check(NOT_EQUALS))
    {
        printInfo("rExpr4End", "NOT_EQUALS rExpr4");
        advance();
        rExpr4();
    }
    else
    {
        printInfo("rExpr4End", "EMPTY");
    }
    depth--;
}
static void rExpr5()
{
    depth++;
    printInfo("rExpr5", "rExpr6 rExpr5End");
    rExpr6();
    if (check(LESS_THAN))
    {
        printInfo("rExpr5End", "LESS_THAN rExpr5");
        advance();
        rExpr5();
    }
    else if (check(LESS_THAN_EQUAL))
    {
        printInfo("rExpr5End", "LESS_THAN_EQUAL rExpr5");
        advance();
        rExpr5();
    }
    else if (check(GREATER_THAN))
    {
        printInfo("rExpr5End", "GREATER_THAN rExpr5");
        advance();
        rExpr5();
    }
    else if (check(GREATER_THAN_EQUAL))
    {
        printInfo("rExpr5End", "GREATER_THAN_EQUAL rExpr5");
        advance();
        rExpr5();
    }
    else
    {
        printInfo("rExpr5End", "EMPTY");
    }
    depth--;
}
static void rExpr6()
{
    depth++;
    printInfo("rExpr6", "rExpr7 rExpr6End");
    rExpr7();
    if (check(PLUS))
    {
        printInfo("rExpr6End", "PLUS rExpr6");
        advance();
        rExpr6();
    }
    else if (check(MINUS))
    {
        printInfo("rExpr6End", "MINUS rExpr6");
        advance();
        rExpr6();
    }
    else
    {
        printInfo("rExpr6End", "EMPTY");
    }
    depth--;
}
static void rExpr7()
{
    depth++;
    if (check(MINUS))
    {
        printInfo("rExpr7", "U_MINUS rExpr7");
        advance();
        rExpr7();
    }
    else
    {
        printInfo("rExpr7", "unary rExpr7End");
        unary();
        if (check(TIMES))
        {
            printInfo("rExpr7End", "TIMES rExpr7");
            advance();
            rExpr7();
        }
        else if (check(DIVIDE))
        {
            printInfo("rExpr7End", "DIVIDE rExpr7");
            advance();
            rExpr7();
        }
        else if (check(MODULUS))
        {
            printInfo("rExpr7End", "MODULUS rExpr7");
            advance();
            rExpr7();
        }
        else
        {
            printInfo("rExpr7End", "EMPTY");
        }
    }
    depth--;
}
static void unary()
{
    depth++;
    if (lExprPending())
    {
        printInfo("unary", "lExpr unaryEnd");
        lExpr();
        if (check(OPAREN))
        {
            depth++;
            printInfo("unaryEnd", "OPAREN rExprList CPAREN");
            advance();
            rExprList();
            match(CPAREN);
            depth--;
        }
    }
    else if (constantPending())
    {
        printInfo("unary", "constant");
        constant();
    }
    else if (funcCallPending())
    {
        printInfo("unary", "funcCall");
        funcCall();
    }
    else if (check(IF))
    {
        printInfo("unary", "IF rExpr THEN rExpr optRExprElsifList ELSE rExpr END IF");
        advance();
        rExpr();
        optRExprElsifList();
        match(ELSE);
        rExpr();
        match(END);
        match(IF);
    }
    else
    {
        printInfo("unary", "OPAREN rExpr CPAREN");
        match(OPAREN);
        rExpr();
        match(CPAREN);
    }
    depth--;
}
static int unaryPending()
{
    return lExprPending() || constantPending() || funcCallPending() || check(IF) || check(OPAREN);
}
static void optRExprElsifList()
{
    depth++;
    if (elsifPending())
    {
        printInfo("optRExprElsifList", "rExprElsifList");
        rExprElsifList();
    }
    else
    {
        printInfo("optRExprElsifList", "EMPTY");
    }
    depth--;
}
static void rExprElsifList()
{
    depth++;
    printInfo("rExprElsifList", "rExprElsif rExprElsifListEnd");
    rExprElsif();
    if (elsifPending())
    {
        printInfo("rExprElsifListEnd", "rExprElsifList");
        rExprElsifList();
    }
    depth--;
}
static void rExprElsif()
{
    depth++;
    printInfo("rExprElsif", "ELSIF rExpr THEN rExpr");
    match(ELSIF);
    rExpr();
    match(THEN);
    rExpr();
    depth--;
}
static void constant()
{
    depth++;
    if (check(NUMBER))
    {
        printInfo("constant", "NUMBER");
        advance();
    }
    else if (check(TRUE))
    {
        printInfo("constant", "TRUE");
        advance();
    }
    else
    {
        printInfo("constant", "FALSE");
        match(FALSE);
    }
    depth--;
}
static int constantPending()
{
    return check(NUMBER) || check(TRUE) || check(FALSE);
}
static void funcCall()
{
    depth++;
    printInfo("funcCall", "ID OPAREN optRExprList CPAREN");
    match(IDENTIFIER);
    match(OPAREN);
    optRExprList();
    match(CPAREN);
    depth--;
}
static int funcCallPending()
{
    return check(IDENTIFIER);
}
static void optRExprList()
{
    depth++;
    if (rExprPending())
    {
        printInfo("optRExprList", "rExprList");
        rExprList();
    }
    else
    {
        printInfo("optRExprList", "EMPTY");
    }
    depth--;
}
