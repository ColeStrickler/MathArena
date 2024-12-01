#include "eqscanner.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "parser.h"
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
    std::vector<Token*> tokens;

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
        tokens.push_back(out);
    }
    fs.close();

    EquationParser parser(tokens);
    if(!parser.Parse())
    {
        //std::cout << parser.GetErrorString();
        printf("Parse failed.\n");
    }
    else
    {
        printf("Parse successful.");
        auto ast = parser.GetEquation();
        std::cout << ast->toString();
    }
    

    printf("done\n");
    return 0;
}