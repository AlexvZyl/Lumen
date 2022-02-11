#pragma once

//==============================================================================================================================================//
//  Includes..																																	//
//==============================================================================================================================================//

#include <string>
#include "External/ImGUI/Core/imgui.h"

//==============================================================================================================================================//
//  Forward declerations.																														//
//==============================================================================================================================================//

class Event;
class WindowEvent;
class MouseButtonEvent;
class MouseMoveEvent;
class MouseScrollEvent;
class KeyEvent;
class LayerEvent;
struct ImGuiWindow;

//==============================================================================================================================================//
//  GUI Core Element.																															//
//==============================================================================================================================================//

class GuiElementCore 
{
public:

	// Pass event to element.
	virtual void onEvent(Event& event);

protected:

	friend class Layer;
	friend class BasicGuiLayer;
	friend class EngineLayer;

	// Constructor.
	GuiElementCore(std::string name, int windowFlags);

	// ------------------- //
	//  R E N D E R I N G  //
	// ------------------- //

	// Start the ImGUI widget.
	inline virtual void begin() = 0;
	// Render the ImGUI widget.
	inline virtual void renderBody() = 0;
	// End the ImGUI widget.
	inline virtual void end() = 0;
	// Updates the data related to the gui element.
	virtual void dispatchGuiEvents(ImGuiWindow* window);

	// --------- //
	//  D A T A  //
	// --------- //

	// Should the window close?
	bool m_isOpen = true;
	// Is the window collapsed?
	bool m_isCollapsed = false;
	// Is the window hidden?
	bool m_isHidden = false;
	// Is the GUI element docked?
	bool m_isDocked = false;
	// The window name.
	std::string m_name = "NULL";
	// The ImGUI flags describing the window.
	int m_imguiWindowFlags = 0;

	// The size of the content area.
	ImVec2 m_contentRegionSize = { 0.f, 0.f };
	// The position of the content area.
	ImVec2 m_contentRegionPosition = { 0.f, 0.f };

	// ------------- //
	//  E V E N T S  //
	// ------------- //

	// Mouse events.
	inline virtual void onMouseButtonEvent(MouseButtonEvent& event) {};
	inline virtual void onMouseMoveEvent(MouseMoveEvent& event) {};
	inline virtual void onMouseScrollEvent(MouseScrollEvent& event) {};

	// Key events.
	inline virtual void onKeyEvent(KeyEvent& event) {};

	// Content region events.
	void detectContentRegionResize(ImGuiWindow* window);
	void detectContentRegionMove(ImGuiWindow* window);
	virtual void onContentRegionResizeEvent(WindowEvent& event);
	virtual void onContentRegionMoveEvent(WindowEvent& event);

	// Layer events.
	inline virtual void onFocusEvent(LayerEvent& event) {};
	inline virtual void onDefocusEvent(LayerEvent& event) {};
	inline virtual void onHoverEvent(LayerEvent& event) {};
	inline virtual void onDehoverEvent(LayerEvent& event) {};
};

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//