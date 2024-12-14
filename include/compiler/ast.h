#ifndef EQAST_H
#define EQAST_H
#include <string>
#include <vector>
#include <unordered_map>
#include <cassert>
#include "eqscanner.h"
#include <cmath>
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
    EquationNode(ExprNode* lhs, ExprNode* rhs);
    ~EquationNode();


    virtual std::string toString() {return m_LHS->toString() + "=" + m_RHS->toString();}

    virtual double evaluate(std::unordered_map<std::string, double>& varVals)
    {
        /*
            Eventually want to add some logic to force everything to one side
        */


       return m_RHS->evaluate(varVals);
    }



// we will add some properties to interact with other equation nodes here

private: 
    ExprNode* m_LHS;
    ExprNode* m_RHS;

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
    ~TermNode()
    {
        delete m_Expression;
    }

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
    virtual double evaluate(std::unordered_map<std::string, double>& varVals)
    {
        double ret = 0.0f;
        switch (m_Type)
        {
            case TermType::TERMNUMBER:
            {
                ret = m_Number;
                break;
            }
            case TermType::TERMVARIABLE:
            {
                ret = m_Number*pow(varVals[m_Var], m_Exponent);
                break;
            }
            case TermType::TERMEXPRESSION:
            {
                ret = m_Number*pow(m_Expression->evaluate(varVals), m_Exponent);
                break;
            }
        }

        return ret;
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
        for (auto& e : m_Entries)
            delete e.expr;
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

    virtual double evaluate(std::unordered_map<std::string, double>& varVals)
    {
        double val = m_Entries[0].expr->evaluate(varVals);
        for (int i = 1; i < m_Entries.size(); i++)
        {
            auto& entry = m_Entries[i];
            if (entry.type == TokenType::CROSS)
                val += entry.expr->evaluate(varVals);
            else
                val -= entry.expr->evaluate(varVals);
        }
        return val;
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
        for (auto& e : m_Entries)
            delete e.expr;
    }
    void AddExpr(ExprNode* expr, TokenType op);

    virtual std::string toString() override {
        std::string ret;
        printf("MDNode::toString()\n");
        ret += m_Entries[0].expr->toString();

        for (int i = 1; i < m_Entries.size(); i++)
        {
            auto& entry = m_Entries[i];
            if (entry.type == TokenType::STAR)
                ret += "*" + entry.expr->toString();
            else
                ret += "/" + entry.expr->toString();
        }
        return ret;
    }

    virtual double evaluate(std::unordered_map<std::string, double>& varVals)
    {
        double val = m_Entries[0].expr->evaluate(varVals);
        for (int i = 1; i < m_Entries.size(); i++)
        {
            auto& entry = m_Entries[i];
            if (entry.type == TokenType::STAR)
                val *= entry.expr->evaluate(varVals);
            else
                val /= entry.expr->evaluate(varVals);
        }
        return val;
    }
private:
    std::vector<ExpEntry> m_Entries;
};








#endif