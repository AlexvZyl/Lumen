#pragma once

//=============================================================================================================================================//
//  Includes.																																   //
//=============================================================================================================================================//

#include <vector>
#include <glm.hpp>
#include "OpenGL/Primitives/Primitive.h"

//=============================================================================================================================================//
//  Forward dclerations.																													   //
//=============================================================================================================================================//

template<typename VertexType>
class VertexArrayObject;

class VertexData;

//=============================================================================================================================================//
//  Class.																																	   //
//=============================================================================================================================================//

class Polygon2D : public Primitive<VertexData>
{
public:				
	
	// Constructors.
	Polygon2D(const std::vector<glm::vec3>& vertices, VertexArrayObject<VertexData>* VAO, Entity* parent);
};

//=============================================================================================================================================//
//  EOF.																																	   //
//=============================================================================================================================================//