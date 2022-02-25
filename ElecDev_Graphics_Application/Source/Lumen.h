#pragma once

//==============================================================================================================================================//
//  Defines.																																	//
//==============================================================================================================================================//

#define TOOLBAR_PADDING 6
#define RIBBON_HEIGHT 40
#define VERTEX_BUFFER_INCREMENTS 100
#define MAX_VERTEX_BUFFER_SIZE 10000

//==============================================================================================================================================//
//  Forward declerations.																														//
//==============================================================================================================================================//

class Application;

//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

class Lumen
{

public:

	// Set the singleton Lumen uses.
	static void setApp(Application* app);
	// Get the singleton Lumen is using.
	static Application& getApp();

private:

	// No instantiations.
	Lumen() = default;

	// The singleton application.
	static Application* s_applicationSingleton;

};

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//