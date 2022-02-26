#pragma once

//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include "GUI/GuiElementCore/GuiElementCore.h"
#include "glm/glm.hpp"

//==============================================================================================================================================//
//  Popup menu.																																	//
//==============================================================================================================================================//

class ColorEditor : public GuiElementCore
{
public:

	// Constructor.
	ColorEditor(std::string name, int imguiWindowFlags);
	// Destructor.
	inline virtual ~ColorEditor() = default;

	// Rendering.
	virtual void begin() override;
	virtual void onRender() override;
	virtual void end() override;

	glm::vec2 m_initialPosition;

};

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//