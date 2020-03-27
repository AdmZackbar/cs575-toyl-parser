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
    //printf("New token: %s\n", current);
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
    printf("Incorrect structure in grammar module %s, have token %s\n", component, current);
    exit(-404);
}
static void printInfo(char *symbol, char *definition)
{
    for (int i=0; i<depth; i++)
    {
        printf(" ");
    }
    printf("%s --> %s\n", symbol, definition);
}

// GRAMMAR PARSING FUNCTIONS
static void program()
{
    printInfo("Program", "Procedure");
    procedure();
}
static void procedure()
{
    depth++;
    printInfo("Procedure", "PROCEDURE ID OptFormalList MoreProcedure");
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
        printInfo("MoreProcedure", "IS DeclList BEGIN StmtList END ID ;");
        advance();
        declList();
        match(BEGIN);
        stmtList();
        match(END);
        match(IDENTIFIER);
        match(SEMICOLON);
    }
    else if (check(SEMICOLON))
    {
        printInfo("ProcedureEnd", "SEMICOLON");
        advance();
    }
    else
    {
        failParse("Procedure");
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
    printInfo("Function", "FUNCTION ID OptFormalList RETURN TypeID MoreFunction");
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
        printInfo("MoreFunction", "IS DeclList BEGIN StmtList END ID ;");
        advance();
        declList();
        match(BEGIN);
        stmtList();
        match(END);
        match(IDENTIFIER);
        match(SEMICOLON);
    }
    else if (check(SEMICOLON))
    {
        printInfo("MoreFunction", ";");
        advance();
    }
    else
    {
        failParse("function");
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
        printInfo("OptFormalList", "( FormalList )");
        advance();
        formalList();
        match(CPAREN);
    }
    else
    {
        printInfo("OptFormalList", "empty");
    }
    depth--;
}
static void formalList()
{
    depth++;
    printInfo("FormalList", "Formal MoreFormalList");
    formal();
    if (check(SEMICOLON))
    {
        depth++;
        printInfo("MoreFormalList", "; FormalList");
        advance();
        formalList();
        depth--;
    }
	else
	{
		depth++;
		printInfo("MoreFormalList", "empty");
		depth--;
	}
    depth--;
}
static void formal()
{
    depth++;
    printInfo("Formal", "IdList : OptMode TypeID");
    idList();
    match(COLON);
    optMode();
    typeID();
    depth--;
}
static void idList()
{
    depth++;
    printInfo("IdList", "ID MoreIdList");
    match(IDENTIFIER);
    if (check(COMMA))
    {
        depth++;
        printInfo("MoreIdList", ", idList");
        advance();
        idList();
        depth--;
    }
	else
	{
		depth++;
		printInfo("MoreIdList", "empty");
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
            printInfo("OptMode", "IN OUT");
            advance();
        }
        else
        {
            printInfo("OptMode", "IN");
        }
    }
    else if (check(OUT))
    {
        printInfo("OptMode", "OUT");
        advance();
    }
    else
    {
        printInfo("OptMode", "empty");
    }
    depth--;
}
static void declList()
{
    depth++;
    if (declarationPending())
    {
        printInfo("DeclList", "Declaration DeclList");
        declaration();
        declList();
    }
    else
    {
        printInfo("DeclList", "empty");
    }
    depth--;
}
static void declaration()
{
    depth++;
    if (typeDeclPending())
    {
        printInfo("Declaration", "TypeDecl");
        typeDecl();
    }
    else if (varDeclPending())
    {
        printInfo("Declaration", "VarDecl");
        varDecl();
    }
    else if (procedurePending())
    {
        printInfo("Declaration", "Procedure");
        procedure();
    }
    else if (functionPending())
    {
        printInfo("Declaration", "Function");
        function();
    }
    else if (packagePending())
    {
        printInfo("Declaration", "Package");
        package();
    }
    else
    {
        failParse("Declaration");
    }
    depth--;
}
static int declarationPending()
{
    return typeDeclPending() || varDeclPending() || procedurePending() || functionPending() || packagePending();
}
// TODO: check if private is optional and if decllists are optional
static void package()
{
    depth++;
    printInfo("Package", "PACKAGE ID IS DeclList PRIVATE DeclList END ID ;");
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
    printInfo("TypeDecl", "TYPE ID MoreTypeDecl");
    match(TYPE);
    match(IDENTIFIER);
    if (check(IS))
    {
        depth++;
        printInfo("MoreTypeDecl", "IS TypeDesc ;");
        advance();
        typeDesc();
        match(SEMICOLON);
        depth--;
    }
    else if (check(SEMICOLON))
    {
        depth++;
        printInfo("MoreTypeDecl", ";");
        advance();
        depth--;
    }
    else
    {
        failParse("TypeDecl");
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
        printInfo("TypeDesc", "TypeID");
        typeID();
    }
    else if (check(ARRAY))
    {
        printInfo("TypeDesc", "ARRAY [ RExpr .. RExpr ] OF TypeDesc");
        advance();
        match(OBRACKET);
        rExpr();
        match(DOUBLE_DOT);
        rExpr();
        match(CBRACKET);
        match(OF);
        typeDesc();
    }
    else if (check(RECORD))
    {
        printInfo("TypeDesc", "RECORD VarDeclList END RECORD");
        advance();
        varDeclList();
        match(END);
        match(RECORD);
    }
    else
    {
        failParse("TypeDesc");
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
    printInfo("VarDeclList", "VarDecl VarDeclListEnd");
    varDecl();
    if (varDeclPending())
    {
        depth++;
        printInfo("VarDeclListEnd", "VarDeclList");
        varDeclList();
        depth--;
    }
    depth--;
}
static void varDecl()
{
    depth++;
    printInfo("VarDecl", "IdList : TypeDesc ;");
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
        printInfo("TypeID", "LExpr");
        lExpr();
    }
    else if (check(INTEGER))
    {
        printInfo("TypeID", "INTEGER");
        advance();
    }
    else if (check(BOOLEAN))
    {
        printInfo("TypeID", "BOOLEAN");
        advance();
    }
    else
    {
        failParse("TypeID");
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
    printInfo("StmtList", "Statement MoreStmtList");
    statement();
    if (statementPending())
    {
        depth++;
        printInfo("MoreStmtList", "StmtList");
        stmtList();
        depth--;
    }
	else
	{
		depth++;
		printInfo("MoreStmtList", "empty");
		depth--;
	}
    depth--;
}
static void statement()
{
    depth++;
    if (check(NULL_WORD))
    {
        printInfo("Statement", "NULL ;");
        advance();
        match(SEMICOLON);
    }
    else if (check(RETURN))
    {
        advance();
        if (check(SEMICOLON))
        {
            printInfo("Statement", "RETURN ;");
            advance();
        }
        else if (rExprPending())
        {
            printInfo("Statement", "RETURN RExpr ;");
            rExpr();
            match(SEMICOLON);
        }
        else
        {
            failParse("Return Statement");
        }
    }
    else if (check(IF))
    {
        printInfo("Statement", "IF RExpr THEN StmtList OptElsifList OptElse END IF ;");
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
        printInfo("Statement", "WHILE RExpr LOOP StmtList END LOOP ;");
        advance();
        rExpr();
        match(LOOP);
        stmtList();
        match(END);
        match(LOOP);
        match(SEMICOLON);
    }
    else if (lExprPending())
    {
        printInfo("Statement", "LExpr MoreStatement");
        lExpr();
        if (check(OPAREN))
        {
            printInfo("MoreStatement", "( OptRExprList ) ;");
            advance();
            optRExprList();
            match(CPAREN);
            match(SEMICOLON);
        }
        else if (check(ASSIGNMENT))
        {
            printInfo("MoreStatement", ":= RExpr ;");
            advance();
            rExpr();
            match(SEMICOLON);
        }
        else
        {
            failParse("LExpr Statement");
        }
    }
    else
    {
        failParse("Statement");
    }
    depth--;
}
// TODO check if NULL works
static int statementPending()
{
    return check(NULL_WORD) || check(RETURN) || check(IF) || check(WHILE) || lExprPending();
}
static void optElsifList()
{
    depth++;
    if (elsifPending())
    {
        printInfo("OptElsifList", "ElsifList");
        elsifList();
    }
	else
	{
		printInfo("OptElsifList", "empty");
	}
    depth--;
}
static void elsifList()
{
    depth++;
    printInfo("ElsifList", "Elsif MoreElsifList");
    elsif();
    if (elsifPending())
    {
        depth++;
        printInfo("MoreElsifList", "ElsifList");
        elsifList();
        depth--;
    }
	else
	{
		depth++;
		printInfo("MoreElsifList", "empty");
		depth--;
	}
    depth--;
}
static void elsif()
{
    depth++;
    printInfo("Elsif", "ELSIF RExpr THEN StmtList");
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
        printInfo("OptElse", "ELSE StmtList");
        advance();
        stmtList();
    }
    else
    {
        printInfo("OptElse", "empty");
    }
    depth--;
}
static void rExprList()
{
    depth++;
    printInfo("RExprList", "RExpr MoreRExprList");
    rExpr();
    if (check(COMMA))
    {
        depth++;
        printInfo("MoreRExprList", "COMMA rExprList");
        advance();
        rExprList();
        depth--;
    }
	else
	{
		depth++;
		printInfo("MoreRExprList", "empty");
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
        printInfo("LExpr", "ID LExpr2");
        lExpr2();
    }
    else
    {
        printInfo("LExpr", "ID");
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
        printInfo("LExpr2", "[ RExpr ] MoreLExpr2");
        advance();
        rExpr();
        match(CBRACKET);
        if (lExpr2Pending())
        {
			depth++;
            printInfo("MoreLExpr2", "LExpr2");
            lExpr2();
			depth--;
        }
        else
        {
			depth++;
            printInfo("MoreLExpr2", "empty");
			depth--;
        }
    }
    else if (check(DOT))
    {
        printInfo("LExpr2", ". LExpr");
        advance();
        lExpr();
    }
    else
    {
        failParse("LExpr2");
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
    printInfo("RExpr", "RExpr2 MoreRExpr");
    rExpr2();
    if (check(LOGICAL_OR))
    {
		depth++;
        printInfo("MoreRExpr", "OR rExpr");
        advance();
        rExpr();
		depth--;
    }
    else
    {
		depth++;
        printInfo("MoreRExpr", "empty");
		depth--;
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
    printInfo("RExpr2", "RExpr3 MoreRExpr2");
    rExpr3();
	depth++;
    if (check(LOGICAL_AND))
    {
        printInfo("MoreRExpr2", "AND RExpr2");
        advance();
        rExpr2();
    }
    else
    {
        printInfo("MoreRExpr2", "empty");
    }
	depth--;
    depth--;
}
static void rExpr3()
{
    depth++;
    if (check(NOT))
    {
        printInfo("RExpr3", "NOT RExpr3");
        advance();
        rExpr3();
    }
    else
    {
        printInfo("RExpr3", "RExpr4");
        rExpr4();
    }
    depth--;
}
static void rExpr4()
{
    depth++;
    printInfo("RExpr4", "RExpr5 MoreRExpr4");
    rExpr5();
	depth++;
    if (check(EQUALS))
    {
        printInfo("MoreRExpr4", "= RExpr4");
        advance();
        rExpr4();
    }
    else if (check(NOT_EQUALS))
    {
        printInfo("MoreRExpr4", "<> RExpr4");
        advance();
        rExpr4();
    }
    else
    {
        printInfo("MoreRExpr4", "empty");
    }
	depth--;
    depth--;
}
static void rExpr5()
{
    depth++;
    printInfo("RExpr5", "RExpr6 MoreRExpr5");
    rExpr6();
	depth++;
    if (check(LESS_THAN))
    {
        printInfo("MoreRExpr5", "< RExpr5");
        advance();
        rExpr5();
    }
    else if (check(LESS_THAN_EQUAL))
    {
        printInfo("MoreRExpr5", "<= RExpr5");
        advance();
        rExpr5();
    }
    else if (check(GREATER_THAN))
    {
        printInfo("MoreRExpr5", "> RExpr5");
        advance();
        rExpr5();
    }
    else if (check(GREATER_THAN_EQUAL))
    {
        printInfo("MoreRExpr5", ">= RExpr5");
        advance();
        rExpr5();
    }
    else
    {
        printInfo("MoreRExpr5", "empty");
    }
	depth--;
    depth--;
}
static void rExpr6()
{
    depth++;
    printInfo("RExpr6", "RExpr7 MoreRExpr6");
    rExpr7();
	depth++;
    if (check(PLUS))
    {
        printInfo("MoreRExpr6", "+ RExpr6");
        advance();
        rExpr6();
    }
    else if (check(MINUS))
    {
        printInfo("MoreRExpr6", "- RExpr6");
        advance();
        rExpr6();
    }
    else
    {
        printInfo("MoreRExpr6", "empty");
    }
	depth--;
    depth--;
}
static void rExpr7()
{
    depth++;
    if (check(MINUS))
    {
        printInfo("RExpr7", "- RExpr7");
        advance();
        rExpr7();
    }
    else
    {
        printInfo("RExpr7", "Unary MoreRExpr7");
        unary();
		depth++;
        if (check(TIMES))
        {
            printInfo("MoreRExpr7", "* RExpr7");
            advance();
            rExpr7();
        }
        else if (check(DIVIDE))
        {
            printInfo("MoreRExpr7", "/ RExpr7");
            advance();
            rExpr7();
        }
        else if (check(MODULUS))
        {
            printInfo("MoreRExpr7", "% RExpr7");
            advance();
            rExpr7();
        }
        else
        {
            printInfo("MoreRExpr7", "empty");
        }
		depth--;
    }
    depth--;
}
static void unary()
{
    depth++;
    if (lExprPending())
    {
        printInfo("Unary", "LExpr MoreUnary");
        lExpr();
        if (check(OPAREN))
        {
            depth++;
            printInfo("MoreUnary", "( OptRExprList )");
            advance();
            optRExprList();
            match(CPAREN);
            depth--;
        }
		else
		{
			depth++;
			printInfo("MoreUnary", "empty");
			depth--;
		}
    }
    else if (constantPending())
    {
        printInfo("Unary", "Constant");
        constant();
    }
    else if (funcCallPending())
    {
        printInfo("Unary", "FuncCall");
        funcCall();
    }
    else if (check(IF))
    {
        printInfo("Unary", "IF RExpr THEN RExpr OptRExprElsifList ELSE RExpr END IF");
        advance();
        rExpr();
        match(THEN);
        rExpr();
        optRExprElsifList();
        match(ELSE);
        rExpr();
        match(END);
        match(IF);
    }
    else if (check(OPAREN))
    {
        printInfo("Unary", "( RExpr )");
        match(OPAREN);
        rExpr();
        match(CPAREN);
    }
    else
    {
        failParse("Unary");
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
        printInfo("OptRExprElsifList", "RExprElsifList");
        rExprElsifList();
    }
    else
    {
        printInfo("OptRExprElsifList", "empty");
    }
    depth--;
}
static void rExprElsifList()
{
    depth++;
    printInfo("RExprElsifList", "RExprElsif RExprElsifListEnd");
    rExprElsif();
    if (elsifPending())
    {
        printInfo("RExprElsifListEnd", "RExprElsifList");
        rExprElsifList();
    }
    depth--;
}
static void rExprElsif()
{
    depth++;
    printInfo("RExprElsif", "ELSIF RExpr THEN RExpr");
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
        printInfo("Constant", "<number>");
        advance();
    }
    else if (check(TRUE))
    {
        printInfo("Constant", "TRUE");
        advance();
    }
    else if (check(FALSE))
    {
        printInfo("Constant", "FALSE");
        advance();
    }
    else
    {
        failParse("Constant");
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
    printInfo("FuncCall", "ID ( OptRExprList )");
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
        printInfo("OptRExprList", "RExprList");
        rExprList();
    }
    else
    {
        printInfo("OptRExprList", "empty");
    }
    depth--;
}
