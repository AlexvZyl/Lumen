
//=============================================================================================================================================//
//  Includes.																																   //
//=============================================================================================================================================//

#include <glm.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include <vector>
#include "../VertexArrayObjectGL.h"
#include "Entity.h"

//=============================================================================================================================================//
//  Functions.																																   //
//=============================================================================================================================================//

void Entity::translate(glm::vec3 translation)
{
	for (VertexData& vertex : m_vertexData)
	{
		vertex.position += translation;
	}
}

//=============================================================================================================================================//
//  EOF.																																	   //
//=============================================================================================================================================//
