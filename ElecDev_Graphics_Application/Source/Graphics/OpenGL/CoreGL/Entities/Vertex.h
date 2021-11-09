#pragma once

/*
The parent class that contains all of the information that a vertex type can contain.
BufferLayout is used to setup the VAO.
*/

//=============================================================================================================================================//
//  Includes.																																   //
//=============================================================================================================================================//

#include "GLAD/glad.h"
#include <vector>

//=============================================================================================================================================//
//  Forward declerations.																													   //
//=============================================================================================================================================//
 
 

//=============================================================================================================================================//
//  Vertex Layout Struct.																													   //
//=============================================================================================================================================//

// Used to describe the layout of the VAO for the vertex type.
struct BufferLayout 
{
	GLuint index;
	GLint size;
	GLenum type;
	GLboolean normalized;
	GLsizei stride;
	const GLvoid* pointer;
};

//=============================================================================================================================================//
//  Class.																																	   //
//=============================================================================================================================================//

class Vertex
{

private:

public:
	std::vector<BufferLayout> m_bufferLayout;
};

//=============================================================================================================================================//
//  EOF.																																	   //
//=============================================================================================================================================//