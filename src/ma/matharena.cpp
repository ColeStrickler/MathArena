#include "matharena.h"

extern Renderer renderer;
extern Logger logger;

MathArena *matharena;

MathArena::MathArena() : m_Resolution(DEFAULTRESOLUTION)
{
    if (!Setup_GenShaderProg())
    {
        m_HasError = true;
    }
    SetGridLimits(DEFAULTXmin, DEFAULTXmax, DEFAULTYmin, DEFAULTYmax, DEFAULTZmin, DEFAULTZmax);

    // Add Grid to renderer
    m_Grid = new Grid();
    renderer.AddRenderObject(m_Grid->m_RenderObj);
}

MathArena::~MathArena()
{
    while (m_ActiveEquations.size())
        RemoveEquation(0);
}

void MathArena::AddEquation(EquationNode *equation)
{
    auto func = new Function3D(static_cast<float>(DEFAULTXmax), static_cast<float>(DEFAULTYmax),
                               static_cast<float>(DEFAULTZmax), m_Resolution, m_FunctionShader, equation);
                                  
    m_ActiveEquations.push_back(func);
    renderer.AddRenderObject(func->m_RenderObj);
}

void MathArena::NewEquation(const std::string &equationString)
{
    auto tokens = HandleNewEquationScan(equationString);
    if (!tokens.size())
        return;

    auto eqNode = HandleNewEquationParse(tokens);
    if (eqNode == nullptr)
        return;

    /*
        We will put in some logic to solve the equation here
    */

    AddEquation(eqNode);
}

void MathArena::RemoveEquation(int index)
{
    assert(index >= 0 && index < m_ActiveEquations.size());
    auto equation = m_ActiveEquations[index];
    m_ActiveEquations.erase(m_ActiveEquations.begin() + index);
    delete equation; // handle m_RenderObj properly inside Function3D deconstructor
}

void MathArena::SetGridLimits(int xmin, int xmax, int ymin, int ymax, int zmin, int zmax)
{
    m_Xmin = xmin;
    m_Xmax = xmax;
    m_Ymin = ymin;
    m_Ymax = ymax;
    m_Zmin = zmin;
    m_Zmax = zmax;
}

bool MathArena::Setup_GenShaderProg()
{
    Shader *vertex_shader = new Shader(util::getcwd() + "/src/shaders/vertex.glsl", GL_VERTEX_SHADER);
    if (vertex_shader->CheckError() != ShaderError::NO_ERROR_OK)
        logger.Log(LOGTYPE::ERROR, vertex_shader->FetchLog());
    Shader *fragment_shader = new Shader(util::getcwd() + "/src/shaders/fragment.glsl", GL_FRAGMENT_SHADER);
    if (fragment_shader->CheckError() != ShaderError::NO_ERROR_OK)
        logger.Log(LOGTYPE::ERROR, fragment_shader->FetchLog());

    m_FunctionShader = new ShaderProgram;
    m_FunctionShader->AddShader(vertex_shader);
    m_FunctionShader->AddShader(fragment_shader);

    if (!m_FunctionShader->Compile())
    {
        Error("MathArena::Setup_GenShaderProg() ==> unable to compile FunctionShader. Error # " + std::to_string(m_FunctionShader->CheckError()) + "\n");
        return false;
    }
    return true;
}

void MathArena::Error(const std::string &err)
{
    m_ErrorLog += err + "\n";
}

std::vector<Token *> MathArena::HandleNewEquationScan(const std::string &eq)
{
    auto istream = new std::istringstream(eq);
    auto scanner = new EquationScanner(istream);
    bool err = false;
    std::vector<Token *> tokens;
    while (true)
    {
        Token *out = nullptr;
        auto type = scanner->GetNextToken(&out);
        if (type == TokenType::EQERROR)
        {
            logger.Log(LOGTYPE::ERROR, "Scanner error at token " + std::to_string(eq[scanner->m_colNum]) + ":" + std::to_string(scanner->m_colNum));
            goto error;
        }
        else if (type == TokenType::WHITESPACE)
            continue;
        else if (type == TokenType::END)
            break;
        tokens.push_back(out);
    }

    return tokens;
error:
    for (auto &tok : tokens)
        delete tok;
    return {};
}

EquationNode *MathArena::HandleNewEquationParse(std::vector<Token *> &tokens)
{

    auto parser = new EquationParser(tokens);
    if (!parser->Parse())
    {
        logger.Log(LOGTYPE::ERROR, parser->GetErrorString());
        return nullptr;
    }
    else
        logger.Log(LOGTYPE::INFO, parser->GetEquation()->toString());

    auto eq = parser->GetEquation();
    delete parser;
    return eq;
}
