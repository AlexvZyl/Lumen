#pragma once

//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include "GUI/GuiElementCore/GuiElementCore.h"

//==============================================================================================================================================//
//  Graphics Scene.																																//
//==============================================================================================================================================//

class GraphicsScene : public GuiElementCore
{
public:

	// Constructor.
	GraphicsScene(std::string name, int windowFlags);

	// Rendering functions.
	virtual void renderBody() override;

	// Set the texture ID that is to be rendered.
	void setTextureID(unsigned textureID);

private:

	// Events.
	virtual void onWindowResizeEvent(WindowResizeEvent& event) override;

	// Pointer to the graphics texture.
	void* m_textureID = nullptr;
	ImVec2 m_textureSize;
};

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//
