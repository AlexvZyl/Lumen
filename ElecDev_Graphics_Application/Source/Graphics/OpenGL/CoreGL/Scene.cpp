//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include "Scene.h"
#include "Camera.h"

//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

glm::mat4* Scene::getViewMatrix() 
{ 
	return &m_camera->m_viewMatrix; 
}

void Scene::updateCamera() 
{
	m_camera->updateCamera();
}

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//