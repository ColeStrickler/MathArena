#include "eqscanner.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "parser.h"
EquationScanner::EquationScanner(std::ifstream* file) : yyFlexLexer(file), m_colNum(0)
{
   
}

EquationScanner::EquationScanner(std::istream *stringstream) : yyFlexLexer(stringstream), m_colNum(0)
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



int main2()
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
        if (type == TokenType::EQERROR)
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
      //  printf("here\n");
        std::cout << parser.GetErrorString();
        printf("Parse failed.\n");

    }
    else
    {
        printf("Parse successful.");
        auto ast = parser.GetEquation();
        std::cout << ast->toString();
        std::unordered_map<std::string, double> varVals = {{"x", 1.0f}, {"z",2.0f}};
        std::cout << "evaluated: " << ast->evaluate(varVals);
    }
    

    printf("done\n");
    return 0;
}