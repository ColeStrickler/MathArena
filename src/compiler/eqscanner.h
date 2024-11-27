#ifndef EQSCANNER_H
#define EQSCANNER_H
#include <string>
#include <FlexLexer.h>

enum TokenType
{
    ERROR,
    NUMBER,
    VARIABLE,
    LPAREN,
    RPAREN,
    CROSS,
    DASH,
    STAR,
    SLASH,
    POW,
    WHITESPACE,
    END,
};



typedef struct Token
{
    Token(TokenType type, std::string lexeme, int colStart, int colEnd) : m_Type(type), m_Lexeme(lexeme), m_ColStart(colStart), m_ColEnd(colEnd)
    {

    }
    int m_ColStart;
    int m_ColEnd;
    TokenType m_Type;
    std::string m_Lexeme;
}Token;



class EquationScanner: public yyFlexLexer
{
public:
    EquationScanner(std::ifstream* file);
    ~EquationScanner();

    int yylex(Token** out);
    Token* GetNextToken();

private:
    int m_colNum;
    std::string m_FileStringStream;
    std::ifstream* m_File;

};




#endif