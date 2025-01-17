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
#include "OpenGL/ShaderGL.h"
#include "Application/Events/Events.h"
#include "imgui/imgui.h"

//==============================================================================================================================================//
//  Constructor & Destructor.																													//
//==============================================================================================================================================//

Scene::Scene(CameraType cameraType, const glm::vec2& size) 
{
	FrameBufferAttachment attachment;

	// Start with MSAA FBO.
	attachment.samples = Renderer::MSAA;
	// Default color.
	attachment.slot = FrameBufferAttachmentSlot::COLOR_0;
	attachment.type = FrameBufferAttachmentType::TEXTURE_BUFFER;
	attachment.internalFormat = FrameBufferTextureFormat::RGBA;
	attachment.format = FrameBufferTextureFormat::RGBA;
	m_msaaFBO.addAttachment(attachment);
	// Entity ID.
	attachment.slot = FrameBufferAttachmentSlot::COLOR_1;
	attachment.type = FrameBufferAttachmentType::TEXTURE_BUFFER;
	attachment.internalFormat = FrameBufferTextureFormat::R32_UI;
	attachment.format = FrameBufferTextureFormat::RED_INTEGER;
	m_msaaFBO.addAttachment(attachment);
	// Outline.
	attachment.slot = FrameBufferAttachmentSlot::COLOR_2;
	attachment.type = FrameBufferAttachmentType::TEXTURE_BUFFER;
	attachment.internalFormat = FrameBufferTextureFormat::RGBA;
	attachment.format = FrameBufferTextureFormat::RGBA;
	m_msaaFBO.addAttachment(attachment);
	// Depth Stencil.
	attachment.slot = FrameBufferAttachmentSlot::DEPTH_STENCIL;
	attachment.type = FrameBufferAttachmentType::TEXTURE_BUFFER;
	attachment.internalFormat = FrameBufferTextureFormat::DEPTH_24_STENCIL_8;
	attachment.format = FrameBufferTextureFormat::DEPTH_COMPONENT;
	attachment.minFilter = FrameBufferTextureFilter::NEAREST;
	attachment.magFilter = FrameBufferTextureFilter::NEAREST;
	m_msaaFBO.addAttachment(attachment);
	// Create the FBO with its attachments.
	m_msaaFBO.setDrawBuffers({ FrameBufferAttachmentSlot::COLOR_0, FrameBufferAttachmentSlot::COLOR_1, FrameBufferAttachmentSlot::COLOR_2 });
	m_msaaFBO.create();

	// Normal FBO.
	attachment.samples = FrameBufferSamples::NORMAL;
	attachment.minFilter = FrameBufferTextureFilter::LINEAR;
	attachment.magFilter = FrameBufferTextureFilter::LINEAR;
	// Default color.
	attachment.slot = FrameBufferAttachmentSlot::COLOR_0;
	attachment.type = FrameBufferAttachmentType::TEXTURE_BUFFER;
	attachment.internalFormat = FrameBufferTextureFormat::RGBA;
	attachment.format = FrameBufferTextureFormat::RGBA;
	m_renderFBO.addAttachment(attachment);
	// Entity ID.
	attachment.slot = FrameBufferAttachmentSlot::COLOR_1;
	attachment.type = FrameBufferAttachmentType::TEXTURE_BUFFER;
	attachment.internalFormat = FrameBufferTextureFormat::R32_UI;
	attachment.format = FrameBufferTextureFormat::RED_INTEGER;
	m_renderFBO.addAttachment(attachment); 
	// Outline.
	attachment.slot = FrameBufferAttachmentSlot::COLOR_2;
	attachment.type = FrameBufferAttachmentType::TEXTURE_BUFFER;
	attachment.internalFormat = FrameBufferTextureFormat::RGBA;
	attachment.format = FrameBufferTextureFormat::RGBA;
	m_renderFBO.addAttachment(attachment);
	// Depth Stencil.
	attachment.slot = FrameBufferAttachmentSlot::DEPTH_STENCIL;
	attachment.type = FrameBufferAttachmentType::TEXTURE_BUFFER;
	attachment.internalFormat = FrameBufferTextureFormat::DEPTH_24_STENCIL_8;
	attachment.format = FrameBufferTextureFormat::DEPTH_COMPONENT;
	attachment.minFilter = FrameBufferTextureFilter::NEAREST;
	attachment.magFilter = FrameBufferTextureFilter::NEAREST;
	m_renderFBO.addAttachment(attachment);
	// Create the FBO with its attachments.
	m_renderFBO.setDrawBuffers({ FrameBufferAttachmentSlot::COLOR_0, FrameBufferAttachmentSlot::COLOR_1, FrameBufferAttachmentSlot::COLOR_2 });
	m_renderFBO.create();

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

unsigned Scene::getRenderTexture() 
{ 
	return m_renderFBO.getAttachment(FrameBufferAttachmentSlot::COLOR_0).rendererID;
}

unsigned Scene::getEntityID(const glm::vec2& pixelCoords)
{
	return m_renderFBO.readPixel(FrameBufferAttachmentSlot::COLOR_1, (int)pixelCoords.x, (int)pixelCoords.y);
}

void Scene::deleteGPUResources() 
{
	m_msaaFBO.destroy();
	m_renderFBO.destroy();
}

void Scene::recreateGPUResources() 
{
	m_msaaFBO.create((int)getCamera().getViewport()[2], (int)getCamera().getViewport()[3]);
	m_renderFBO.create((int)getCamera().getViewport()[2], (int)getCamera().getViewport()[3]);
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
	if (m_renderFBO.isOnGPU())m_renderFBO.resize((int)size.x, (int)size.y);
	if (m_msaaFBO.isOnGPU()) m_msaaFBO.resize((int)size.x, (int)size.y);
}

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//