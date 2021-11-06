#pragma once

//=============================================================================================================================================//
//  Includes.																																   //
//=============================================================================================================================================//

#include "../Base2DEngineGL/Base2D_Engine.h"
#include "Peripherals/Component2D.h"

#include "CoreGL/Polygon.h"

//=============================================================================================================================================//
//  Design 2D Engine class.																													   //
//=============================================================================================================================================//

class Design2DEngineGL : public Base2DEngineGL
{
public:

	// ------------------ //
	//  V A R I A B L E S //
	// ------------------ //

	std::vector<std::shared_ptr<Component2D>> m_components;
	std::shared_ptr<Component2D> m_activeComponent;
	Polygon2D* polygon;

	// ------------------------------------------------ //
	//  C O N S T R U C T O R   &   D E S T R U C T O R //
	// ------------------------------------------------ //

	// Constructor
	Design2DEngineGL(GUIState* guiState);
	// Destructor.
	~Design2DEngineGL();

	// ------------------- //
	//  R E N D E R I N G  //
	// ------------------- //

	// Rendering loop.
	virtual void renderLoop();

	// ------- //
	//  A P I  //
	// ------- //

	// --------------------------- //
	//  W I N D O W   E V E N T S  //
	// --------------------------- //

	virtual void resizeEvent(int width, int height);

	// -------------------------- //
	//  W I N D O W   E V E N T S //
	// -------------------------- //

	// Handling mouse events.
	virtual void mousePressLeft(float pixelCoords[2]);
	virtual void mousePressRight(float pixelCoords[2]);
	virtual void mouseMoveEvent(float pixelCoords[2], int buttonStateLeft, int buttonStateRight, int buttonStateMiddle);
	virtual void mouseScrollEvent(float pixelCoords[2], float yOffset);
};

//=============================================================================================================================================//
//  EOF.																																	   //
//=============================================================================================================================================//