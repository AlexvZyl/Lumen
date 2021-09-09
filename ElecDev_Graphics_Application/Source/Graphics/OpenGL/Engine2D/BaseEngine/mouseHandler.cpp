/*
This is where the drawing enigine mouse events are handled.
*/

//----------------------------------------------------------------------------------------------------------------------
//  Includes.
//----------------------------------------------------------------------------------------------------------------------

#include "Engine2D/BaseEngine/baseEngineCore.h"

//----------------------------------------------------------------------------------------------------------------------
//  Mouse Press event.
//----------------------------------------------------------------------------------------------------------------------

// Event handler for a mouse left click.
void BaseEngineGL::mousePressLeft(double pixelCoords[2])
{
	// Find current click in world coords.
	glm::vec4 currmousePosVec = pixelCoordsToWorldCoords(pixelCoords);
	// Save current mouse pos click.
	m_prevMouseEventWorldCoords[0] = currmousePosVec[0];
	m_prevMouseEventWorldCoords[1] = currmousePosVec[1];
}

// Event handler for a mouse right click.
void BaseEngineGL::mousePressRight(double pixelCoords[2])
{
	// Reset the view values.
	m_scalingMatrix = glm::mat4(1.0f);
	m_translationMatrix = glm::mat4(1.0f);
	m_rotationMatrix = glm::mat4(1.0f);
}

//----------------------------------------------------------------------------------------------------------------------
//  Mouse Move event.
//----------------------------------------------------------------------------------------------------------------------

// Event handler for a mouse move event.
void BaseEngineGL::mouseMoveEvent(double pixelCoords[2], int buttonState)
{
	// Check if left mouse is pressed.
	if (buttonState == GLFW_PRESS)
	{
		// Find current mouse position in the world.
		glm::vec4 currMousePosVec = pixelCoordsToWorldCoords(pixelCoords);
		// Calculate distance to translate.
		glm::vec3 translateVec({ (currMousePosVec[0] - m_prevMouseEventWorldCoords[0]),(currMousePosVec[1] - m_prevMouseEventWorldCoords[1]),0 });
		// Translate to the view matrix.
		m_translationMatrix = glm::translate(m_translationMatrix, translateVec);

		// Save mouse click position.
		m_prevMouseEventWorldCoords[0] = currMousePosVec[0];
		m_prevMouseEventWorldCoords[1] = currMousePosVec[1];
	}
}

//----------------------------------------------------------------------------------------------------------------------
//  Mouse Scroll event.
//----------------------------------------------------------------------------------------------------------------------

void BaseEngineGL::mouseScrollEvent(double pixelCoords[2], int yOffset) 
{	
	
	// Calculate zoom value based on mouse wheel scroll.
	//----------------------------------------------------------
	
	// Define the scaling value.
	double zoomScaleValue;
	 
	// Determine if it should be a zoom in or out.
	if (yOffset > 0) // Zoom in.
	{	
		// Zoom scale value.
		zoomScaleValue = 1 + (double)m_scaleRate;
	}
	else // Zoom out.
	{
		zoomScaleValue = 1 / (1 + (double)m_scaleRate);
	}

	//  Apply scale and translate to keep zoom around mouse.
	//----------------------------------------------------------
	
	// Calculate mouse world coords before scaling.
	glm::vec4 mouseWorldCoordsPre = pixelCoordsToWorldCoords(pixelCoords);
	// Apply scaling.
	glm::vec3 scaleVector = glm::vec3(zoomScaleValue, zoomScaleValue, 1);
	m_scalingMatrix = glm::scale(m_scalingMatrix, scaleVector);
	// Calculate mouse world coordinates after scaling.
	glm::vec4 mouseWorldCoordsPost = pixelCoordsToWorldCoords(pixelCoords);
	// Translate the world so that the zoom happens on the mouse position. 
	glm::vec3 translateVector = glm::vec3((double)mouseWorldCoordsPost[0] - (double)mouseWorldCoordsPre[0], (double)mouseWorldCoordsPost[1] - (double)mouseWorldCoordsPre[1], 0);
	//  Apply translation vector.
	m_translationMatrix = glm::translate(m_translationMatrix, translateVector);

	// Update mouse coordinates.
	m_prevMouseEventWorldCoords[0] = mouseWorldCoordsPost[0];
	m_prevMouseEventWorldCoords[1] = mouseWorldCoordsPost[1];
}

//----------------------------------------------------------------------------------------------------------------------
//  EOF.
//----------------------------------------------------------------------------------------------------------------------