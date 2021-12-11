#pragma once
#include "CoreGL/Entities/LineSegment.h"
#include "CoreGL/Entities/Entity.h"
#include "Port.h"
/*
#include "CoreGL/VertexArrayObjectGL.h"
#include <glm.hpp>
#include <vector>*/
//#include "CoreGL/Entities/Vertex.h"

class Circuit;

enum class LineOrientation
{
    HORIZONTAL,
    VERTICAL
};

class Cable :
    public Entity
{
private:
    Port* m_startPort;
    Port* m_endPort = nullptr;
    VertexArrayObject<VertexData>* engine_triangleVAO;
    VertexArrayObject<VertexDataCircle>* engine_circleVAO;
    float m_thickness = 0.005f;
    glm::vec4 m_colour = { 0.5f, 0.5f, 0.5f, 0.5f };
    //std::vector<glm::vec2> m_vertices;
    std::vector<std::shared_ptr<LineSegment>> m_lines;
    std::vector<std::shared_ptr<Circle<VertexDataCircle>>> m_nodes;
    LineOrientation m_curOrientation = LineOrientation::HORIZONTAL;


public:
    
    Cable(Port* startPort, VertexArrayObject<VertexData>* triangleVAO, VertexArrayObject<VertexDataCircle>* circleVAO, Circuit* parent);
    void extendSegment(glm::vec2 nextPoint);
    void extendPrevSegment(glm::vec2 nextPoint);
    void addSegment(glm::vec2 nextPoint);
    void setContext(GUIState* guiState);
    void attach(Port* endPort);
    void followPort(Port* movedPort);
    void setColour(glm::vec4 colour);
};

