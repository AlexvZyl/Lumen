#pragma once
#include <vector>
#include <glm.hpp>
#include "VertexArrayObjectGL.h"


class Polygon2D
{

	//Private Members
private:
	//VBO members
	//Pointer to the VAO to use for the polygon
	VertexArrayObject* m_VAO;
	//indices for VBO access
	int start_idx;

	//polygon shape attributes
	unsigned n_vertices;
	std::vector<VertexData> m_vertices;

	//position of the polygon
	glm::vec2 m_pos;

	//other polygon attributes
	Colour m_colour;
	unsigned eID = 0;

	//Public Members
public:

	//Constructors

	//Creates a regular polygon with n_vertices vertices centred at (0, 0)
	//**should change this to only accept GL_TRIANGLE type VAOs**
	//Polygon2D(unsigned n_vertices, VertexArrayObject* VAO);

	//Adds a polygon with the given vertices to a VBO
	Polygon2D(std::vector<glm::vec3> vertices, VertexArrayObject* VAO);


	//Remove a polygon from the VBO and deconstruct
	~Polygon2D();

	//Draw the polygon
	void draw();

	//Render the polygon -- Possibly keep the rendering seperate --
	void render();

	//Perform a translation operation
	void translate(glm::vec2 transVec);

	//Perform a translation to the specified position
	void translateTo(glm::vec2 pos);
	
	//Perform a rotation operation
	void rotate(float angle);
};

