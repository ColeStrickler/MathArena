#ifndef EQPARSER_H
#define EQPARSER_H

#include "eqscanner.h"
#include "ast.h"
#include <vector>

class EquationParser
{
public:
    EquationParser(std::vector<Token*> tokens);
    ~EquationParser();
    bool Parse();
    EquationNode* GetEquation() const { return m_Equation; };
    std::string GetErrorString() {
        return "EquationParser error at token #" + std::to_string(m_CurrentToken) + " ==> " + Peek()->toString() + "\n" + m_ErrorLog;
    }
    
private:
    // Parsing Functions
    ExprNode* ParseMDExpr(); // mults & divs
    ExprNode* ParseASExpr(); // add & sub
    ExprNode* ParseUnaryExpr();
    ExprNode* ParseTermNode();    



    
    EquationNode* m_Equation;
    std::vector<Token*> m_Tokens;
    int m_CurrentToken;
    void Consume();
    bool ConsumeIfEqual(TokenType type);
    Token* Peek();
    Token* PeekNext();
    Token* PeekAhead(int n);
    bool AtEnd();
    TokenType PeekType();
    

    // Errors
    bool IsError();
    void SetError(const std::string& err);
    bool m_HasError;
    std::string m_ErrorLog;

};



#endif