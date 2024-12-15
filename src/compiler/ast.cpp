#include "ast.h"

EquationNode::EquationNode(ExprNode* lhs, ExprNode* rhs) : m_LHS(lhs), m_RHS(rhs)
{
}

EquationNode::~EquationNode()
{
    delete m_LHS;
    delete m_RHS;
}

bool EquationNode::CanSolveForVar(const std::string &var)
{
    
    assert(var == "x" || var == "y" || var == "z");

    return m_LHS->CanSolveForVar(var);
}

bool EquationNode::Solve()
{
    std::string selected_var = "";
    std::vector<std::string> vars = {"y", "z", "x"};
    for (auto& var: vars)
    {
        if (CanSolveForVar(var))
        {
            selected_var = var;
            m_SolvedVar = var;
            break;
        }
    }

    // Found no valid variable candidates to solve for
    if (selected_var.size() == 0)
        return false;
    printf("solving for var: %s\n", selected_var.c_str());
    // if fail to solve, return false
    if(!m_LHS->Solve(selected_var, this))
        return false; 

    m_LHS = new TermNode(1.0f, m_SolvedVar, 1.0f);

    printf("Solved: %s\n", toString().c_str());
    return true;
}

void ASNode::AddExpr(ExprNode *expr, TokenType op)
{
    assert(op == TokenType::CROSS || op == TokenType::DASH);

    m_Entries.push_back(ExpEntry{op, expr});
}

bool ASNode::Solve(const std::string &var, EquationNode *equation)
{
    printf("ASNode::Solve\n");
    int i = 0;
    auto asExpr = new ASNode();
    asExpr->AddExpr(equation->GetRHS(), TokenType::CROSS);
    for (int i = 0; i < m_Entries.size();)
    {
        auto& e = m_Entries[i];
        if (e.expr->HasVarInstance(var))
        {
            i++;
            continue;
        }
        printf("adding over %s\n", e.expr->toString().c_str());
                 
        // Do the opposite operation
        asExpr->AddExpr(e.expr, e.type == TokenType::CROSS ? TokenType::DASH : TokenType::CROSS);
        m_Entries.erase(m_Entries.begin() + i);
    }
   // printf("here! %d\n", m_Entries.size());
    // we do not yet have factoring, should only be one term left
    if (m_Entries.size() != 1)
        return false;
    
    equation->SetRHS(asExpr);
    return m_Entries[0].expr->Solve(var, equation);
}
void MDNode::AddExpr(ExprNode *expr, TokenType op)
{
    assert(op == TokenType::STAR || op == TokenType::SLASH);
    m_Entries.push_back(ExpEntry{op, expr});
}

bool MDNode::Solve(const std::string &var, EquationNode *equation)
{
    printf("MDNode::Solve\n");
    int i = 0;
    auto asExpr = new MDNode();
    asExpr->AddExpr(equation->GetRHS(), TokenType::STAR);
    for (int i = 0; i < m_Entries.size();)
    {
        auto& e = m_Entries[i];
        if (e.expr->HasVarInstance(var))
        {
            i++;
            continue;
        }
                 
        // Do the opposite operation
        asExpr->AddExpr(e.expr, e.type == TokenType::STAR ? TokenType::SLASH : TokenType::STAR);
        m_Entries.erase(m_Entries.begin() + i);
    }
    printf("here\n");
    // we do not yet have factoring, should only be one term left
    if (m_Entries.size() != 1)
        return false;
    
    equation->SetRHS(asExpr); 
    return m_Entries[0].expr->Solve(var, equation);
}

void TermNode::Negate()
{
    m_Number *= -1.0f;
}

bool TermNode::Solve(const std::string &var, EquationNode *equation)
{
    printf("TermNode::Solve\n");
    switch (m_Type)
    {
        case TermType::TERMNUMBER: return false;
        case TermType::TERMVARIABLE:
        {
            printf("TermNode solve variable\n");
            if (m_Number != 1.0f)
            {
                auto mdNode = new MDNode();
                mdNode->AddExpr(equation->GetRHS(), TokenType::STAR);
                // divide by coefficient
                auto divTerm = new TermNode(m_Number);
                mdNode->AddExpr(divTerm, TokenType::SLASH);
                equation->SetRHS(mdNode);
            }
            
            // power to inverse exponent
            auto invExp = new TermNode(1.0f, equation->GetRHS(), 1.0f/m_Exponent);
            equation->SetRHS(invExp);
            break;
        }
        case TermType::TERMEXPRESSION:
        {
            auto mdNode = new MDNode();
            mdNode->AddExpr(equation->GetRHS(), TokenType::STAR);
            // divide by coefficient
            auto divTerm = new TermNode(m_Number);
            mdNode->AddExpr(divTerm, TokenType::SLASH);
            equation->SetRHS(mdNode);
            // power to inverse exponent
            auto invExp = new TermNode(1.0f, equation->GetRHS(), 1.0f/m_Exponent);
            equation->SetRHS(invExp);
            m_Expression->Solve(var, equation);
            break;
        }
        default:
        {
            return false;
        }
    }
    return true;
}
