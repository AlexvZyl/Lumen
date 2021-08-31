/*
This is where the drawing enigine is handled.  This is only used to draw elements to the screen.
The interactive engine (the one where elements can be drawn is handled in designEngine).
*/

//----------------------------------------------------------------------------------------------------------------------
//  Includes.
//----------------------------------------------------------------------------------------------------------------------

#include "core.h"

//----------------------------------------------------------------------------------------------------------------------
//  Constructors.
//----------------------------------------------------------------------------------------------------------------------

// Default.
//DrawingEngineGL::DrawingEngineGL() {};

// With GLFW window.
DrawingEngineGL::DrawingEngineGL(GLFWwindow* window)
{	
	// Save pointer to GLFW window.
	this->window = window;

	// Create shader.
	std::string shaderFilePath = "Source\\Graphics\\OpenGL\\ShaderHandler\\Source\\basicShader.shader";
	Shader basicShader(shaderFilePath);
	this->basicShader = basicShader;

	// Find the viewpwort dimensions and store it.
	int viewport[2];
	glfwGetWindowSize(this->window, &viewport[0], &viewport[1]);
	// Store the value to use when viewport changes.
	this->viewportDimensions[0] = viewport[0];
	this->viewportDimensions[1] = viewport[1];

	// Matrices setup.
	//----------------------------------------------------------
	
	// Find the minimum value of the viewport dimensions.
	int minValue;
	if (viewport[0] < viewport[1]) { minValue = viewport[0]; }
	else { minValue = viewport[1]; }
	// Scale the projection values according to the viewport aspect ratio.
	float projValuesTemp[6] = {(float)-viewport[0]/minValue, (float)viewport[0]/minValue, (float)-viewport[1]/minValue, (float)viewport[1]/minValue,-1.0, 1.0 };
	// Save projection values to be used with resizing of the window.
	for (int i = 0; i < 6; i++) { this->projectionValues[i] = projValuesTemp[i]; }
	// Create projection matrix.
	this->projectionMatrix = glm::ortho(this->projectionValues[0], this->projectionValues[1], this->projectionValues[2], this->projectionValues[3], -1.0f, 1.0f);

	// Assign original projection values.
	this->modelMatrix = glm::mat4(1.0f);
	this->viewMatrix = glm::mat4(1.0f);
	this->viewportMatrix = glm::mat4(1.0f);
	this->scalingMatrix = glm::mat4(1.0f);
	this->translationMatrix = glm::mat4(1.0f);
	this->rotationMatrix = glm::mat4(1.0f);

	// Assign matrices to shader.
	this->basicShader.use();
	this->basicShader.setMat4("worldMatrix", this->modelMatrix);
	this->basicShader.setMat4("projectionMatrix", this->projectionMatrix);
	this->basicShader.setMat4("viewMatrix", this->viewMatrix);

	// Print success message.
	std::cout << "[OPENGL SHADER] Shaders compiled.\n\n";

	// Mouse event variables.
	this->scaleRate = 0.3;

	// create the triangle
	float triangle_vertices[] = {
		0.0f, 0.25f, 0.0f,	// position vertex 1
		1.0f, 0.0f, 0.0f,	 // color vertex 1
		0.25f, -0.25f, 0.0f,  // position vertex 1
		0.0f, 1.0f, 0.0f,	 // color vertex 1
		-0.25f, -0.25f, 0.0f, // position vertex 1
		0.0f, 0.0f, 1.0f,	 // color vertex 1
	};
	unsigned int triangle_indices[] = {
		0, 1, 2 };

	VertexArray VAO;
	VertexBuffer VBO(triangle_vertices, 4 * 2 * sizeof(float));
	VertexBufferLayout VBL;
	VBL.push<float>(2);
	VAO.addBuffer(VBO, VBL);
	IndexBuffer IBO(triangle_indices, 3);

	this->VAO = VAO;
	this->IBO = IBO;
};

//----------------------------------------------------------------------------------------------------------------------
//  Rendering.
//----------------------------------------------------------------------------------------------------------------------

void DrawingEngineGL::renderLoop()
{
	// Apply camera movements to shader.
	this->viewMatrix = this->scalingMatrix * this->rotationMatrix * this->translationMatrix;
	this->basicShader.setMat4("viewMatrix", this->viewMatrix);

	// rendering our geometries
	this->basicShader.use();
	this->VAO.bind();
	GLCall(glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0));
	GLCall(glBindVertexArray(0));

	// Draw temporary border.
	GLCall(glBegin(GL_LINE_LOOP));
	glVertex2f(-1.0f, 1.0f);
	glVertex2f(1.0f, 1.0f);
	glVertex2f(1.0f, -1.0f);
	glVertex2f(-1.0f, -1.0f);
	glEnd();

}

//----------------------------------------------------------------------------------------------------------------------
//  Coordinate systems.
//----------------------------------------------------------------------------------------------------------------------

// Function that takes pixel coordinates as input and return the coordinates in the world.
glm::vec4 DrawingEngineGL::pixelCoordsToWorldCoords(double pixelCoords[2])
{
	// The coordinates on the screen.
	double screenCoords[2];  
	// Find the viewpwort dimensions.
	int viewport[2];
	glfwGetWindowSize(this->window, &viewport[0], &viewport[1]);
	// Account for pixel offset.
	float viewportOffset[2] = { (float)viewport[0], (float)viewport[1] };
	// OpenGL places the (0,0) point in the top left of the screen.  Place it in the bottom left cornder.
	double pixelCoordsTemp[2] = { pixelCoords[0] , (double)viewport[1] - pixelCoords[1] };
	
	// Apply the viewport transform the the pixels.
	screenCoords[0] = (pixelCoordsTemp[0] - viewportOffset[0] / 2) / (viewportOffset[0] / 2);
	screenCoords[1] = (pixelCoordsTemp[1] - viewportOffset[1] / 2) / (viewportOffset[1] / 2);
	// Convert to screen vector.
	glm::vec4 screenVec = { screenCoords[0], screenCoords[1], 0, 1 };

	// Apply MVP matrices.
	this->viewMatrix = this->scalingMatrix * this->rotationMatrix * this->translationMatrix;
	glm::mat4 MVPinverse = glm::inverse(this->modelMatrix * this->viewMatrix * this->projectionMatrix);
	glm::vec4 worldVec = screenVec * MVPinverse ;

	return worldVec;
}

//----------------------------------------------------------------------------------------------------------------------
//  Window functions.
//----------------------------------------------------------------------------------------------------------------------

// Function that handles engine resizing.
// Viewport changes are made in the main Applicatioon since it affects everything.
void DrawingEngineGL::resizeEvent(int width, int height)
{
	// Calculate the value of the scaling.
	double scalingFactor[2] = { (double)width / (double)this->viewportDimensions[0], (double)height / (double)this->viewportDimensions[1] };
	this->viewportDimensions[0] = width;
	this->viewportDimensions[1] = height;
		
	// Scale projection values.
	this->projectionValues[0] *= scalingFactor[0];
	this->projectionValues[1] *= scalingFactor[0];
	this->projectionValues[2] *= scalingFactor[1];
	this->projectionValues[3] *= scalingFactor[1];

	// Arrange order of sacling based on if it should be division or multiplication.
	if (scalingFactor[1] < 1)  //  Minimizing.
	{
		// Create new projection matrix.
		this->projectionMatrix = glm::ortho(this->projectionValues[0], this->projectionValues[1], this->projectionValues[2], this->projectionValues[3], this->projectionValues[4], this->projectionValues[5]);
		//// Scale the drawing so that it stays the same size relative to the viewport.
		//this->projectionMatrix = glm::scale(this->projectionMatrix, glm::vec3(scalingFactor[1], scalingFactor[1], 1));
	
	}
	else  //  Maximizing.
	{
		// Create new projection matrix.
		this->projectionMatrix = glm::ortho(this->projectionValues[0], this->projectionValues[1], this->projectionValues[2], this->projectionValues[3], this->projectionValues[4], this->projectionValues[5]);
		// Scale the drawing so that it stays the same size relative to the viewport.
		this->projectionMatrix = glm::scale(this->projectionMatrix, glm::vec3(scalingFactor[1], scalingFactor[1], 1));
	}

	// Apply changes to shaders.
	this->basicShader.setMat4("projectionMatrix", this->projectionMatrix);
	
}

//----------------------------------------------------------------------------------------------------------------------
//  EOF.
//----------------------------------------------------------------------------------------------------------------------