//=============================================================================================================================================//
//  Includes.																																   //
//=============================================================================================================================================//

#include "Circle.h"
#include "CoreGL/VertexArrayObjectGL.h"
#include "CoreGL/Entities/Vertex.h"
#include "CoreGL/Entities/EntityManager.h"

//=============================================================================================================================================//
//  Constructor & Destructor.																												   //
//=============================================================================================================================================//

template<typename VertexType>
Circle<VertexType>::Circle(VertexArrayObject<VertexType>* vao, glm::vec3 center, float radius, glm::vec4& color, float thickness, float fade, ManagedEntity* parent):Entity<VertexType>(parent)
{
	// Assign variables.
	m_VAO = vao;
	m_trackedCenter = center;
	m_colour = color;
	//m_entityID = EntityManager::generateEID();
	m_thickness = thickness;
	m_fade = fade;
	// Create position variables.
	glm::vec3 pos1(center.x - radius, center.y + radius, center.z);
	glm::vec3 pos2(center.x + radius, center.y + radius, center.z);
	glm::vec3 pos3(center.x + radius, center.y - radius, center.z);
	glm::vec3 pos4(center.x - radius, center.y - radius, center.z);
	// Create vertices.
	m_vertices =
	{
		VertexDataCircle(pos1, local1, m_colour, m_thickness, m_fade, m_entityID),
		VertexDataCircle(pos2, local2, m_colour, m_thickness, m_fade, m_entityID),
		VertexDataCircle(pos3, local3, m_colour, m_thickness, m_fade, m_entityID),
		VertexDataCircle(pos4, local4, m_colour, m_thickness, m_fade, m_entityID)
	};
	m_vertexCount = 4;
	m_indices = {0,1,2,2,3,0};
	m_indexCount = 6;
	// Add to VAO.
	m_VAO->appendDataCPU(this);
}

template<typename VertexType>
Circle<VertexType>::Circle(VertexArrayObject<VertexType>* vao, glm::vec2 center, float radius, glm::vec4& color, float thickness, float fade, ManagedEntity* parent)
	: Circle( vao, glm::vec3(center, 0.f), radius, color, thickness, fade, parent)
{}

template<typename VertexType>
Circle<VertexType>::~Circle() {}

//=============================================================================================================================================//
//  Instantiations.																															   //
//=============================================================================================================================================//

template class Circle<VertexDataCircle>;

//=============================================================================================================================================//
//  EOF.																																	   //
//=============================================================================================================================================//
