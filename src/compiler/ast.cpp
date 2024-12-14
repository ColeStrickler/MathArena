#include "ast.h"

EquationNode::EquationNode(ExprNode* lhs, ExprNode* rhs) : m_LHS(lhs), m_RHS(rhs)
{
}

EquationNode::~EquationNode()
{
    delete m_LHS;
    delete m_RHS;
}


void ASNode::AddExpr(ExprNode *expr, TokenType op)
{
    assert(op == TokenType::CROSS || op == TokenType::DASH);

    m_Entries.push_back(ExpEntry{op, expr});
}

void MDNode::AddExpr(ExprNode *expr, TokenType op)
{
    assert(op == TokenType::STAR || op == TokenType::SLASH);
    m_Entries.push_back(ExpEntry{op, expr});
}

void TermNode::Negate()
{
    m_Number *= -1.0f;
}
