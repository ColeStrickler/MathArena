#ifndef GRID_H
#define GRID_H

#include <vector>
#include "geometry.h"
#include "glm/glm.hpp"

struct GridVertex
{
    glm::vec3 pos;
};


class Grid
{
public:

    Grid(int xmin=-20, int ymin=-20, int zmin=-20, int xmax=20, int ymax=20, int zmax=20);
    ~Grid();

    void SetXMin(int xMin);
    void SetYMin(int yMin);
    void SetZMin(int zMin);
    void SetXMax(int xMax);
    void SetYMax(int yMax);
    void SetZMax(int zMax);

    RenderObject* m_RenderObj;
    std::vector<GridVertex> m_Points;
private:
    int m_xMin;
    int m_yMin;
    int m_zMin;
    int m_xMax;
    int m_yMax;
    int m_zMax;
    
    ShaderProgram* m_GridShader;

};


#endif