#pragma once

//----------------------------------------------------------------------------------------------------------------------
//  The drawingEngine class.
//----------------------------------------------------------------------------------------------------------------------

class DesignEngineGL
{
public:

	//---------------------------------------------------------------------------------------------------------------------
	//  Variables.
	//----------------------------------------------------------------------------------------------------------------------

	int x;

	//---------------------------------------------------------------------------------------------------------------------
	//  API
	//----------------------------------------------------------------------------------------------------------------------

	// Adds a line to the VBO object.
	void drawLine();

	// Adds a circle to the VBO object.
	void drawCircle();


	// Adds text to the VBO object.
	void drawText();

	// Displays the new drawing to the screen.
	// Required after each new element has been added.
	void display();

	//----------------------------------------------------------------------------------------------------------------------

};

//----------------------------------------------------------------------------------------------------------------------
//  EOF.
//----------------------------------------------------------------------------------------------------------------------