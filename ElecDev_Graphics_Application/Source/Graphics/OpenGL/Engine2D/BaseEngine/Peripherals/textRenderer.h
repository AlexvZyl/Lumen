#pragma once

/*
Class that handles the text rendring in the OpenGL engines.
Reads the font data from the .fnt file and stores it in a 
Character dictionary.  This allows the use of one texture to
render all of the text.
*/

//----------------------------------------------------------------------------------------------------------------------
//  Includes.
//----------------------------------------------------------------------------------------------------------------------

// General.
#include <iostream>

// Used for dictionary.
#include <map>

// OpenGL Engines incude.
#include "vertexArrayObject.h"

//----------------------------------------------------------------------------------------------------------------------
//  Class.
//----------------------------------------------------------------------------------------------------------------------

class TextRenderer
{

private:

	// Struct that contains the information regarding the font used in this renderer.
	struct Font
	{
		std::string name;			// Name of the font used.
		int padding=0;				// Padding around each character.
		int textureSize[2] = {0,0};	// x, y.  Size of the .png file.
	};

	// Stores the information regarding the characters.
	struct Character
	{
		float id=0;				// Character code in ascii.
		float x=0;				// Top left coordinate of character.
		float y=0;				// Top left coordinate of character
		float width=0;			// Width of character in pixels.
		float height=0;			// Height of the charcter in pixels
		float xOffset=0;		// Offset from the cursor psition.
		float yOffset=0;		// Offset from the cursor psition.
		float xAdvance=0;		// Amount that the cursor should move to draw a new character.
	};

	// Character dictionary that contains the info on each character.
	std::map<float, Character> m_characterDictionary;

public:

	// Constructor.  Load the .fnt file and reads it.
	TextRenderer();
	// Destructor.
	~TextRenderer();

	// Writes the text to the vao supplied.
	void writeText(std::string text, float coords[2], VertexArrayObject* vao, float textureID, float color[4], float scale);

};

//----------------------------------------------------------------------------------------------------------------------
//  EOF.
//----------------------------------------------------------------------------------------------------------------------