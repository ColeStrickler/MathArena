#include "grid.h"

extern Logger logger;

Grid::Grid(int xmin, int ymin, int zmin, int xmax, int ymax, int zmax) : m_xMin(xmin), m_yMin(ymin), m_zMin(zmin), m_xMax(xmax), m_yMax(ymax), m_zMax(zmax)
{

    for (int x = m_xMin; x < m_xMax; x++)
    {
        for (int z = m_yMin; z < m_zMax; z++)
        {
            m_Points.push_back(GridVertex{glm::vec3(static_cast<float>(x), 0.0f, static_cast<float>(z))});
        }
    }
    //m_Points.push_back(GridVertex{glm::vec3(0.0f, 0.0f, 0.0f)});

    VertexBuffer* vb = new VertexBuffer((float*)m_Points.data(), m_Points.size() * sizeof(GridVertex));
    BufferLayout* gridVBLayout = new BufferLayout({new BufferElement("COORDS", ShaderDataType::Float3, false) });
    vb->SetLayout(gridVBLayout);
    VertexArray* va = new VertexArray();
     
    va->AddVertexBuffer(vb);
    va->SetCount(m_Points.size());

   
    Shader* gridVertexShader = new Shader(util::getcwd() + "/src/shaders/vertex_points.glsl", GL_VERTEX_SHADER);
    if (gridVertexShader->CheckError() != ShaderError::NO_ERROR_OK)
        logger.Log(LOGTYPE::ERROR, gridVertexShader->FetchLog());
    Shader* gridFragmentShader = new Shader(util::getcwd() + "/src/shaders/fragment_points.glsl", GL_FRAGMENT_SHADER);
    if (gridFragmentShader->CheckError() != ShaderError::NO_ERROR_OK)
        logger.Log(LOGTYPE::ERROR, gridFragmentShader->FetchLog());
    Shader* gridGeometryShader = new Shader(util::getcwd() + "/src/shaders/geometry.glsl", GL_GEOMETRY_SHADER);
    if (gridGeometryShader->CheckError() != ShaderError::NO_ERROR_OK)
        logger.Log(LOGTYPE::ERROR, gridGeometryShader->FetchLog());

    ShaderProgram* sp = new ShaderProgram();
    sp->AddShader(gridVertexShader);
    sp->AddShader(gridFragmentShader);
    sp->AddShader(gridGeometryShader);
    m_GridShader = sp;
    if (!m_GridShader->Compile())
    {
        logger.Log(LOGTYPE::ERROR, "Sphere::Sphere --> Unable to compile Grid Shader.");
        logger.Log(LOGTYPE::ERROR, "Error #" + std::to_string(m_GridShader->CheckError()));
        while (1)
            ;
    }

    m_RenderObj = new RenderObject(va, vb, m_GridShader, OBJECTYPE::PointsObject);
}

Grid::~Grid()
{
}
