//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include "ImGUI/Implementations/imgui_impl_glfw.h"
#include "GLFW/glfw3.h"
#include "Events.h"

//==============================================================================================================================================//
//  Event overloaded functions.																													//
//==============================================================================================================================================//

bool operator==(uint64_t id, EventType eventType) 
{ 
	return id & eventType; 
}

bool operator!=(uint64_t id, EventType eventType)
{
	return not (id & eventType);
}

//==============================================================================================================================================//
//  Event Class.																																//
//==============================================================================================================================================//

void Event::consume()
{ 
	consumed = true; 
}

bool Event::isConsumed() 
{ 
	return consumed; 
}

Event::Event(uint64_t ID) 
	: ID(ID)
{}

//==============================================================================================================================================//
//  Mouse events.																																//
//==============================================================================================================================================//

// --------------------- //
//  M O U S E   B A S E  //
// --------------------- //

MouseEvent::MouseEvent(glm::vec2& positionPixels, uint64_t ID)
	: Event(ID), mousePosition(positionPixels)
{}

// ------------------------- //
//  M O U S E   B U T T O N  //
// ------------------------- //

MouseButtonEvent::MouseButtonEvent(glm::vec2& mousePositionPixels, uint64_t ID)
	: MouseEvent(mousePositionPixels, ID)
{}



// --------------------- //
//  M O U S E   M O V E  //
// --------------------- //

MouseMoveEvent::MouseMoveEvent(glm::vec2& mousePositionPixels, uint64_t ID)
	: MouseEvent(mousePositionPixels, ID | EventType_MouseMove)
{}

// ------------------------- //
//  M O U S E   S C R O L L  //
// ------------------------- //

MouseScrollEvent::MouseScrollEvent(glm::vec2 mousePositionPixels, float yOffset, uint64_t ID)
	: MouseEvent(mousePositionPixels, ID | EventType_MouseScroll), yOffset(yOffset) 
{}

//==============================================================================================================================================//
//  Key Events.																																    //
//==============================================================================================================================================//

KeyEvent::KeyEvent(int key, uint64_t ID, glm::vec2& mousePos)
	: Event(ID), key(key), mousePosition(mousePos)
{}

//==============================================================================================================================================//
//  Window events.																																//
//==============================================================================================================================================//

WindowEvent::WindowEvent(glm::vec2& windowResize, uint64_t ID, bool isScale)
	: Event(ID), windowData(windowResize), isScale(isScale)
{}

//==============================================================================================================================================//
//  Serialisation events.																														//
//==============================================================================================================================================//

// ------------------- //
//  F I L E   D R O P  //
// ------------------- //

FileDropEvent::FileDropEvent(std::vector<std::string>& paths)
	: Event(EventType_FileDrop | EventType_Application)
{
	filePaths = std::make_unique<std::vector<std::string>>(std::move(paths));
}

//==============================================================================================================================================//
//  Layer Events.																														//
//==============================================================================================================================================//

LayerEvent::LayerEvent(uint64_t ID)
	: Event(ID)
{}

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//