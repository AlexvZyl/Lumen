#pragma once

//=======================================================================================================================================//
// Includes.																															 //
//=======================================================================================================================================//

#include "GUI/GuiElementCore/GuiElementCore.h"

//=======================================================================================================================================//
// Forward Declerations.																												 //
//=======================================================================================================================================//

struct GUIState;

//=======================================================================================================================================//
// Component Editor.																													 //
//=======================================================================================================================================//

class ComponentEditor : public GuiElementCore
{
public:

	// Constructor.
	ComponentEditor(std::string name, int windowFlags);
	
	// Rendering functions.
	virtual void begin() override;
	virtual void onRender() override;
	virtual void end() override;

private:
	int typeval2 = 1;
	int typeval3 = 1;
	bool addingPort = false;
	std::string newName = "Untitled";
	int newType = 2;
	int newPos = 0;
};

//=======================================================================================================================================//
// EOF.																																	 //
//=======================================================================================================================================//