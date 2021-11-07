//=============================================================================================================================================//
//  Includes.																																   //
//=============================================================================================================================================//

#include "Entities/Polygon.h"
#include "VertexArrayObjectGL.h"

//=============================================================================================================================================//
//  Constructor & Destructor.																												   //
//=============================================================================================================================================//

VertexArrayObject::VertexArrayObject(GLenum type, bool textured)
	: m_bufferType(type)
{
	// Generate the VAO.
	GLCall(glGenVertexArrays(1, &m_vAID));
	GLCall(glBindVertexArray(m_vAID));

	// Create untextured array.
	if (!textured)
	{
		// Generate a VBO for the VAO.
		GLCall(glGenBuffers(1, &m_vBID));
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_vBID));

		// Potition.
		GLCall(glEnableVertexArrayAttrib(m_vAID, 0));
		GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (const void*)offsetof(VertexData, position)));
		// Color.
		GLCall(glEnableVertexArrayAttrib(m_vAID, 1));
		GLCall(glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(VertexData), (const void*)offsetof(VertexData, color)));
		// Entity ID.
		GLCall(glEnableVertexArrayAttrib(m_vAID, 2));
		GLCall(glVertexAttribIPointer(2, 1, GL_UNSIGNED_INT, sizeof(VertexData), (const void*)offsetof(VertexData, entityID)));
	}
	// Create textured array.
	else
	{
		// Generate a VBO for the VAO.
		GLCall(glGenBuffers(1, &m_vBID));
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_vBID));
		// Bind Vertex position attribute.
		GLCall(glEnableVertexArrayAttrib(m_vAID, 0));
		GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexDataTextured), (const void*)offsetof(VertexDataTextured, position)));
		// Bind Vertex color attribute.
		GLCall(glEnableVertexArrayAttrib(m_vAID, 1));
		GLCall(glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(VertexDataTextured), (const void*)offsetof(VertexDataTextured, color)));
		// Bind texture position attribute
		GLCall(glEnableVertexArrayAttrib(m_vAID, 2));
		GLCall(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexDataTextured), (const void*)offsetof(VertexDataTextured, texturePosition)));
		// Bind texture ID attribute
		GLCall(glEnableVertexArrayAttrib(m_vAID, 3));
		GLCall(glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(VertexDataTextured), (const void*)offsetof(VertexDataTextured, textureID)));
		// Entity ID.
		GLCall(glEnableVertexArrayAttrib(m_vAID, 4));
		GLCall(glVertexAttribIPointer(4, 1, GL_UNSIGNED_INT, sizeof(VertexDataTextured), (const void*)offsetof(VertexDataTextured, entityID)));
	}
}

VertexArrayObject::~VertexArrayObject()
{
	// Delete VBO.
	GLCall(glDeleteBuffers(1, &m_vBID))
		// Delete VAO.
		GLCall(glDeleteVertexArrays(1, &m_vAID));
}

//=============================================================================================================================================//
//  Rendering.																																   //
//=============================================================================================================================================//

void VertexArrayObject::render()
{
	// Bind the VAO.
	GLCall(glBindVertexArray(m_vAID));
	// Draw call.
	GLCall(glDrawArrays(m_bufferType, 0, m_bufferIndex));
}

void VertexArrayObject::bind() const { GLCall(glBindVertexArray(m_vAID)); }

void VertexArrayObject::unbind() const { GLCall(glBindVertexArray(0)); }

//=============================================================================================================================================//
//  Vertex Data Untextured.  																												   //
//=============================================================================================================================================//

void VertexArrayObject::appendDataCPU(std::vector<VertexData> vertices)
{
	m_vertexDataCPU.insert(m_vertexDataCPU.end(), vertices.begin(), vertices.end());
}

void VertexArrayObject::assignDataCPU(std::vector<VertexData> vertices, unsigned int index)
{
	// Create the VAO if it is empty.
	if (!m_vertexDataCPU.size()) { appendDataCPU(vertices); }
	else 
	{
		for (VertexData& vertex : vertices)
		{
			m_vertexDataCPU.at(index) = vertex;
			index++;
		}
	}
}

void VertexArrayObject::assignDataGPU(std::vector<VertexData> vertices, unsigned int index)
{
	// Bind VBO.
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_vBID));
	// Populate with vertex data.
	for (VertexData& vertex : vertices)
	{
		// Write the data to the buffer.
		GLCall(glBufferSubData(GL_ARRAY_BUFFER, index * sizeof(vertex), sizeof(vertex) - sizeof(vertex.entityID), vertex.rawData()));
		GLCall(glBufferSubData(GL_ARRAY_BUFFER, index * sizeof(vertex) + offsetof(VertexData, entityID), sizeof(vertex.entityID), (const GLvoid*)vertex.entityID));
		index += 1;
	}
}

//=============================================================================================================================================//
//  Vertex Data Textured.																													   //
//=============================================================================================================================================//

void VertexArrayObject::appendDataCPU(std::vector<VertexDataTextured> vertices)
{
	m_vertexDataTexturedCPU.insert(m_vertexDataTexturedCPU.end(), vertices.begin(), vertices.end());
}

void VertexArrayObject::assignDataCPU(std::vector<VertexDataTextured> vertices, unsigned int index)
{
	// Create the VAO if it is empty.
	if (!m_vertexDataTexturedCPU.size()) { appendDataCPU(vertices); }
	else 
	{
		for (VertexDataTextured& vertex : vertices)
		{
			m_vertexDataTexturedCPU.at(index) = vertex;
			index++;
		}
	}
}

void VertexArrayObject::assignDataGPU(std::vector<VertexDataTextured> vertices, unsigned int index)
{
	// Bind VBO.
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_vBID));
	// Populate with vertex data.
	for (VertexDataTextured& vertex : vertices)
	{
		// Write the data to the buffer.
		GLCall(glBufferSubData(GL_ARRAY_BUFFER, index * sizeof(vertex), sizeof(vertex) - sizeof(vertex.entityID), vertex.rawData()));
		GLCall(glBufferSubData(GL_ARRAY_BUFFER, index * sizeof(vertex) + offsetof(VertexDataTextured, entityID), sizeof(vertex.entityID), (const GLvoid*)vertex.entityID));
		index += 1;
	}
}

//=============================================================================================================================================//
//  Polygons.																																   //
//=============================================================================================================================================//

void VertexArrayObject::appendDataCPU(Polygon2D* polygon) 
{
	// Add the polygon to the vector.
	m_polygon2DCPU.push_back(polygon);
	polygon->start_idx = m_bufferIndex;
	m_bufferIndex += polygon->m_vertices.size();
	updateGPU();
}

void VertexArrayObject::assignDataGPU(Polygon2D* polygon) 
{
	assignDataGPU(polygon->m_vertices, polygon->start_idx);
}

void VertexArrayObject::deleteDataCPU(Polygon2D* polygon) 
{
	//m_polygon2DCPU.at
}

//=============================================================================================================================================//
//  Memory management.																															   //
//=============================================================================================================================================//

void VertexArrayObject::updateGPU()
{
	// A VAO can only contain one of the vertex data types.
	// --------------------------------------- //
	//  U N T E X T U R E D   V E R T I C E S  //
	// --------------------------------------- //
	if (m_vertexDataCPU.size())
	{
		// Reset the buffer pointer.
		unsigned int index = 0;
		// Bind VBO.
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_vBID));
		// Define buffer size.
		GLCall(glBufferData(GL_ARRAY_BUFFER, m_vertexDataCPU.size() * sizeof(VertexData), NULL, GL_DYNAMIC_DRAW));
		// Populate with vertex data.
		for (VertexData& vertex : m_vertexDataCPU)
		{
			// Write the data to the buffer.
			GLCall(glBufferSubData(GL_ARRAY_BUFFER, index * sizeof(vertex), sizeof(vertex) - sizeof(vertex.entityID), vertex.rawData()));
			GLCall(glBufferSubData(GL_ARRAY_BUFFER, index * sizeof(vertex) + offsetof(VertexData, entityID), sizeof(vertex.entityID), (const GLvoid*)vertex.entityID));
			index += 1;
		}
	}
	// ----------------------------------- //
	//  T E X T U R E D   V E R T I C E S  //
	// ----------------------------------- //
	else if (m_vertexDataTexturedCPU.size())
	{
		// Reset the buffer pointer.
		unsigned int index = 0;
		// Bind VBO
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_vBID));
		// Define buffer size.
		GLCall(glBufferData(GL_ARRAY_BUFFER, m_vertexDataTexturedCPU.size() * sizeof(VertexDataTextured), NULL, GL_DYNAMIC_DRAW));
		// Populate with vertex data.
		for (VertexDataTextured& vertex : m_vertexDataTexturedCPU)
		{
			// Write the data to the buffer.
			GLCall(glBufferSubData(GL_ARRAY_BUFFER, index * sizeof(vertex), sizeof(vertex) - sizeof(vertex.entityID), vertex.rawData()));
			GLCall(glBufferSubData(GL_ARRAY_BUFFER, index * sizeof(vertex) + offsetof(VertexDataTextured, entityID), sizeof(vertex.entityID), (const GLvoid*)vertex.entityID));
			index += 1;
		}
	}
	// ----------------- //
	//  P O L Y G O N S  //
	// ----------------- //
	else if (m_polygon2DCPU.size())
	{
		// Calculate the size of the new VBO.
		unsigned int vertexCount = 0;
		for (Polygon2D* polygon : m_polygon2DCPU) { vertexCount += polygon->m_vertices.size(); }
		// Reset the buffer pointer.
		unsigned int index = 0;
		// Bind VBO.
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_vBID));
		// Define buffer size.
		GLCall(glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(VertexData), NULL, GL_DYNAMIC_DRAW));
		// Populate with vertex data.
		for (Polygon2D* polygon : m_polygon2DCPU) 
		{
			for (VertexData& vertex : polygon->m_vertices)
			{
				// Write the data to the buffer.
				GLCall(glBufferSubData(GL_ARRAY_BUFFER, index * sizeof(vertex), sizeof(vertex) - sizeof(vertex.entityID), vertex.rawData()));
				GLCall(glBufferSubData(GL_ARRAY_BUFFER, index * sizeof(vertex) + offsetof(VertexData, entityID), sizeof(vertex.entityID), (const GLvoid*)vertex.entityID));
				index += 1;
			}
		}
	}
}

void VertexArrayObject::wipeCPU()
{
	if (m_vertexDataCPU.size()) { m_vertexDataCPU.clear(); m_vertexDataCPU.shrink_to_fit(); }
	if (m_vertexDataTexturedCPU.size()) { m_vertexDataTexturedCPU.clear(); m_vertexDataTexturedCPU.shrink_to_fit(); }
}

//=============================================================================================================================================//
//  EOF.																																	   //
//=============================================================================================================================================//