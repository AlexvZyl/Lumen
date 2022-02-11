#pragma once

//=============================================================================================================================================//
//  Includes.																																   //
//=============================================================================================================================================//

#include <string>
#include "glm.hpp"
#include <memory>
#include <vector>
#include "Port.h"
#include "OpenGL/Entities/Entity.h"

//=============================================================================================================================================//
//  Forward declerations.																													   //
//=============================================================================================================================================//

template<typename VertexType>
class VertexArrayObject;

class Circle;
class Text;
class Polygon2D;
struct Font;
class VertexData;
class VertexDataTextured;
class VertexDataCircle;
class Circuit;
class Scene;

//=============================================================================================================================================//
//  Class.																																	   //
//=============================================================================================================================================//

class Component2D: public Entity
{
public:

	// Shape and edge data.
	Polygon2D* shape;
	Polygon2D* border;
	Text* title;

	static unsigned componentID;

	// Component shape attributes.
	float height = 0.1f;
	float width = 0.1f;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> vertices2;

	// Colour attributes.
	glm::vec4 shapeColour = { 0.5f, 0.5f, 0.9f, 0.5f };
	glm::vec4 borderColour = { 0.f, 0.f, 0.f, 1.f };

	//title
	static Font titleFont;
	glm::vec2 titleOffset = glm::vec2(0.f, -0.15f);
	glm::vec4 titleColour = glm::vec4(0.f, 0.f, 0.f, 1.f);
	std::string titleString;
	float titleSize = 0.035f;

	//port specifications
	unsigned n_ports_north = 0;
	unsigned n_ports_south = 0;
	unsigned n_ports_east = 0;
	unsigned n_ports_west = 0;
	glm::vec2 portOffset;

	// Interaction attributes.
	bool selected = true;

	std::vector<std::shared_ptr<Port>> portsNorth;
	std::vector<std::shared_ptr<Port>> portsSouth;
	std::vector<std::shared_ptr<Port>> portsEast;
	std::vector<std::shared_ptr<Port>> portsWest;

	unsigned numPorts = 0;

	float componentLayer = 0.9f;
	float borderLayerOffset = 0.01f;
	float portLayerOffset = 0.02f;
	glm::vec2 centre;
	
	// Creates a generic component centred at (0, 0).
	Component2D(Circuit* parent);
	// Creates a generic component centred at the specified coordinates.
	Component2D(glm::vec2 centreCoords, Circuit* parent);

	// Deconstructor.s
	~Component2D();
	//Move the component to a new positioned centred at the given coordinates
	void moveTo(glm::vec2 pointerPos);
	//Move the component by the  given vector
	void move(glm::vec2 translation);
	//Place the component.
	void place(glm::vec2 pos);
	//Move the component to a new layer.
	void setLayer(float layer);
	//set the clickedZone.component flag in the GUIState.
	void setContext(GUIState* guiState);
	//Highlight the component.
	void highlight();
	//Remove the component highlighting.
	void unhighlight();
	//Add a port with the given definition to the component.
	unsigned addPort(int side, PortType type, std::string name);
	//remove a specified port from the component.
	void removePort(std::shared_ptr<Port> port);
	//Update the positions(offsets) of each port. Note: Currently, all ports are updated by this regardless if they have been changed or not. This may lead to inefficiencies, and should be changed in the future.
	void updatePortPositions();
	//void destroy();
};