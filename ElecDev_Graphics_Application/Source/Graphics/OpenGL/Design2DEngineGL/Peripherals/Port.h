#pragma once

//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include "CoreGL/Entities/Entity.h"
#include <string>
#include <memory>
#include "glm.hpp"
#include "CoreGL/Entities/Circle.h"
#include "CoreGL/Entities/Vertex.h"
#include "CoreGL/Entities/Text.h"
#include "CoreGL/VertexArrayObjectGL.h"

//==============================================================================================================================================//
//  Forward declerations.																														//
//==============================================================================================================================================//

struct Font;

class Polygon2D;
class Component2D;
class Cable;
class Scene;

//==============================================================================================================================================//
//  Data.																																		//
//==============================================================================================================================================//

enum PortType
{
	PORT_IN, PORT_OUT, PORT_INOUT
};

enum class PortPosition
{
	TOP, BOTTOM, LEFT, RIGHT
};

//==============================================================================================================================================//
//  Port class.																																	//
//==============================================================================================================================================//

class Port: public Entity
{
public:
	
	glm::vec2 titleOffset;
	static Font titleFont;
	glm::vec4 titleColour = glm::vec4(0.f, 0.f, 0.f, 1.f);
	float titleSize = 0.02f;
	glm::vec2 centre;
	static unsigned portID;
	glm::vec4 bodyColour;
	glm::vec4 borderColour;
	glm::vec4 indicatorColour = {0.5f, 0.5f, 0.5f, 0.f};
	float portLayer;

	Circle* body;
	Circle* border;
	Circle* attachmentIndicator;
	Text* title;

	std::string m_label;
	glm::vec2 m_offset = { 0, 0 };
	PortPosition m_position;
	PortType m_type;

	std::vector<Cable*> m_cables;


	Port(glm::vec2 pos, PortType type, Component2D* parent, std::string label = "default");
	~Port();
	Port& operator = (const Port &t);
	void moveTo(glm::vec2 destination);
	void move(glm::vec2 translation);
	void setLayer(float layer);
	void setContext(GUIState* guiState);
	void highlight();
	void unhighlight();
	void setOffset(glm::vec2 offset);
	void attachCable(Cable* cable);
	void detachCable(Cable* cable);
	void showAttachIndicator();
	void hideAttachIndicator();
};

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//