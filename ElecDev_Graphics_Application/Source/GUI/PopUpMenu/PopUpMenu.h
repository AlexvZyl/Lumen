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

public:

	// Constructor.
	PopUpMenu(GUIState* guiState);
	// Render call.
	void render();
	void close();

};

/*=======================================================================================================================================*/
/* EOF.																																	 */
/*=======================================================================================================================================*/