#pragma once
#include "Vertex.h"
#include "Polygon.h"
#include <memory>
#include <boost/utility/base_from_member.hpp>
class LineSegment :
    private boost::base_from_member< glm::vec2, 1>,
    private boost::base_from_member< glm::vec2, 2>,
    public Polygon2D<VertexData> {
    using 
};
{
private:
    glm::vec2 m_direction;
    glm::vec2 m_perpendicular;

public:
    LineSegment(glm::vec2 start, glm::vec2 end, VertexArrayObject<VertexData>* VAO, ManagedEntity* parent, float thickness = 0.1f, glm::vec4 colour = { 0.f, 0.f, 0.f, 1.f });

};

