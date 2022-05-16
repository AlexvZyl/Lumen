#pragma once

//=============================================================================================================================================//
//  Includes.																																   //
//=============================================================================================================================================//

#include <string>
#include "glm/glm.hpp"
#include <memory>
#include <vector>
#include "Port.h"
#include <unordered_map>
#include "Graphics/Entities/Entity.h"
#include "External/YAML-CPP/Includes/yaml-cpp/yaml.h"
#include <filesystem>

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
class LineSegment;
class PolyLine;

//=============================================================================================================================================//
//  Class.																																	   //
//=============================================================================================================================================//

class Component2D : public Entity
{
public:

	// Creates a generic component centred at (0, 0).
	Component2D(Circuit* parent);
	// Creates a generic component centred at the specified coordinates.
	Component2D(const glm::vec2& centreCoords, Circuit* parent);
	// Creates a component from a .lmcp file definition
	Component2D(const std::filesystem::path& path, Circuit* parent = nullptr);
	// Create a component from a YAML node.
	Component2D(const YAML::Node& path, Circuit* parent = nullptr);

	// Entites & Primitives describing the component.
	std::vector<Polygon2D*> m_polygons;
	std::vector<PolyLine*> m_lines;
	std::vector<Circle*> m_circles;
	std::vector<Text*> m_text;
	Text* title;
	Text* designator;
	std::vector<std::shared_ptr<Port>> ports;

	static unsigned componentID;

	// Specify the type of the equipment
	std::string equipType = "Block";

	// Data for ElecDec software.
	std::unordered_map<std::string, std::string> dataDict;
	std::string m_internalCircuit;

	// Component shape attributes.
	float height = 0.08f;
	float width = 0.08f;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> vertices2;

	// Colour attributes.
	glm::vec4 shapeColour = { 14.f/255.f, 63.f/255.f, 174.f/255.f, 1.f };
	glm::vec4 borderColour = { 0.f, 0.f, 0.f, 1.f };
	bool m_highlighted = false;

	//title
	static Font titleFont;
	glm::vec2 titleOffset = glm::vec2(0.01f, 0.0f);
	glm::vec4 titleColour = glm::vec4(0.f, 0.f, 0.f, 1.f);
	std::string titleString;
	float titleSize = 0.0018f;

	//port specifications
	unsigned n_ports_north = 0;
	unsigned n_ports_south = 0;
	unsigned n_ports_east = 0;
	unsigned n_ports_west = 0;
	glm::vec2 portOffset;

	// Interaction attributes.
	bool selected = true;

	unsigned numPorts = 0;
	float componentLayer = 0.9f;
	float borderLayerOffset = 0.01f;
	float portLayerOffset = 0.02f;
	glm::vec2 centre = {0.f, 0.f};

	// Deconstructor.s
	~Component2D();
	//Move the component to a new positioned centred at the given coordinates
	void moveTo(const glm::vec2& pointerPos);
	//Move the component by the  given vector
	void move(const glm::vec2& translation);
	//Place the component.
	void place(const glm::vec2& pos);
	//Move the component to a new layer.
	void setLayer(float layer);
	//set the clickedZone.component flag in the GUIState.
	void setContext(GUIState* guiState);
	//Highlight the component.
	void enableOutline();
	//Remove the component highlighting.
	void disableOutline();
	//Add a port with the given definition to the component.
	//unsigned addPort(int side, PortType type, const std::string& name);
	//remove a specified port from the component.
	void removePort(std::shared_ptr<Port> port);
	//Update the positions(offsets) of each port. Note: Currently, all ports are updated by this regardless if they have been changed or not. This may lead to inefficiencies, and should be changed in the future.
	//void updatePortPositions();
	//move the title text relative to the component
	void translateTitle(glm::vec2 translation);
	//updates the Component label
	void updateText();
	//updates the Component type, without a label (for comp. designer)
	void updateTextWithoutLabel();
	//Set the colour of the polygons and circles belonging to the component
	void setColour(const glm::vec4& colour);
	//Adds a new polygon to the polygon list
	void addPoly(Polygon2D* poly);
	void addCircle(Circle* circle);
	void addLine(PolyLine* line);
	void addPort(std::shared_ptr<Port> port);

	void removePoly(Polygon2D* poly);
	void removeCircle(Circle* circle);
	void removeLine(PolyLine* line);
	void removeText(Text* text);
	
	void rotate(float degrees);
	float m_rotation = 0.f;

private:
	PortType getPortType(YAML::Node node);
};
