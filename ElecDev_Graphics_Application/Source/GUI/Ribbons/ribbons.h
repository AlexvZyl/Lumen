#pragma once

/*=======================================================================================================================================*/
/* Includes                                                                                                                              */
/*=======================================================================================================================================*/

// To use resources.
#include <Windows.h>
#include <glad/glad.h>
#include <Core/imgui.h>
#include <string>
#include "../GUIState.h"

// Resource loading.
#include "../Resources/ResourceHandler.h"

/*=======================================================================================================================================*/
/* Forward declerations                                                                                                                  */
/*=======================================================================================================================================*/

class GraphicsHandler;

/*=======================================================================================================================================*/
/* Class.	                                                                                                                             */
/*=======================================================================================================================================*/

class Ribbons
{
public:

	//--------------------------------------------------------------------------------------------------------------
	//  Variables.
	//--------------------------------------------------------------------------------------------------------------

	bool show_app_main_menu_bar;
	bool show_app_documents;
	bool show_app_console;
	bool show_app_log;
	bool show_app_layout;
	bool show_app_property_editor;
	bool show_app_long_text;
	bool show_app_auto_resize;
	bool show_app_constrained_resize;
	bool show_app_simple_overlay;
	bool show_app_fullscreen;
	bool show_app_window_titles;
	bool show_app_custom_rendering;
	bool show_app_metrics;
	bool show_app_style_editor;
	bool show_app_about;
	bool no_titlebar;
	bool no_scrollbar;
	bool no_menu;
	bool no_move;
	bool no_resize;
	bool no_collapse;
	bool no_close;
	bool no_nav;
	bool no_background;
	bool no_bring_to_front;
	bool unsaved_document;
	bool my_tool_active;
	bool drawToggle;
    int image1_width;
    int image1_height;
    GLuint image1_texture;
    bool ret1;
    int image2_width;
    int image2_height;
    GLuint image2_texture;
    bool ret2;
    int image3_width;
    int image3_height;
    GLuint image3_texture;
	int image4_width;
	int image4_height;
	GLuint image4_texture;
    bool ret3;
	std::string sideBarFlag;
	GUIState* m_guiState;
	ImVec2 MCCPos;
	ImGuiID* dock;
	ImGuiID left;
	bool first[3];

	GraphicsHandler* m_graphicsHandler;

	//--------------------------------------------------------------------------------------------------------------
	//  Functions.
	//--------------------------------------------------------------------------------------------------------------

	// Constructor.
	Ribbons(GUIState* guiState, GraphicsHandler* graphicsHandler);

	// Function that handles which engine should be active.
	void renderRibbons(ImGuiID* dock);
	bool topBar();
	bool sideBar();
	bool MCC();
	bool blockDiagram();
	bool BeginDrawButtonDropDown(const char* label, ImVec2 buttonSize);
};

/*=======================================================================================================================================*/
/* EOF.		                                                                                                                             */
/*=======================================================================================================================================*/