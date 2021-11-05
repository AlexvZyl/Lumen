#pragma once

//----------------------------------------------------------------------------------------------------------------------
//  Includes.
//----------------------------------------------------------------------------------------------------------------------

#include "../Base2DEngineGL/Base2D_Engine.h"
#include "Peripherals/MousePointGL.h"
#include "Peripherals/Component2D.h"

//----------------------------------------------------------------------------------------------------------------------
//  The Design Engine class.
//----------------------------------------------------------------------------------------------------------------------

class Design2DEngineGL : public Base2DEngineGL
{
public:

	//---------------------------------------------------------------------------------------------------------------------
	//  Variables.
	//----------------------------------------------------------------------------------------------------------------------

	// A virtual mouse point that is used to edit the circuit.
	MousePoint* m_mousePoint;
	std::vector<Component2D*> m_components;
	Component2D* m_activeComponent;

	//---------------------------------------------------------------------------------------------------------------------
	//  Constructor and Destructor.
	//----------------------------------------------------------------------------------------------------------------------

	// Constructor
	Design2DEngineGL(stateMachine* states);
	// Destructor.
	~Design2DEngineGL();

	//---------------------------------------------------------------------------------------------------------------------
	//  Rendering.
	//---------------------------------------------------------------------------------------------------------------------

	// Rendering loop.
	virtual void renderLoop();

	//---------------------------------------------------------------------------------------------------------------------
	//  API
	//---------------------------------------------------------------------------------------------------------------------

	//---------------------------------------------------------------------------------------------------------------------
	//  Window events.
	//---------------------------------------------------------------------------------------------------------------------

	virtual void resizeEvent(int width, int height);

	//---------------------------------------------------------------------------------------------------------------------
	//  Mouse events.
	//---------------------------------------------------------------------------------------------------------------------

	// Handling mouse events.
	virtual void mousePressLeft(float pixelCoords[2]);
	virtual void mousePressRight(float pixelCoords[2]);
	virtual void mouseMoveEvent(float pixelCoords[2], int buttonStateLeft, int buttonStateRight);
	virtual void mouseScrollEvent(float pixelCoords[2], float yOffset);

	//--------------------------------------------------------------------------------------------------------------------

};

//------------------------------------------------------------------------------------------------------------------------
//  EOF.
//------------------------------------------------------------------------------------------------------------------------