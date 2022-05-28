#include "glad/glad.h"
#include <stddef.h>

class BufferObject
{
public:

	// Constructor should do nothing.
	BufferObject(int target);

	// Destructor.
	~BufferObject();

	// Create the buffer.
	void create();

	// Delete the buffer.
	void destroy();

	// Get the VBO ID.
	int ID();

	// Bind the VBO.
	void bind();

	// Unbind the VBO.
	void unbind();

	// Set the data. 
	void bufferData(int size, const void* data, int usage = GL_DYNAMIC_DRAW);

	// Set the data by name.
	void namedBufferData(int size, const void* data, int usage = GL_DYNAMIC_DRAW);

	// Set the buffer sub data.
	void bufferSubData(int offset, int size, const void* data);

	// Set the buffer sub data by name.
	void namedBufferSubData(int offset, int size, const void* data);

	// Get the capacity.
	int capacity();

private:

	// Data.
	int m_target = NULL;
	unsigned m_rendererID = NULL;
	int m_capacity = NULL;
	bool m_existsOnGPU = false;
};

class VertexBufferObject : public BufferObject
{
public:
	VertexBufferObject() : BufferObject(GL_ARRAY_BUFFER) { }
};

class IndexBufferObject : public BufferObject
{
public:
	IndexBufferObject() : BufferObject(GL_ELEMENT_ARRAY_BUFFER) { }
};