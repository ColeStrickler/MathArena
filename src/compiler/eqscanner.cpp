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

int EquationScanner::GetNextToken(Token** tok)
{
    int type = yylex(tok);
    return type;
}



int main()
{
    std::string file = "/home/cole/Documents/MathArena/src/compiler/test.txt";
    auto fs = std::ifstream(file);
    auto eq = EquationScanner(&fs);

    bool err = false;

    while (true)
    {

        Token* out = nullptr;
        auto type = eq.GetNextToken(&out);
        if (type == TokenType::ERROR)
        {
            printf("Error %d\n", eq.m_colNum);
            break;
        }
        else if (type == TokenType::WHITESPACE)
            continue;
        else if (type == TokenType::END)
            break;
        else
            std::cout << out->toString() << "\n";
    }
    

    printf("done\n");
}