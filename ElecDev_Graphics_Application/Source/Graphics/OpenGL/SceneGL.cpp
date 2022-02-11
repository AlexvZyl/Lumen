//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include "OpenGL/Buffers/FrameBufferObjectGL.h"
#include "OpenGL/Buffers/VertexArrayObjectGL.h"
#include "OpenGL/Entities/Vertex.h"
#include "OpenGL/SceneGL.h"
#include "OpenGL/CameraGL.h"
#include "OpenGL/Entities/Entity.h"
#include "OpenGL/TextureGL.h"
#include "OpenGL/Entities/Primitive.h"
#include "Application/Events/Events.h"

//==============================================================================================================================================//
//  Constructor & Destructor.																													//
//==============================================================================================================================================//

Scene::Scene(CameraType cameraType, float width, float height, unsigned msaaSamples) 
{
	// FBO.
	m_FBO					= std::make_unique<FrameBufferObject>(width, height, msaaSamples);
	// Camera.
	m_camera				= std::make_unique<Camera>(cameraType, width, height);
	// VAO's.
	m_linesVAO				= std::make_unique<VertexArrayObject<VertexData>>(GL_LINES);
	m_trianglesVAO			= std::make_unique<VertexArrayObject<VertexData>>(GL_TRIANGLES);
	m_texturedTrianglesVAO  = std::make_unique<VertexArrayObject<VertexDataTextured>>(GL_TRIANGLES);
	m_circlesVAO			= std::make_unique<VertexArrayObject<VertexDataCircle>>(GL_TRIANGLES);
	// Background.
	createDefaultBackground();
}

//==============================================================================================================================================//
//  Camera Methods.																																//
//==============================================================================================================================================//

glm::mat4* Scene::getViewMatrix() 
{ 
	return &m_camera->m_viewMatrix; 
}

glm::mat4* Scene::getProjectionMatrix() 
{ 
	return &m_camera->m_projectionMatrix; 
}

void Scene::updateCamera() 
{ 
	m_camera->updateCamera(); 
}

glm::vec4& Scene::getViewport() 
{ 
	return m_camera->m_viewportVec;
}

void Scene::setViewport(int width, int height) 
{ 
	m_camera->setViewport(width, height); 
}

//==============================================================================================================================================//
//  FBO Methods.																															    //	
//==============================================================================================================================================//

void Scene::bindFBO() 
{	
	m_FBO->bind();
}

void Scene:: clearFBO()
{
	m_FBO->clear();
}

void Scene::unbindFBO() 
{
	m_FBO->unbind();
}

unsigned Scene::getRenderTexture() 
{ 
	return m_FBO->getRenderTexture(); 
}

unsigned Scene::getEntityID(glm::vec2& pixelCoords)
{
	// Adjust the pixel coords.
	glm::vec2 pixelCoordsTemp = { pixelCoords[0], m_camera->m_viewportVec[3] - pixelCoords[1] };
	return m_FBO->getEntityID(pixelCoordsTemp);
}

//==============================================================================================================================================//
//  Coordinates.																															    //
//==============================================================================================================================================//

glm::vec3 Scene::pixelCoordsToWorldCoords(float pixelCoords[2])  
{ 
	return m_camera->pixelCoordsToWorldCoords(pixelCoords);  
}

glm::vec3 Scene::pixelCoordsToCameraCoords(float pixelCoords[2]) 
{ 
	return m_camera->pixelCoordsToCameraCoords(pixelCoords); 
}

//==============================================================================================================================================//
//  Backgrounds.																																	//
//==============================================================================================================================================//

void Scene::createDefaultBackground() 
{
	if      (m_camera->m_type == CameraType::Standard2D) create2DBackground();
	else if (m_camera->m_type == CameraType::Standard3D) create2DBackground();  // TODO: Implement skybox.
}

void Scene::create2DBackground()
{
	// TODO: Background vertices used to render the gradient.
	// 
	//			1 ----- 2 ----- 3 ----- 4
	//			|	    |	    |	    |
	//			|	    |       |	    |
	//			5 ----- 6 ----- 7 ----- 8
	//			|	    |	    |	    |
	//			|       |	    |	    |
	//			9 ----- 10 ---- 11 ---- 12
	//			|	    |	    |	    |
	//			|	    |	    |	    |
	//			13 ---- 14 ---- 15 ---- 16
	
	// Create the VAO.
	m_backgroundVAO = std::make_unique<VertexArrayObject<VertexData>>(GL_TRIANGLES);
	// Assign background data.
	glm::vec4 bgColor1((float)182 / 255, (float)200 / 255, (float)255 / 255, 1.f);
	glm::vec4 bgColor2((float)222 / 255, (float)255 / 255, (float)255 / 255, 1.f);
	glm::vec3 pos1(1.0f, 1.0f, 0.99f);
	glm::vec3 pos2(-1.0f, 1.0f, 0.99);
	glm::vec3 pos3(-1.0f, -1.0f, 0.99);
	glm::vec3 pos4(1.0f, -1.0f, 0.99);
	// Create and append the vertices.
	std::vector<std::unique_ptr<VertexData>> vertices;
	vertices.reserve(4);
	vertices.emplace_back(std::make_unique<VertexData>(pos1, bgColor2, -1)); //  Top right.
	vertices.emplace_back(std::make_unique<VertexData>(pos2, bgColor1, -1)); //  Top left.
	vertices.emplace_back(std::make_unique<VertexData>(pos3, bgColor1, -1)); //  Bottom left.
	vertices.emplace_back(std::make_unique<VertexData>(pos4, bgColor1, -1)); //  Bottom right.
	// Create background.
	std::vector<unsigned> indices({ 0,1,2,2,3,0 });
	m_backgroundVAO->appendVertexData(vertices, indices);
}

void Scene::create3DBackground() 
{
	// Implement skybox.
}

//==============================================================================================================================================//
//  Events.																																		//
//==============================================================================================================================================//

void Scene::resize(int width, int height) 
{
	m_camera->resize(width, height);
	m_FBO->resize(width, height);
}

void Scene::onEvent(Event& event) 
{

}

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//