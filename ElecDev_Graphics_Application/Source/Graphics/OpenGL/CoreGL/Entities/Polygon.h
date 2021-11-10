#pragma once

//=============================================================================================================================================//
//  Includes.																																   //
//=============================================================================================================================================//

#include <vector>
#include <glm.hpp>
#include "Entity.h"

//=============================================================================================================================================//
//  Forward dclerations.																													   //
//=============================================================================================================================================//

class VertexArrayObject;

//=============================================================================================================================================//
//  Class.																																	   //
//=============================================================================================================================================//

class Polygon2D : public Entity
{
public:

	//polygon shape attributes
	unsigned n_vertices;

	Polygon2D() = default;

	//Adds a polygon with the given vertices centred at (0,0) to a VBO
	Polygon2D(std::vector<glm::vec3> vertices, VertexArrayObject* VAO);

	//Creates a regular polygon with n_vertices vertices centred at (0, 0)
	//**should change this to only accept GL_TRIANGLE type VAOs**
	//Polygon2D(unsigned n_vertices, VertexArrayObject* VAO);

	//Adds a polygon with the given vertices centred at (0,0) to a VBO
	Polygon2D(std::vector<glm::vec3> vertices, VertexArrayObject* VAO);
	//Remove a polygon from the VBO and deconstruct
	~Polygon2D();
	//change the layer in which the polygon is drawn
	void setLayer(float layer);
};

//=============================================================================================================================================//
//  EOF.																																	   //
//=============================================================================================================================================//