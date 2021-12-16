#pragma once

//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include "External/GLM/glm.hpp"

//==============================================================================================================================================//
//  Forward Declerations.																														//
//==============================================================================================================================================//

class Event;

//==============================================================================================================================================//
//  Camera Types.																																//
//==============================================================================================================================================//

enum class CameraType
{
	Standard2D, Standard3D
};

//==============================================================================================================================================//
//  Camera Class.																																//
//==============================================================================================================================================//

class Camera 
{
public:

	// Constructor.
	Camera(CameraType cameraType);

	// Updates the camera view matrix.
	void updateCamera();

	// Handles the passed event.
	void onEvent(Event& event);

private:

	friend class Renderer;
	friend class Scene;

	CameraType m_type;

	// ----------------- //
	//  C O N T R O L S  //
	// ----------------- //



	// ----------------- //
	//  M A T R I C E S  //
	// ----------------- //

	glm::mat4 m_viewMatrix;
	glm::mat4 m_scalingMatrix;
	glm::mat4 m_rotationMatrix;
	glm::mat4 m_translationMatrix;
};

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//