//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include "Camera.h"

//==============================================================================================================================================//
//  Functions.																																	//
//==============================================================================================================================================//

Camera::Camera(CameraType cameraType) 
	: m_type(cameraType)
{}

void Camera::updateCamera() 
{
	m_viewMatrix = m_scalingMatrix * m_rotationMatrix * m_translationMatrix; 
}

//==============================================================================================================================================//
//  Controls.																																	//
//==============================================================================================================================================//



//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//
