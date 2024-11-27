#include "eqscanner.h"

#include <iostream>
#include <fstream>
#include <sstream>

EquationScanner::EquationScanner(std::ifstream* file) : yyFlexLexer(file)
{

    
    
}

EquationScanner::~EquationScanner()
{
}

Token *EquationScanner::GetNextToken()
{
    Token* out = nullptr;
    int type = yylex(&out);
    if (type == TokenType::ERROR)
        printf("got error\n");
    printf("got token %d, %x\n", type, out);
    return out;
}



int main()
{
    std::string file = "/home/cole/Documents/MathArena/src/compiler/test.txt";
    auto fs = std::ifstream(file);
    auto eq = EquationScanner(&fs);

    auto tok = eq.GetNextToken();

    printf("lexeme: %s\n", tok->m_Lexeme.c_str());
}