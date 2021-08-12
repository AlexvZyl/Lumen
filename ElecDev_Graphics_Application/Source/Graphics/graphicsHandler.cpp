/*
This file will control all of the graphics engines and all of the API's, as well as the unitialization.
This is so that the main loop that will containt both ImGUI calls and pure OpenGL calls can remain clean.
*/

//----------------------------------------------------------------------------------------------------------------------
//  Class include.
//----------------------------------------------------------------------------------------------------------------------

#include "graphicsHandler.h"



//----------------------------------------------------------------------------------------------------------------------
//  Constructors & setup.
//----------------------------------------------------------------------------------------------------------------------

// Default.
GraphicsHandler::GraphicsHandler() {};

// With GLFW window.
GraphicsHandler::GraphicsHandler(GLFWwindow* window, stateMachineGraphics* states)
{
	this->states = *states;

	// Create engines.
	DrawingEngineGL drawingEngine(window);
	this->drawingEngine = drawingEngine;
	//DesignEngineGL designEngine(this->window);
	//this->designEngine = designEngine;

	// Set the default active engine.  (Should be set to animation when one is available.)
	activeEngine = "DrawingEngine";

	// Store pointer to GLFW window.
	this->window = window;
};

//----------------------------------------------------------------------------------------------------------------------
//  Functions.
//----------------------------------------------------------------------------------------------------------------------

// [LOOP FUNCTION] Function that handles which engine should be active and is placed into the OpenGL loop.
void GraphicsHandler::renderGraphics()
{

	//  Run engine that has been set as active.
	if (activeEngine == "DrawingEngine")
	{
		drawingEngine.renderLoop();
	}
	else if (activeEngine == "DesignEngine")
	{
		// Run designEngine.
	}
	else if (activeEngine == "Animation")
	{
		// Run some sort of cool place holder animation.
	}
	else
	{
		std::cout << "[ENGINES ERROR] Please supply a valid engine name.\n";
	};

};

// Function that closes the engine passed.
void GraphicsHandler::setEngine(std::string engine)
{
	// Close the current active engine.
	this->closeEngine();

	// Set the new active engine.
	if (activeEngine == "DrawingEngine")
	{
		// Init.
	}
	else if (activeEngine == "DesignEngine")
	{
		// Init.
	}
	else if (activeEngine == "Animatin")
	{
		// Init.
	}
	else
	{
		std::cout << "[ENGINES ERROR] Please supply a valid engine name.\n";
	};
};

// Function that closes the engine passed.
void GraphicsHandler::closeEngine()
{
	// Close the active engine.
	if (this->activeEngine == "DrawingEngine")
	{
		// Close.
	}
	else if (this->activeEngine == "DesignEngine")
	{
		// Close.
	}
	else if (this->activeEngine == "Animation")
	{
		// Close animation.
	}
	else
	{
		std::cout << "[ENGINES ERROR] Error occured closing the active engine.\n";
	};
};

//----------------------------------------------------------------------------------------------------------------------
//  Mouse event handler.
//----------------------------------------------------------------------------------------------------------------------

// Handle mouse press events.
void GraphicsHandler::mousePressEvent(GLFWwindow* window, int button, int action, int mods)
{
	if (!ImGui::GetIO().WantCaptureMouse)
	{
		// Find cursos position.
		double mousePos[2];
		glfwGetCursorPos(window, &mousePos[0], &mousePos[1]);

		// Check if left press.
		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
		{
			// Call active engine.
			drawingEngine.mousePressLeft(mousePos);
		}
	}
		
	

	
}

// Handle mouse move events.
void GraphicsHandler::mouseMoveEvent(GLFWwindow* window, double xpos, double ypos)
{
	if (!ImGui::GetIO().WantCaptureMouse)
	{

		// Find cursos position.
		double mousePos[2] = { xpos, ypos };
		// Check button state.
		int buttonState = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);

		// Call active engine.
		drawingEngine.mouseMoveEvent(mousePos, buttonState);
	}
}

//----------------------------------------------------------------------------------------------------------------------
//  EOF.
//----------------------------------------------------------------------------------------------------------------------