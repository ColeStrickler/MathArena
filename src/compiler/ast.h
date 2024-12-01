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


class ExprNode : public MathNode
{
public:
    ExprNode()
    {

    }
    ~ExprNode()
    {

    }

    virtual std::string toString() override
    {
        return "ExprNode::toString()";
    }
};



class EquationNode : public MathNode
{
public:
    EquationNode(ExprNode* root);
    ~EquationNode();


    virtual std::string toString() {return m_Root->toString();}
// we will add some properties to interact with other equation nodes here

private: 
    ExprNode* m_Root;

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

    virtual std::string toString() override {
        printf("TermNode::toString()\n");
        switch (m_Type)
        {
            case TermType::TERMNUMBER:
            {
                std::string ret = std::to_string(m_Number);
                return ret;
            }
            case TermType::TERMVARIABLE:
            {
                std::string ret = m_Number == 1.0f ? "" : std::to_string(m_Number);
                ret += m_Var;
                ret += m_Exponent == 1.0f ? "" : "^(" + std::to_string(m_Exponent) + ")";
                return ret;
            }
            case TermType::TERMEXPRESSION:
            {
                 std::string ret = m_Number == 1.0f ? "" : std::to_string(m_Number);
                 ret += "(" + m_Expression->toString() + ")";
                 ret += m_Exponent == 1.0f ? "" : "^(" + std::to_string(m_Exponent) + ")";
                 return ret;
            }
        }
        return "TermNode::toString() error\n";
    }


private:
    double m_Number;
    std::string m_Var;
    double m_Exponent;
    ExprNode* m_Expression;
    TermType m_Type;
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

    virtual std::string toString() override {
        printf("ASNode::toString() %d\n", m_Entries.size());
        std::string ret;
        
        ret += m_Entries[0].expr->toString();

        for (int i = 1; i < m_Entries.size(); i++)
        {
            auto& entry = m_Entries[i];
            if (entry.type == TokenType::CROSS)
                ret += "+" + entry.expr->toString();
            else
                ret += "-" + entry.expr->toString();
        }
        return ret;
    }

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

    virtual std::string toString() override {
        std::string ret;
        printf("MDNode::toString()\n");
        ret += m_Entries[0].expr->toString();

        for (int i = 1; i < m_Entries.size(); i++)
        {
            auto& entry = m_Entries[i];
            if (entry.type == TokenType::CROSS)
                ret += "+" + entry.expr->toString();
            else
                ret += "-" + entry.expr->toString();
        }
        return ret;
    }
private:
    std::vector<ExpEntry> m_Entries;
};








#endif