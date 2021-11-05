/*
This is where the interactive 2D design engine is implemented.
*/

//=============================================================================================================================================//
//  Includes.																																   //
//=============================================================================================================================================//

// Class include.
#include "Design2D_Engine.h"

//=============================================================================================================================================//
//  Constructor & Destructor.																												   //
//=============================================================================================================================================//

// Constructor.
Design2DEngineGL::Design2DEngineGL(stateMachine* states) : Base2DEngineGL(states)
{
	std::cout << blue << "\n[OPENGL] [INFO] : " << white << "Design 2D engine starting...";

	float color[4] = { 1.0f, 0.0f, 0.0f, 1.0f };
	m_mousePoint = new MousePoint(color, 0.015f, 20, &m_projectionMatrix, m_states);

	std::cout << blue << "\n[OPENGL] [INFO] : " << white << "Design 2D engine done.";
	
	m_components.insert(m_components.begin(), new Component2D());
	m_activeComponent = m_components[0];
}

// Destructor.
Design2DEngineGL::~Design2DEngineGL()
{
	// Call base destructor.
	Base2DEngineGL::~Base2DEngineGL();

	// Delete helpers.
	delete m_mousePoint;

	for (int i = 0; i < m_components.size(); i++) {
		delete m_components[i];
	}
	m_components.empty();
}

//=============================================================================================================================================//
//  Rendering.																																   //
//=============================================================================================================================================//

void Design2DEngineGL::renderLoop()
{
	// Call the base engine rendering loop.
	Base2DEngineGL::renderLoop();

	// ----------- //
	//  S E T U P  //
	// ----------- //

	// Render to the FBO.
	m_frameBuffer->bind();

	// ------------------- //
	//  R E N D E R I N G  //
	// ------------------- //

	// Helpers.
	m_basicShader->bind();
	m_activeComponent->render();
	m_mousePoint->render();

	// --------------- //
	//  C L E A N U P  //
	// --------------- //

	// Stop rendering to the FBO.
	m_frameBuffer->unbind();
}

//=============================================================================================================================================//
//  Events.																																	   //
//=============================================================================================================================================//

void Design2DEngineGL::resizeEvent(int width, int height)
{
	// Call base engine resize event.
	Base2DEngineGL::resizeEvent(width, height);

	// Now call resizing required for design engine.
	m_mousePoint->updateProjection();
}

//=============================================================================================================================================//
//  EOF.																																	   //
//=============================================================================================================================================//
