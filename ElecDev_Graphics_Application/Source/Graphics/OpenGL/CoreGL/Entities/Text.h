#pragma once

// The first 4 vertices in the text vertex buffer contains the text box.

//=============================================================================================================================================//
//  Includes.																																   //
//=============================================================================================================================================//

#include "Entity.h"
#include <string>
#include <map>

//=============================================================================================================================================//
//  Forward declarations.																													   //
//=============================================================================================================================================//

template<typename VertexType>
class VertexArrayObject;
class VertexDataTextured;
struct Character;
struct Font;

//=============================================================================================================================================//
//  Class.																																	   //
//=============================================================================================================================================//

template<typename VertexType=VertexDataTextured>
class Text : public Entity<VertexType>
{
public:

	// ------------ //
	//  E N T I T Y //
	// ------------ //

	using Entity<VertexType>::m_vertices;
	using Entity<VertexType>::m_indices;
	using Entity<VertexType>::m_vertexCount;
	using Entity<VertexType>::m_indexCount;
	using Entity<VertexType>::m_VAO;
	using Entity<VertexType>::m_bufferStartIndex;
	using Entity<VertexType>::m_trackedCenter;
	using Entity<VertexType>::m_colour;
	using Entity<VertexType>::m_entityID;

	// Constructor.
	Text(std::string text, glm::vec3& position, glm::vec4& color, float scale,
		 VertexArrayObject<VertexType>* vao, Font& font, ManagedEntity* parent,
		 std::string horizontalAlignment = "C", std::string verticalAlignment = "B");
	// Destructor.
	~Text();

	// Text box manipulation.
	void setBoxColour(glm::vec4 colour);
};

//=============================================================================================================================================//
//  EOF.																																	   //
//=============================================================================================================================================//
