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

static char* TokenTypeStrings[]
{
    "ERROR",
    "NUMBER",
    "VARIABLE",
    "LPAREN",
    "RPAREN",
    "CROSS",
    "DASH",
    "STAR",
    "SLASH",
    "POW",
    "WHITESPACE",
    "END",
};



typedef struct Token
{
    Token(TokenType type, std::string lexeme, int colStart, int colEnd) : m_Type(type), m_Lexeme(lexeme), m_ColStart(colStart), m_ColEnd(colEnd)
    {

    }

    std::string toString() const {return "TokenType::" + std::string(TokenTypeStrings[m_Type]) + " ==> " + m_Lexeme;}
    TokenType GetType() const {return m_Type;}
    std::string GetLexeme() const {return m_Lexeme;}
private:
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
    int GetNextToken(Token** tok);
    int m_colNum;
private:
    std::string m_FileStringStream;
    std::ifstream* m_File;

};




#endif