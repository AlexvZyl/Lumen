#pragma once

/*
All entities have to be a child of this class.  The data contained in this class is all that is required 
for a VAO to be able to render the entity to the screen.
*/

//=============================================================================================================================================//
//  Includes.																																   //
//=============================================================================================================================================//

#include <vector>
#include <glm.hpp>
#include <memory>
#include "Entity.h"

//=============================================================================================================================================//
//  Forward declerations.																													   //
//=============================================================================================================================================//

template<typename VertexType>
class VertexArrayObject;
struct GUIState;

//=============================================================================================================================================//
//  Class.																																	   //
//=============================================================================================================================================//

template<typename VertexType>
class Primitive: public Entity
{
public:

	// ------------------- //
	//  V A R I A B L E S  //
	// ------------------- //

	VertexArrayObject<VertexType>* m_VAO;			// Pointer to the VAO that the entity is drawn to.
	std::vector<VertexType> m_vertices;				// The untextured vertex data used for the entity.
	std::vector<unsigned int> m_indices;			// The index buffer used to draw the entity.
	unsigned int m_vertexCount = 0;					// Counts the amount of vertices.
	unsigned int m_indexCount = 0;					// Counts the amount of indices.
	unsigned int m_bufferStartIndex = 0 ;			// The start position of the entity in the VAO.
	unsigned int m_indecesStartIndex = 0;			// The position in the indeces buffer.
	glm::vec4 m_colour = {0.f, 0.f, 0.f, 1.f};		// Saves the global color for the entity.
	glm::vec3 m_trackedCenter = {0.f,0.f,0.f};		// Gives the option to track the center of the entity.
													// Useful for rotation, scaling and moving to a point.

	// ------------------------------------------------- //
	//  C O N S T R U C T O R   &   D E S T R U C T O R  //
	// ------------------------------------------------- //

	// Default constructor.
	Primitive();
	// Constructor that sets the context as well.
	Primitive(Entity* parent);
	// Destructor.
	~Primitive();

	// ----------------- //
	//  M O V E M E N T  //
	// ----------------- //

	// Translate the entity by the given vector.
	virtual void translate(glm::vec3& translation);
	virtual void translate(glm::vec2& translation);
	// Translate the entity to the given position.
	virtual void translateTo(glm::vec3& position);
	//Translates the entity in the XY plane, keeping the same Z value.
	virtual void translateTo(glm::vec2& position);
	// Rotates the entity by the given vector and center point.
	virtual void rotate(glm::vec3& rotation);
	// Scales the entity by the given vector and center point.
	virtual void scale(glm::vec3& scaling);
	
	// --------------------- //
	//  A T T R I B U T E S  //
	// --------------------- //

	// Sets the color for all of the vertices.
	virtual void setColor(glm::vec4& color);
	// Sets the entty ID of the entity.
	virtual void setEntityID(unsigned int eID);
	// Set the entity later.
	virtual void setLayer(float layer);
	// Set context.
	virtual void setContext(GUIState* guiState);

	// ------------- //
	//  M E M O R Y  //
	// ------------- //

	// Deletes the entity and clears the memory.
	virtual void wipeMemory();
	// Offsets the entities' indices to match the VAO.
	virtual void offsetIndices(int offset);
};

//=============================================================================================================================================//
//  EOF.																																	   //
//=============================================================================================================================================//
