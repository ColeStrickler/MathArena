#ifndef EQAST_H
#define EQAST_H
#include <string>
#include <vector>
#include <unordered_map>
#include <cassert>
#include "eqscanner.h"

class ExprNode;




class MathNode
{
public:
    virtual std::string toString()
    {

    }
    virtual double evaluate(std::unordered_map<std::string, double>& varVals)
    {

    }
    virtual ExprNode* derivative(const std::string& dVar)
    {

    }
    virtual ExprNode* integrate(const std::string& dVar, float boundA, float boundB)
    {

    }
    virtual std::string toC()
    {

    }
    virtual void Delete()
    {

    }
    virtual void Negate()
    {

    }
};


class EquationNode : public MathNode
{
public:
    EquationNode(ExprNode* root);
    ~EquationNode();

// we will add some properties to interact with other equation nodes here

private: 
    ExprNode* m_Root;

};








class ExprNode : public MathNode
{
public:
    ExprNode()
    {

    }
    ~ExprNode()
    {

    }
};


enum TermType {
    TERMNUMBER,
    TERMVARIABLE,
    TERMEXPRESSION,
};


class TermNode : public ExprNode
{
public:
    TermNode(double coefficient, const std::string& variable, double exp) : m_Type(TERMVARIABLE), m_Number(coefficient), m_Var(variable), m_Exponent(exp)
    {
        
    }
    TermNode(double coefficient, ExprNode* expr, double exp) : m_Type(TERMEXPRESSION), m_Number(coefficient), m_Expression(expr), m_Exponent(exp)
    {

    }
    TermNode(double number) : m_Type(TERMNUMBER), m_Number(number)
    {

    }

    virtual void Negate() override;
    ~TermNode();

private:
    double m_Number;
    std::string m_Var;
    double m_Exponent;
    ExprNode* m_Expression;
    TermType m_Type;
};


class BinaryExprNode : public ExprNode
{
public:
    BinaryExprNode()
    {

    }
    ~BinaryExprNode()
    {

    }
    ExprNode* m_Expr1;
    ExprNode* m_Expr2;
};

class UnaryExprNode : public ExprNode
{
public:
    UnaryExprNode()
    {

    }
    ~UnaryExprNode()
    {

    }
    ExprNode* m_Expr;
};




typedef struct ExpEntry
{
    TokenType type;
    ExprNode* expr;

}ASEntry;

/*
    Addition/Subtraction node
*/
class ASNode : public ExprNode
{
public:
    ASNode()
    {

    }
    ~ASNode()
    {

    }
    void AddExpr(ExprNode* expr, TokenType op);

private:
    std::vector<ExpEntry> m_Entries;
};


/*
    Multiply/Divide node
*/
class MDNode : public ExprNode
{
public:
    MDNode()
    {

    }
    ~MDNode()
    {

    }
    void AddExpr(ExprNode* expr, TokenType op);
private:
    std::vector<ExpEntry> m_Entries;
};








#endif