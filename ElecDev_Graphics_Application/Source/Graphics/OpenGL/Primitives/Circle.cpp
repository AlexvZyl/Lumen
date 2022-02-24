//=============================================================================================================================================//
//  Includes.																																   //
//=============================================================================================================================================//

#include "OpenGL/Primitives/Circle.h"
#include "OpenGL/Primitives/Vertex.h"
#include "Graphics/Entities/EntityManager.h"
#include "OpenGL/Buffers/VertexArrayObjectGL.h"

//=============================================================================================================================================//
//  Constructor & Destructor.																												   //
//=============================================================================================================================================//

Circle::Circle(VertexArrayObject<VertexDataCircle>* VAO, const glm::vec3& center, float radius, const glm::vec4& color, float thickness, float fade, Entity* parent)
	: Primitive<VertexDataCircle>(parent)
{
	// Init.
	m_VAO = VAO;
	m_trackedCenter = center;
	m_colour = color;
	m_thickness = thickness;
	m_fade = fade;

	// Create the vertices.
	std::vector<VertexDataCircle> vertices;
	vertices.reserve(4);
	vertices.emplace_back(VertexDataCircle(glm::vec3(center.x - radius, center.y + radius, center.z), local1, m_colour, m_thickness, m_fade, m_entityID));
	vertices.emplace_back(VertexDataCircle(glm::vec3(center.x + radius, center.y + radius, center.z), local2, m_colour, m_thickness, m_fade, m_entityID));
	vertices.emplace_back(VertexDataCircle(glm::vec3(center.x + radius, center.y - radius, center.z), local3, m_colour, m_thickness, m_fade, m_entityID));
	vertices.emplace_back(VertexDataCircle(glm::vec3(center.x - radius, center.y - radius, center.z), local4, m_colour, m_thickness, m_fade, m_entityID));

	// Create the indices.
	std::vector<unsigned> indices = {0,1,2,2,3,0};
	m_vertexCount = 4;
	m_indexCount = 6;

	// Add to VAO.
	m_VAO->pushPrimitive(this, vertices, indices);
} 

Circle::Circle(VertexArrayObject<VertexDataCircle>* vao, const glm::vec2& center, float radius, const glm::vec4& color, float thickness, float fade, Entity* parent)
	: Circle( vao, glm::vec3(center, 0.f), radius, color, thickness, fade, parent)
{}


//=============================================================================================================================================//
//  EOF.																																	   //
//=============================================================================================================================================//
