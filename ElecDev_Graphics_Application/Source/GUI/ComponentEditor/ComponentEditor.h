#pragma once

//=======================================================================================================================================//
// Includes.																															 //
//=======================================================================================================================================//

#include "GUI/GuiElementCore/GuiElementCore.h"

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
	int fromSelector = 0;
	int databaseSelector = 0;
	int sizeSelector = 0;
	int ifRowSelector = 0;
	int comparatorSelector = 0;
	int ifSelector = 0;
	int ifSelector2 = 0;
	int equipmentSelector = -1;
	int combineSelector = 0;
	int combineSelectorVariable = 0;
	const char* comparatorSelection[6] = { "==", "!=", "<=", ">=", "<", ">" };
	std::string comparisonValue = "0";

	std::string trueStatement = "True";

	std::string falseStatement = "False";

	std::string combineTextString = "";
	bool addingPort = false;
	std::string newName = "Untitled";
	int newType = 2;
	int newPos = 0;

	inline static unsigned s_cableIcon = 0;
	inline static bool s_iconCreated = false;
	inline static float s_textHeight = 0;
};

//=======================================================================================================================================//
// EOF.																																	 //
//=======================================================================================================================================//