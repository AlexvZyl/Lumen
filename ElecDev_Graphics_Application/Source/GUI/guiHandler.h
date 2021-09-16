#pragma once

//----------------------------------------------------------------------------------------------------------------------
//  Includes.
//----------------------------------------------------------------------------------------------------------------------

#include <../Graphics/graphicsHandler.h>

//  General.
#include <string>
#include <iostream>

#include "Ribbons/ribbons.h"
#include "Toolbar/toolbar.h"
#include "Graphics/graphics.h"
#include "../stateMachine.h"

// ImGUI (GUI software). 
#include "Core/imgui.h"
#include "Implementations/imgui_impl_glfw.h"
#include "Implementations/imgui_impl_opengl3.h"

#define IMGUI_DEFINE_MATH_OPERATORS
#include <Core/imgui_internal.h>

//----------------------------------------------------------------------------------------------------------------------
//  GUI Handler Class.
//----------------------------------------------------------------------------------------------------------------------

class GUIHandler
{
public:

	//--------------------------------------------------------------------------------------------------------------
	//  Variables.
	//--------------------------------------------------------------------------------------------------------------

	stateMachine* states;

	GraphicsHandler* graphicsHandler;

	unsigned int textureID;

	Toolbar* toolbar;
	Ribbons* ribbons;
	Graphics* graphics;

	//--------------------------------------------------------------------------------------------------------------
	//  Functions.
	//--------------------------------------------------------------------------------------------------------------

	// Constructor.
	GUIHandler(stateMachine* states, GraphicsHandler* graphicsHandler);

	// Function that handles which engine should be active.
	void renderGui(ImGuiIO& io);

	// Set custom theme.
	void setTheme();

};

//----------------------------------------------------------------------------------------------------------------------
//  EOF.
//----------------------------------------------------------------------------------------------------------------------