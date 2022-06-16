//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include "OpenGL/Renderer/RendererGL.h"
#include "OpenGL/SceneGL.h"
#include "OpenGL/ShaderGL.h"
#include "OpenGL/ErrorHandlerGL.h"
#include "OpenGL/Primitives/Grid.h"
#include "Lumen/Lumen.h"
#include "Graphics/Camera/Camera.h"
#include "Utilities/Profiler/Profiler.h"
#include "Application/Application.h"
#include "Utilities/Logger/Logger.h"

//==============================================================================================================================================//
//  General.																																	//
//==============================================================================================================================================//

void Renderer::bindScene(Scene* scene)
{
	s_scene = scene;
}

void Renderer::unbindScene()
{
	s_scene = nullptr;
}

Scene* Renderer::getScene()
{
	return s_scene;
}

void Renderer::initSceneDestruction(Scene* scene)
{
	storeAndBindScene(scene);
}

void Renderer::doneSceneDestruction()
{
	// Restore scene.
	if (s_scene != s_storedScenes.back()) 
	{ 
		s_scene = s_storedScenes.back();
	}
	else									
	{ 
		s_scene = nullptr;		 
	}
	// Remove stored scene.
	s_storedScenes.pop_back();
}

void Renderer::storeAndBindScene(Scene* scene) 
{
	s_storedScenes.push_back(s_scene);
	Renderer::bindScene(scene);	
}

void Renderer::restoreAndUnbindScene() 
{
	Renderer::bindScene(s_storedScenes.back());
	s_storedScenes.pop_back();
}

//==============================================================================================================================================//
//  Rendering.																																	//
//==============================================================================================================================================//

// --------------- //
//  G E N E R A L  //
// --------------- //

void Renderer::renderScene()
{
	if (s_scene) Renderer::renderScene(s_scene);
	else LUMEN_LOG_WARN("No scene bound.", "Renderer");
}

void Renderer::renderScene(Scene* scene)
{
	LUMEN_PROFILE_SCOPE("Render Scene");

	// Prepare scene for rendering.
	scene->onRenderInit();

	// Dispatch pipeline.
	switch (scene->getCamera().getType())
	{
		case CameraType::Standard2D:
			renderingPipeline2D(scene);
			break;

		case CameraType::Standard3D:
			renderingPipeline3D(scene);
			break;

		default:
			LUMEN_LOG_ERROR("Scene has unknown camera type.", "Renderer");
			return;
	}

	// Done with rendering.
	scene->onRenderCleanup();
}

void Renderer::backgroundPass(Scene* scene) 
{
	// Setup.
	Renderer::enable(GL_DEPTH_TEST);
	
	// Draw background.
	s_shaders["BackgroundShader"]->bind();
	Renderer::setDepthFunc(GL_ALWAYS);
	Renderer::drawBufferIndexed(*scene->m_backgroundBuffer.get());
	Renderer::setDepthFunc(GL_LESS);
}

void Renderer::gridPass(Scene* scene) 
{
	// Setup.
	Renderer::enable(GL_DEPTH_TEST);
	Renderer::enable(GL_BLEND);

	// Setup shader.
	Shader* shader = s_shaders["BasicShader"].get();
	Grid& grid = scene->getGrid();
	Camera& camera = scene->getCamera();
	shader->bind();
	shader->setMat4("viewProjMatrix", grid.getViewProjectionMatrix(camera));

	// Draw grid.	
	Renderer::setDepthFunc(GL_ALWAYS);
	Renderer::drawBufferIndexed(*grid.m_fineBuffer.get());
	Renderer::drawBufferIndexed(*grid.m_coarseBuffer.get());
	Renderer::drawBufferIndexed(*grid.m_originBuffer.get());
	Renderer::setDepthFunc(GL_LESS);
}

// ----------------------- //
//  2 D   P I P E L I N E  //
// ----------------------- //

void Renderer::renderingPipeline2D(Scene* scene) 
{
	if (Renderer::s_pipelineControls["Background"])
	{
		Renderer::backgroundPass(scene);
	}
	
	if (Renderer::s_pipelineControls["Grid"] && scene->m_grid->isEnabled())
	{
		Renderer::gridPass(scene);
	}

	if (Renderer::s_pipelineControls["Geometry"])
	{
		Renderer::geometryPass2D(scene);
	}

	if (Renderer::s_pipelineControls["Outline"])
	{
		Renderer::objectOutliningPass2D(scene);
	}
}

void Renderer::geometryPass2D(Scene* scene)
{
	LUMEN_RENDER_PASS();

	// ----------- //
	//  S E T U P  //
	// ----------- //

	Renderer::enable(GL_DEPTH_TEST);
	Renderer::enable(GL_BLEND);

	// ------------------- //
	//  R E N D E R I N G  //
	// ------------------- //

	// The shader used in rendering.
	Shader* shader = nullptr;
	Camera& camera = scene->getCamera();

	// Draw basic primitives.
	shader = s_shaders["BasicShader"].get();
	shader->bind();
	shader->setMat4("viewProjMatrix", camera.getViewProjectionMatrix());
	Renderer::drawBufferIndexed(*scene->m_linesBuffer.get());
	Renderer::drawBufferIndexed(*scene->m_trianglesBuffer.get());

	// Draw Circles.
	shader = s_shaders["CircleShader"].get();
	shader->bind();
	shader->setMat4("viewProjMatrix", camera.getViewProjectionMatrix());
	Renderer::drawBufferIndexed(*scene->m_circlesBuffer.get());

	// Draw textured primitives.
	shader = s_shaders["TextureShader"].get();
	shader->bind();
	shader->setMat4("viewProjMatrix", camera.getViewProjectionMatrix());
	Renderer::loadTextures(scene);
	Renderer::drawBufferIndexed(*scene->m_texturedTrianglesBuffer.get());
}

void Renderer::objectOutliningPass2D(Scene* scene) 
{
	LUMEN_RENDER_PASS();

	// Render outline with post processing.
	Renderer::setDepthFunc(GL_ALWAYS);
	if (Renderer::s_pipelineControls["OutlinePostProc"]) 
	{
		Shader* shader = nullptr;
		Camera& camera = scene->getCamera();
		int samples = (int)scene->m_msaaFBO.getAttachment(FrameBufferAttachmentSlot::COLOR_0).samples;
		shader = s_shaders["OutlinePostProc"].get();
		shader->bind();
		shader->setFloat("width",  camera.getViewportSize().x * samples);
		shader->setFloat("height", camera.getViewportSize().y * samples);
		Renderer::drawTextureOverFBOAttachment(scene->m_msaaFBO, FrameBufferAttachmentSlot::COLOR_0, scene->m_msaaFBO.getAttachment(FrameBufferAttachmentSlot::COLOR_2).rendererID, shader);
	}
	// Render outline texture directly.
	else
	{
		Renderer::drawTextureOverFBOAttachment(scene->m_msaaFBO, FrameBufferAttachmentSlot::COLOR_0, scene->m_msaaFBO.getAttachment(FrameBufferAttachmentSlot::COLOR_2).rendererID, s_shaders["StaticTextureShader"].get());
	}

	Renderer::setDepthFunc(GL_LESS);
}

// ----------------------- //
//  3 D   P I P E L I N E  //
// ----------------------- //

void Renderer::renderingPipeline3D(Scene* scene)
{
	Renderer::geometryPass3D(scene);
}

void Renderer::geometryPass3D(Scene* scene)
{
	LUMEN_RENDER_PASS();

	// ----------- //
	//  S E T U P  //
	// ----------- //

	Renderer::enable(GL_BLEND);
	Renderer::setViewport(scene->getCamera().getViewport());
	
	// ------------------- //
	//  R E N D E R I N G  //
	// ------------------- //

	// The shader used in rendering.
	Shader* shader = nullptr;
	Camera& camera = scene->getCamera();

	// Draw basic primitives.
	shader = s_shaders["BasicShader"].get();
	shader->bind();
	shader->setMat4("viewProjMatrix", camera.getViewProjectionMatrix());
	Renderer::drawBufferIndexed(*scene->m_linesBuffer.get());
	Renderer::drawBufferIndexed(*scene->m_trianglesBuffer.get());

	// Draw textured primitives.
	shader = s_shaders["TextureShader"].get();
	shader->bind();
	shader->setMat4("viewProjMatrix", camera.getViewProjectionMatrix());
	Renderer::loadTextures(scene);
	Renderer::drawBufferIndexed(*scene->m_texturedTrianglesBuffer.get());

	// Draw Circles.
	shader = s_shaders["CircleShader"].get();
	shader->bind();
	shader->setMat4("viewProjMatrix", camera.getViewProjectionMatrix());
	Renderer::drawBufferIndexed(*scene->m_circlesBuffer.get());
}

//==============================================================================================================================================//
//  Default Scenes.																																//
//==============================================================================================================================================//

void Renderer::generateDefaultScenes()
{
	m_default2DScene = std::make_unique<Scene>(CameraType::Standard2D, glm::vec2(500,500));
	// Render the scene once so that the background can be generated.
	Renderer::renderScene(m_default2DScene.get());
}

unsigned Renderer::getDefault2DSceneTexture()
{
	return m_default2DScene->getRenderTexture();
}

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//