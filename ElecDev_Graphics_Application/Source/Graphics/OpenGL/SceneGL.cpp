//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include "OpenGL/Buffers/FrameBufferObjectGL.h"
#include "OpenGL/Buffers/VertexArrayObjectGL.h"
#include "OpenGL/Primitives/Vertex.h"
#include "OpenGL/SceneGL.h"
#include "Graphics/Camera/Camera.h"
#include "Graphics/Entities/Entity.h"
#include "OpenGL/Primitives/TextureGL.h"
#include "OpenGL/Primitives/Primitive.h"
#include "OpenGL/Renderer/RendererGL.h"
#include "Application/Events/Events.h"
#include "imgui/imgui.h"

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

Scene::~Scene() 
{
	m_primitives.clear();
	Renderer::doneSceneDestruction();
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

void Scene::onUpdate() 
{ 
	m_camera->onUpdate(); 
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

void Scene::clearFBO()
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

unsigned Scene::getEntityID(const glm::vec2& pixelCoords)
{
	// Adjust the pixel coords.
	return m_FBO->getEntityID({pixelCoords.x, m_camera->m_viewportVec[3] - pixelCoords.y});
}

void Scene::deleteGPUResources() 
{
	m_FBO->deleteResources();
}

void Scene::recreateGPUResources() 
{
	m_FBO->createResources(m_camera->m_viewportVec[2], m_camera->m_viewportVec[3]);
}

//==============================================================================================================================================//
//  Coordinates.																															    //
//==============================================================================================================================================//

glm::vec3 Scene::pixelCoordsToWorldCoords(const glm::vec2& pixelCoords)
{
	return m_camera->pixelCoordsToWorldCoords(pixelCoords);
}

glm::vec3 Scene::pixelCoordsToCameraCoords(const glm::vec2& pixelCoords) 
{ 
	return m_camera->pixelCoordsToCameraCoords(pixelCoords); 
}

//==============================================================================================================================================//
//  Backgrounds.																																	//
//==============================================================================================================================================//

void Scene::createDefaultBackground() 
{
	if      (m_camera->m_type == CameraType::Standard2D) create2DBackground();
	else if (m_camera->m_type == CameraType::Standard3D) create2DBackground();
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
	m_backgroundVAO->setBufferIncrementSize(4);

	// Make the background color a lighter version of the GUI background.
	/*glm::vec4 guiBackground = ImGui::GetStyle().Colors[ImGuiCol_WindowBg];
	glm::vec4 colorDark = guiBackground * m_colorScaleDark;
	glm::vec4 colorLight = guiBackground * m_colorScaleLight;*/

	// Assign background data.
	glm::vec4 bgColor2((float)222 / 255, (float)255 / 255, (float)255 / 255, 1.f);
	glm::vec4 bgColor1((float)182 / 255, (float)200 / 255, (float)255 / 255, 1.f);
	glm::vec3 pos1(1.0f, 1.0f, 0.99f);
	glm::vec3 pos2(-1.0f, 1.0f, 0.99f);
	glm::vec3 pos3(-1.0f, -1.0f, 0.99f);
	glm::vec3 pos4(1.0f, -1.0f, 0.99f);

	// Vertices.
	m_backgroundVAO->m_vertexCPU.emplace_back(VertexData(pos1, bgColor2, -1)); //  Top right.
	m_backgroundVAO->m_vertexCPU.emplace_back(VertexData(pos2, bgColor1, -1)); //  Top left.
	m_backgroundVAO->m_vertexCPU.emplace_back(VertexData(pos3, bgColor1, -1)); //  Bottom left.
	m_backgroundVAO->m_vertexCPU.emplace_back(VertexData(pos4, bgColor1, -1)); //  Bottom right.
	m_backgroundVAO->m_vertexCount += 4;

	// Indices.
	m_backgroundVAO->m_indexCPU.insert(m_backgroundVAO->m_indexCPU.end(), { 0,1,2,2,3,0 });
	m_backgroundVAO->m_indexCount += 6;

	// Data will be set on the first resize.
	m_backgroundVAO->queryBufferResize();
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
	if (event.isConsumed()) return;

}

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//