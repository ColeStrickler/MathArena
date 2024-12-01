#include "parser.h"

EquationParser::EquationParser(std::vector<Token *> tokens) : m_Equation(nullptr), m_CurrentToken(0), m_Tokens(tokens), m_HasError(false)
{
}

EquationParser::~EquationParser()
{
    //m_Equation->Delete(); // frees all nodes in AST
}

bool EquationParser::Parse()
{
    printf("Parse()\n");
    auto expr = ParseASExpr();
    printf("Parse done\n");
    if (IsError() || expr == nullptr)
    {
        printf("IsError()\n");
        return false;
    }
    printf("1\n");
    if (m_CurrentToken < m_Tokens.size())
    {
        SetError("Did not successfully parse entire token steam.");
        return false;
    }


    m_Equation = new EquationNode(expr);
    return true;
}

ExprNode *EquationParser::ParseMDExpr()
{
    printf("ParseMDExpr()\n");
    if (AtEnd())
    {
        SetError("EquationParser::ParseMDExpr() arrived at unexpected end.");
        return nullptr;
    }
   
    auto term = ParseTermNode();
    if (AtEnd() || IsError())
        return term;

    auto mdnode = new MDNode();
    /*
        We may want to change the beginning token type to nop
    */
    mdnode->AddExpr(term, TokenType::STAR);

    while (PeekType() == TokenType::STAR || PeekType() == TokenType::SLASH)
    {
        auto type = PeekType();
        Consume();
        auto expr = ParseTermNode();
        if (IsError() || expr == nullptr)
            return nullptr;
        mdnode->AddExpr(expr, type);
    }
    return mdnode;
}

ExprNode *EquationParser::ParseASExpr()
{
    printf("ParseASExpr()\n");
    if (AtEnd())
    {
        SetError("EquationParser::ParseASExpr() arrived at unexpected end.");
        return nullptr;
    }
    
    auto term = ParseMDExpr();
    if (AtEnd() || IsError())
        return term;
    /*
        We may want to change the beginning token type to nop
    */
    auto asnode = new ASNode();
    asnode->AddExpr(term, TokenType::CROSS);

    while (PeekType() == TokenType::CROSS || PeekType() == TokenType::DASH)
    {
        auto type = PeekType();
        Consume();
        auto expr = ParseMDExpr();
        if (IsError() || expr == nullptr)
            return nullptr;
        asnode->AddExpr(expr, type);
    }
    return asnode;
}

ExprNode *EquationParser::ParseUnaryExpr()
{
    printf("ParseUnaryExpr()\n");
    if (!ConsumeIfEqual(TokenType::DASH))
    {
        SetError("EquationParser::ParseUnaryExpr() expected -.");
        return nullptr;
    }
    auto term = ParseTermNode();
    if (term != nullptr)
    {
        term->Negate(); // *= -1
    }
    return term;
}

ExprNode *EquationParser::ParseTermNode()
{
    printf("ParseTermNode()\n");
    if (AtEnd())
    {
        SetError("EquationParser::ParseTermNode() arrived at unexpected end.");
        return nullptr;
    }

    auto tok = Peek();
    auto type = tok->GetType();

    switch (type)
    {
        case TokenType::NUMBER:
        {
            Consume();
            if (!AtEnd())
            {
                auto next = Peek();
                auto nextType = next->GetType();
                if (nextType == TokenType::VARIABLE)
                {
                    Consume();
                    if (AtEnd())
                        return new TermNode(std::stod(tok->GetLexeme()), next->GetLexeme(), 1.0f);
                    
                    if (ConsumeIfEqual(TokenType::POW))
                    {
                        if (AtEnd())
                        {
                            SetError("Expected number operand for ^.");
                            return nullptr;
                        }

                        auto exp = Peek();
                        if (!ConsumeIfEqual(TokenType::NUMBER))
                        {
                            SetError("Expected number operand for ^.");
                            return nullptr;
                        }
                        return new TermNode(std::stod(tok->GetLexeme()), next->GetLexeme(), std::stod(exp->GetLexeme()));
                    }

                    return new TermNode(std::stod(tok->GetLexeme()), next->GetLexeme(), 1.0f);

                }
                else
                {
                    return new TermNode(std::stod(tok->GetLexeme()));
                }
            }
            else
            {
                return new TermNode(std::stod(tok->GetLexeme()));
            }
        }
        case TokenType::VARIABLE:
        {
            Consume();
            if (!AtEnd())
            {
                auto next = Peek();
                auto nextType = PeekNext();

                if (ConsumeIfEqual(TokenType::POW))
                {
                    if (AtEnd())
                    {
                        SetError("Expected number operand for ^.");
                        return nullptr;
                    }

                    auto exp = Peek();
                    if (!ConsumeIfEqual(TokenType::NUMBER))
                    {
                        SetError("Expected number operand for ^.");
                        return nullptr;
                    }
                    return new TermNode(1.0f, tok->GetLexeme(), std::stod(exp->GetLexeme()));
                }
                return new TermNode(1.0f, tok->GetLexeme(), 1.0f);
            }
            else
            {
                return new TermNode(1.0f, tok->GetLexeme(), 1.0f);
            }
        }
        case TokenType::LPAREN:
        {
            Consume();
            auto expr = ParseMDExpr();
            if (IsError())
                return nullptr;

            if (!ConsumeIfEqual(TokenType::RPAREN))
            {
                SetError("Missing closing parentheses.");
                return nullptr;
            }

            if (!ConsumeIfEqual(TokenType::POW))
                return expr;
            auto exp = Peek();
            if (!ConsumeIfEqual(TokenType::NUMBER))
            {
               SetError("Expected number operand for ^.");
                return nullptr;
            }
            return new TermNode(1.0f, expr, std::stod(exp->GetLexeme()));   
        }
        case TokenType::DASH:
        {
            return ParseUnaryExpr();
        }
        default:
        {
            SetError("EquationParser::ParseTermNode() could not parse.");
            return nullptr;
        }
    }
}

void EquationParser::Consume()
{
    m_CurrentToken++;
}

bool EquationParser::ConsumeIfEqual(TokenType type)
{
    auto token = Peek();
    if (token == nullptr)
        return false;
    if(token->GetType() == type)
    {
        m_CurrentToken++;
        return true;
    }
    return false;
}

Token *EquationParser::Peek()
{
    if (AtEnd())
        return nullptr;
    return m_Tokens[m_CurrentToken];
}

Token *EquationParser::PeekNext()
{
    return PeekAhead(1);
}

Token *EquationParser::PeekAhead(int n)
{
    if (m_CurrentToken + n >= m_Tokens.size())
        return nullptr;
    return m_Tokens[m_CurrentToken+n];
}

bool EquationParser::AtEnd()
{
    return m_CurrentToken >= m_Tokens.size();
}

bool EquationParser::IsError()
{
    return m_HasError;
}

void EquationParser::SetError(const std::string& err)
{
    m_ErrorLog += err + "\n";
    m_HasError = true;
}

TokenType EquationParser::PeekType()
{
    if (m_CurrentToken >= m_Tokens.size())
        return (TokenType)-1;
    return m_Tokens[m_CurrentToken]->GetType();
}
