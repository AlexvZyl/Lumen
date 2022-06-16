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
#include "OpenGL/Renderer/RendererGL.h"
#include "OpenGL/ErrorHandlerGL.h"
#include "OpenGL/Primitives/Grid.h"
#include "Application/Events/Events.h"
#include "imgui/imgui.h"

//==============================================================================================================================================//
//  Constructor & Destructor.																													//
//==============================================================================================================================================//

Scene::Scene(CameraType cameraType, const glm::vec2& size) 
{
	FrameBufferAttachment attachment;

	// Default color.
	attachment.slot = FrameBufferAttachmentSlot::COLOR_0;
	attachment.type = FrameBufferAttachmentType::TEXTURE_BUFFER;
	attachment.internalFormat = FrameBufferTextureFormat::RGBA;
	attachment.format = FrameBufferTextureFormat::RGBA;
	m_FBO.addAttachment(attachment);

	// Entity ID.
	attachment.slot = FrameBufferAttachmentSlot::COLOR_1;
	attachment.type = FrameBufferAttachmentType::TEXTURE_BUFFER;
	attachment.internalFormat = FrameBufferTextureFormat::R32_UI;
	attachment.format = FrameBufferTextureFormat::RED_INTEGER;
	m_FBO.addAttachment(attachment);

	// Outline.
	attachment.slot = FrameBufferAttachmentSlot::COLOR_2;
	attachment.type = FrameBufferAttachmentType::TEXTURE_BUFFER;
	attachment.internalFormat = FrameBufferTextureFormat::RGBA;
	attachment.format = FrameBufferTextureFormat::RGBA;
	m_FBO.addAttachment(attachment);

	// Depth Stencil.
	attachment.slot = FrameBufferAttachmentSlot::DEPTH_STENCIL;
	attachment.type = FrameBufferAttachmentType::RENDER_BUFFER;
	attachment.internalFormat = FrameBufferTextureFormat::DEPTH_24_STENCIL_8;
	attachment.format = FrameBufferTextureFormat::DEPTH_24_STENCIL_8;
	m_FBO.addAttachment(attachment);

	// Create the FBO with its attachments.
	m_FBO.setDrawBuffers({ FrameBufferAttachmentSlot::COLOR_0, FrameBufferAttachmentSlot::COLOR_1, FrameBufferAttachmentSlot::COLOR_2 });
	m_FBO.create();

	// Camera.
	m_camera				= std::make_unique<Camera>(cameraType, size);

	// VAO's.
	m_linesBuffer				= std::make_unique<GraphicsLinesBuffer<VertexData>>();
	m_trianglesBuffer			= std::make_unique<GraphicsTrianglesBuffer<VertexData>>();
	m_texturedTrianglesBuffer	= std::make_unique<GraphicsTrianglesBuffer<VertexDataTextured>>();
	m_circlesBuffer				= std::make_unique<GraphicsTrianglesBuffer<VertexDataCircle>>();

	// Grid.
	Renderer::storeAndBindScene(this);
	m_grid = std::make_unique<Grid>();
	Renderer::restoreAndUnbindScene();

	// Background.
	// Currently not in use.
	createDefaultBackground();
}

Scene::~Scene() 
{
	m_grid = nullptr;
	m_primitives.clear();
	Renderer::doneSceneDestruction();
}

Camera& Scene::getCamera()
{
	return *m_camera.get();
}

Grid& Scene::getGrid() 
{
	return *m_grid.get();
}

//==============================================================================================================================================//
//  FBO Methods.																															    //	
//==============================================================================================================================================//

void Scene::onRenderInit() 
{
	m_FBO.bind();
	m_FBO.clear();
	m_FBO.bindDrawBuffers();
	getCamera().onUpdate();
}

void Scene::onRenderCleanup() 
{
	m_FBO.unbind();
}

unsigned Scene::getRenderTexture() 
{ 
	return m_FBO.getAttachment(FrameBufferAttachmentSlot::COLOR_0).rendererID;
}

unsigned Scene::getEntityID(const glm::vec2& pixelCoords)
{
	return m_FBO.readPixel(FrameBufferAttachmentSlot::COLOR_1, (int)pixelCoords.x, (int)pixelCoords.y);
}

void Scene::deleteGPUResources() 
{
	m_FBO.destroy();
}

void Scene::recreateGPUResources() 
{
	m_FBO.create((int)getCamera().getViewport()[2], (int)getCamera().getViewport()[3]);
}

//==============================================================================================================================================//
//  Backgrounds.																																	//
//==============================================================================================================================================//

void Scene::createDefaultBackground() 
{
	if      (getCamera().getType() == CameraType::Standard2D) create2DBackground();
	else if (getCamera().getType() == CameraType::Standard3D) create2DBackground();
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
	m_backgroundBuffer = std::make_unique<GraphicsTrianglesBuffer<VertexData>>();
	m_backgroundBuffer->setCapacityIncrements(2);

	// Vertices.
	VertexData vertices[4] = {
		{ { 1.0f, 1.0f, 0.f   }, Renderer::backgroundColor, (unsigned) -1 }, //  Top right.
		{ { -1.0f, 1.0f, 0.f  }, Renderer::backgroundColor, (unsigned) -1 }, //  Top left.
		{ { -1.0f, -1.0f, 0.f }, Renderer::backgroundColor, (unsigned) -1 }, //  Bottom left.
		{ { 1.0f, -1.0f, 0.f  }, Renderer::backgroundColor, (unsigned) -1 }  //  Bottom right.
	};

	// Indices.
	const UInt3 indices[2] = {
		{ 0, 1, 2 },
		{ 2, 3, 0 }
	};

	// Push data.
	m_backgroundBuffer->push(vertices, 4, indices, 2);
}

void Scene::create3DBackground() 
{
	// Implement skybox.
}

//==============================================================================================================================================//
//  Events.																																		//
//==============================================================================================================================================//

void Scene::resize(const glm::vec2& size) 
{
	getCamera().resize(size);
	m_FBO.resize((int)size.x, (int)size.y);
}

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//