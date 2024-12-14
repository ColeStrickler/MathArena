#ifndef MATHARENA_H
#define MATHARENA_H
#include <vector>
#include <istream>
#include "grid.h"
#include "geometry.h"
#include "ast.h"
#include "shader.h"
#include "parser.h"
#include "renderer.h"
#include "logger.h"


#define DEFAULTXmin -5
#define DEFAULTYmin -5
#define DEFAULTZmin -5
#define DEFAULTXmax 5
#define DEFAULTYmax 5
#define DEFAULTZmax 5
#define DEFAULTRESOLUTION 500



class MathArena
{
public:
    MathArena();
    ~MathArena();


    void NewEquation(const std::string& equationString);
    
    void RemoveEquation(int index);

    void SetGridLimits(int xmin, int xmax, int ymin, int ymax, int zmin, int zmax);

    bool HasError() const { return m_HasError; };
    std::string GetErrorLog() const { return m_ErrorLog; };

private:
    // Helper Functions
    bool Setup_GenShaderProg();

    // Utility Functions
    void Error(const std::string &err);
    void AddEquation(EquationNode *equation);
    std::vector<Token*> HandleNewEquationScan(const std::string& eq);
    EquationNode* HandleNewEquationParse(std::vector<Token*>& tokens);
    // Diagnostics
    std::string m_ErrorLog;
    bool m_HasError;

    // Settings
    int m_Xmin;
    int m_Ymin;
    int m_Zmin;
    int m_Xmax;
    int m_Ymax;
    int m_Zmax;
    int m_Resolution;

    // State
    std::vector<Function3D *> m_ActiveEquations;
    ShaderProgram* m_FunctionShader;
    Grid* m_Grid;
};

#endif