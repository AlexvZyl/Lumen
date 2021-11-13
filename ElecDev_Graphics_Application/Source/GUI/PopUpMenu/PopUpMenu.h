#pragma once

/*=======================================================================================================================================*/
/* Includes.																															 */
/*=======================================================================================================================================*/

struct GUIState;
class GraphicsHandler;

/*=======================================================================================================================================*/
/* PopUp Menu.																															 */
/*=======================================================================================================================================*/

class PopUpMenu 
{
private:

	GUIState* m_guiState;
	GraphicsHandler* m_graphicsHandler;

public:

	// Constructor.
	PopUpMenu(GUIState* guiState, GraphicsHandler* graphicsHandler);
	// Render call.
	void render();

};

/*=======================================================================================================================================*/
/* EOF.																																	 */
/*=======================================================================================================================================*/