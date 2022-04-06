//============================================================================================================================================//
// Includes.																																   //
//=============================================================================================================================================//

#include "Component2D.h"
#include "OpenGL/Primitives/Vertex.h"
#include "OpenGL/Primitives/Polygon.h"
#include "OpenGL/Buffers/VertexArrayObjectGL.h"
#include "Graphics/Fonts/FontLoader.h"
#include "OpenGL/Primitives/Text.h"
#include "Resources/ResourceHandler.h"
#include "OpenGL/Primitives/Circle.h"
#include <iostream>
#include "External/Misc/ConsoleColor.h"
#include "Circuit.h"
#include "OpenGL/SceneGL.h"
#include "OpenGL/Renderer/RendererGL.h"
#include "Utilities/Logger/Logger.h"

//=============================================================================================================================================//
//  Variables.																																   //
//=============================================================================================================================================//

//Add font for component titles
Font Component2D::titleFont = msdfLoadFont(ARIAL_NORMAL_JSON);
//Font Component2D::titleFont = msdfLoadFont(ARIAL_BOLD_MSDF_JSON);
unsigned Component2D::componentID = 0;

//=============================================================================================================================================//
//  Constructor & Destructor.																												   //
//=============================================================================================================================================//

Component2D::Component2D(Circuit* parent)
	: Entity(EntityType::COMPONENT, parent)
{
	// ----------- //
	//  S E T U P  //
	// ----------- //

	// Create vertices.
	centre = glm::vec2(0.0f, 0.0f);
	vertices.reserve(5);
	vertices.emplace_back(glm::vec3(centre.x - width, centre.y - height, 0.0f));
	vertices.emplace_back(glm::vec3(centre.x + width, centre.y - height, 0.0f));
	vertices.emplace_back(glm::vec3(centre.x + width, centre.y + height, 0.0f));
	//vertices.emplace_back(glm::vec3(centre.x - width, centre.y + height, 0.0f));
	//vertices.emplace_back(glm::vec3(centre.x - width/2, centre.y + height/2, 0.0f));

	// --------------------- //
	//  P R I M I T I V E S  //
	// --------------------- //

	// Main shape.
	//shape = Renderer::addPolygon2D(vertices, this);
	//shape->setColor({shapeColour.r, shapeColour.g, shapeColour.b, 0.5f});
	//shape->setLayer(0.001f);//temp fix
	// Component border.
	//border = Renderer::addPolygon2DClear(vertices, this);
	//border->setColor(borderColour);
	//border->setLayer(componentLayer + borderLayerOffset);
	// Component title.
	glm::vec3 titlePos = glm::vec3(centre + titleOffset, componentLayer + borderLayerOffset);
	titleString = "Component " + std::to_string(componentID++);
	std::string textString = equipType + std::string(": ") + titleString;
	title = Renderer::addText2D(textString, titlePos, titleColour, titleSize, "C", "B", this);
	// Add some test ports. (TO BE REMOVED). PLease keep this here while we are testing (at least until we have some generic components that can be added). 
	// It is a bit of a pain setting up ports every time we test.
	//addPort(0, PortType::PORT_IN, "LX1");
	//addPort(1, PortType::PORT_OUT, "RX1");
	highlight();

	// Dictionary for GUI of component for data automation.ToTagNumber	DBRef	Comments	Metric	Type	Unit
	dataDict.insert(std::pair<std::string, std::string>("ToTagNumber", "From(Circuit Database)"));
	dataDict.insert(std::pair<std::string, std::string>("Metric", "1"));
	dataDict.insert(std::pair<std::string, std::string>("Description", "From(Circuit Database)"));
	dataDict.insert(std::pair<std::string, std::string>("Unit", "ea"));
	dataDict.insert(std::pair<std::string, std::string>("DBRef", "From(Circuit Database)"));
}

Component2D::Component2D(const glm::vec2& centreCoords, Circuit* parent)
	: Component2D(parent)
{
	moveTo(centreCoords);
}

Component2D::Component2D(YAML::Node& lmcpFile, Circuit* parent) : Entity(EntityType::COMPONENT, parent)
{

	//check that the node is from a .lmcp file
	if (lmcpFile["Lumen File Info"]["Type"].as<std::string>() != "Component") {
		LUMEN_LOG_ERROR("Components can only be constructed from a .lmcp file.", "Component2D");
	}
	YAML::Node componentNode = lmcpFile["Component"];
	shapeColour = glm::vec4(componentNode["Shape"]["Colour"][0].as<float>(),
							componentNode["Shape"]["Colour"][1].as<float>(),
							componentNode["Shape"]["Colour"][2].as<float>(),
							componentNode["Shape"]["Colour"][3].as<float>());


	// ----------- //
	//  S E T U P  //
	// ----------- //

	// Create vertices.
	centre = glm::vec2(0.0f, 0.0f);
	//vertices.reserve(4);
	//vertices.emplace_back(glm::vec3(centre.x - width, centre.y - height, 0.0f));
	//vertices.emplace_back(glm::vec3(centre.x + width, centre.y - height, 0.0f));
	//vertices.emplace_back(glm::vec3(centre.x + width, centre.y + height, 0.0f));
	//vertices.emplace_back(glm::vec3(centre.x - width, centre.y + height, 0.0f));

	// --------------------- //
	//  P R I M I T I V E S  //
	// --------------------- //

	// Main shape.
	//shape = Renderer::addPolygon2D(vertices, this);
	//shape->setColor({ shapeColour.r, shapeColour.g, shapeColour.b, 0.5f });
	//shape->setLayer(0.001f);//temp fix
	// Component border.
	//border = Renderer::addPolygon2DClear(vertices, this);
	//border->setColor(borderColour);
	//border->setLayer(componentLayer + borderLayerOffset);
	// Component title.
	glm::vec3 titlePos = glm::vec3(centre + titleOffset, componentLayer + borderLayerOffset);
	titleString = componentNode["Title"]["String"].as<std::string>();
	equipType = componentNode["Title"]["EquipType"].as<std::string>();
	std::string textString = equipType + std::string(": ") + titleString;
	title = Renderer::addText2D(textString, titlePos, titleColour, titleSize, "C", "B", this);
	title = Renderer::addText2D(titleString, titlePos, titleColour, titleSize, "C", "B", this);

	//Load dictionary.
	YAML::Node componentDict = componentNode["Dictionary"];
	for (YAML::iterator it = componentDict.begin(); it != componentDict.end(); ++it)
	{
		dataDict.insert({ it->first.as<std::string>(), it->second.as<std::string>() });
	}

	// ----------- //
	//  P O R T S  //
	// ----------- //

	// Load the ports.
	YAML::Node portList = componentNode["Ports"];

	// East ports.
	YAML::Node eastPortList = portList["East Ports"];
	for (YAML::iterator portIterator = eastPortList.begin(); portIterator != eastPortList.end(); ++portIterator)
	{
		YAML::Node portNode = portIterator->second;
		unsigned entityID = addPort(1, getPortType(portNode), portNode["Label"].as<std::string>());
		// Add entity ID to table.
		//idTable.insert({ portNode["Entity ID"].as<unsigned>(), entityID });
	}
	// West ports.
	YAML::Node westPortList = portList["West Ports"];
	for (YAML::iterator portIterator = westPortList.begin(); portIterator != westPortList.end(); ++portIterator)
	{
		YAML::Node portNode = portIterator->second;
		unsigned entityID = addPort(0, getPortType(portNode), portNode["Label"].as<std::string>());
		// Add entity ID to table.
		//idTable.insert({ portNode["Entity ID"].as<unsigned>(), entityID });
	}
	// West ports.
	YAML::Node northPortList = portList["North Ports"];
	for (YAML::iterator portIterator = northPortList.begin(); portIterator != northPortList.end(); ++portIterator)
	{
		YAML::Node portNode = portIterator->second;
		unsigned entityID = addPort(2, getPortType(portNode), portNode["Label"].as<std::string>());
		// Add entity ID to table.
		//idTable.insert({ portNode["Entity ID"].as<unsigned>(), entityID });
	}
	// South ports.
	YAML::Node southPortList = portList["South Ports"];
	for (YAML::iterator portIterator = southPortList.begin(); portIterator != southPortList.end(); ++portIterator)
	{
		YAML::Node portNode = portIterator->second;
		unsigned entityID = addPort(3, getPortType(portNode), portNode["Label"].as<std::string>());
		// Add entity ID to table.
		//idTable.insert({ portNode["Entity ID"].as<unsigned>(), entityID });

	}
}

Component2D::~Component2D() 
{
	//Renderer::remove(shape);
	//Renderer::remove(border);
	Renderer::remove(title);
	for (auto circle : m_circles) {
		Renderer::remove(circle);
	}
	for (auto line : m_lines) {
		Renderer::remove(line);
	}
	for (auto poly : m_polygons) {
		Renderer::remove(poly);
	}
}

//=============================================================================================================================================//
//  Constructor & Destructor.																												   //
//=============================================================================================================================================//

void Component2D::moveTo(const glm::vec2& pointerPos)
{
	//shape->translateTo(pointerPos);
	//border->translateTo(pointerPos);
	glm::vec2 titleDest = pointerPos + titleOffset;
	title->translateTo(titleDest);

	for (auto poly : m_polygons) {
		poly->translateTo(pointerPos);
	}
	for (auto line : m_lines) {
		line->translateTo(pointerPos);
	}
	for (auto circ : m_circles) {
		circ->translateTo(pointerPos);
	}

	for (std::shared_ptr port : ports) {
		port->move(pointerPos - port->centre);
	}
	
	centre = glm::vec2(pointerPos[0], pointerPos[1]);
}

void Component2D::move(const glm::vec2& translation)
{
	//shape->translate(translation);
	//border->translate(translation);
	title->translate(translation);
	for (auto poly : m_polygons) {
		poly->translate(translation);
	}
	for (auto line : m_lines) {
		line->translate(translation);
	}
	for (auto circ : m_circles) {
		circ->translate(translation);
	}

	for (std::shared_ptr port : ports) {
		port->move(translation);
	}
	
	centre += translation;
}

void Component2D::place(const glm::vec2& pos)
{	
	// Ensure the component is at the desired position.
	moveTo(pos);
	setLayer(0.0f);
	//shape->setColor(shapeColour);
	title->setColor(titleColour);
	for (auto poly : m_polygons) {
		poly->setColor(shapeColour);
	}
	for (auto circ : m_circles) {
		circ->setColor(shapeColour);
	}
	// Move to placement layer.
}

void Component2D::setLayer(float layer)
{
	//shape->setLayer(layer);
	//border->setLayer(layer + borderLayerOffset);
	title->setLayer(layer + borderLayerOffset);
	for (auto poly : m_polygons) {
		poly->setLayer(layer);
	}
	for (auto line : m_lines) {
		line->setLayer(layer);
	}
	for (auto circ : m_circles) {
		circ->setLayer(layer);
	}
	for (std::shared_ptr port : ports) {
		port->setLayer(layer + portLayerOffset);
	}
	
	componentLayer = layer;
}

void Component2D::setContext(GUIState* guiState)
{
	//guiState->clickedZone.component = true;
}

void Component2D::highlight()
{
	m_highlighted = true;
	borderColour = { 0.f, 0.f, 1.0f, 1.f };
	//border->setColor(borderColour);

	for (std::shared_ptr port : ports) {
		port->highlight();
	}

	enableOutline();
}

void Component2D::unhighlight()
{
	m_highlighted = false;
	borderColour = { 0.f, 0.f, 0.f, 1.f };
	//border->setColor(borderColour);
	for (std::shared_ptr port : ports) {
		port->unhighlight();
	}

	disableOutline();
}

void Component2D::disableOutline()
{
	//shape->disableOutline();
	//border->disableOutline();
	title->disableOutline();
	for (auto poly : m_polygons) {
		poly->disableOutline();
	}
	for (auto line : m_lines) {
		line->disableOutline();
	}
	for (auto circ : m_circles) {
		circ->disableOutline();
	}
}

unsigned Component2D::addPort(int side, PortType type, const std::string& name)
{
	switch(side)
	{
	case 0:
		portsWest.push_back(std::make_shared<Port>(glm::vec2(-width, 0.f), type, this, name));
		updatePortPositions();
		return portsWest.back()->m_entityID;

	case 1:
		portsEast.push_back(std::make_shared<Port>(glm::vec2(width, 0.f), type, this, name));
		updatePortPositions();
		return portsEast.back()->m_entityID;

	case 2:
		portsNorth.push_back(std::make_shared<Port>(glm::vec2(0.f, height), type, this, name));
		updatePortPositions();
		return portsNorth.back()->m_entityID;

	case 3:
		portsSouth.push_back(std::make_shared<Port>(glm::vec2(0.f, -height), type, this, name));
		updatePortPositions();
		return portsSouth.back()->m_entityID;

	default:
		// Invalid value passed.
		std::cout << yellow << "\n[Design2D] [WARNING]: " << white << "Cannot add a port to side " << side << " (require side < 4).";
		return NULL;
	}
}

void Component2D::removePort(std::shared_ptr<Port> port)
{
	auto port_to_remove = std::find(begin(ports), end(ports), port);

	if (port_to_remove != end(ports)) 
	{
		ports.erase(port_to_remove);
		ports.shrink_to_fit();
		return;
	}
	//Warn the user if the port was not found on this component (i.e. if we have not returned yet).
	std::cout << yellow << "\n[Design2D] [WARNING]: " << white << "Tried to delete port "<<port->m_label<< ", but it does not belong to component "<< titleString <<".";
}

void Component2D::updatePortPositions()
{
	std::vector<std::shared_ptr<Port>>* allPorts[4] = {	&portsWest,
														&portsEast,
														&portsNorth,
														&portsSouth };

	unsigned* n_ports_sides[4] = {&n_ports_west, &n_ports_east, &n_ports_north, &n_ports_south};

	for (int i = 0; i < 4; i++) {
		std::vector<std::shared_ptr<Port>>& portsSide = *allPorts[i];
		unsigned* n_ports = n_ports_sides[i];
		if (portsSide.size() != *n_ports) {
			*n_ports = portsSide.size();

			//Placement is dependent on which side we are on.
			glm::vec2 offset_side;
			switch (i) 
			{
			case 0:
				//Set west offsets
				offset_side = glm::vec2(-width, -height);
				for (int j = 0; j < *n_ports; j++) 
				{
					glm::vec2 offset_j = glm::vec2(0.f, 2.f * (j+1) * height / (*n_ports + 1));
					portsSide[j]->setOffset(offset_side + offset_j);
				}
				break;

			case 1:
				//Set east offsets
				offset_side = glm::vec2(width, -height);
				for (int j = 0; j < *n_ports; j++) 
				{
					glm::vec2 offset_j = glm::vec2(0.f, 2.f * (j + 1) * height / (*n_ports + 1));
					portsSide[j]->setOffset(offset_side + offset_j);
				}
				break;

			case 2:
				//Set north offsets
				offset_side = glm::vec2(-width, height);
				for (int j = 0; j < *n_ports; j++) 
				{
					glm::vec2 offset_j = glm::vec2(2.f * (j + 1) * width / (*n_ports + 1), 0.f);
					portsSide[j]->setOffset(offset_side + offset_j);
				}
				break;

			case 3:
				//Set south offsets
				offset_side = glm::vec2(-width, -height);
				for (int j = 0; j < *n_ports; j++) 
				{
					glm::vec2 offset_j = glm::vec2(2.f * (j + 1) * width / (*n_ports + 1), 0.f);
					portsSide[j]->setOffset(offset_side + offset_j);
				}
				break;
			}
		}
	}

}

void Component2D::translateTitle(glm::vec2 translation)
{
	titleOffset += translation;
	title->translate(translation);
}

void Component2D::updateText()
{
	std::string textString = equipType + std::string(": ") + titleString;
	title->updateText(textString);
}

void Component2D::addPoly(Polygon2D* poly)
{
	m_polygons.push_back(poly);
	m_polygons.back()->setColor(shapeColour);
	m_polygons.back()->setLayer(0.001f);//temp fix
}

void Component2D::addCircle(Circle* circle)
{
	m_circles.push_back(circle);
}

void Component2D::addLine(LineSegment* line)
{
	m_lines.push_back(line);
}

void Component2D::addPort(std::shared_ptr<Port> port)
{
	ports.push_back(port);
}

void Component2D::removePoly(Polygon2D* poly)
{
	auto to_remove = std::find(begin(m_polygons), end(m_polygons), poly);
	if (to_remove != m_polygons.end()) {
		Renderer::remove(poly);
		m_polygons.erase(to_remove);
	}
	else {
		LUMEN_LOG_WARN("Attempted to remove a polygon that is not a member of m_polygons.", "Component2D");
	}
}

void Component2D::removeCircle(Circle* circle)
{
	auto to_remove = std::find(begin(m_circles), end(m_circles), circle);
	if (to_remove != m_circles.end()) {
		Renderer::remove(circle);
		m_circles.erase(to_remove);
	}
	else {
		LUMEN_LOG_WARN("Attempted to remove a circle that is not a member of m_circles.", "Component2D");
	}
}

void Component2D::removeLine(LineSegment* line)
{
	auto to_remove = std::find(begin(m_lines), end(m_lines), line);
	if (to_remove != m_lines.end()) {
		Renderer::remove(line);
		m_lines.erase(to_remove);
	}
	else {
		LUMEN_LOG_WARN("Attempted to remove a line that is not a member of m_lines.", "Component2D");
	}
}

//=============================================================================================================================================//
//  Utilities.				     																											   //
//=============================================================================================================================================//

PortType Component2D::getPortType(YAML::Node node)
{
	if (node["Type"].as<std::string>() == "PORT_IN") { return PortType::PORT_IN; }
	else if (node["Type"].as<std::string>() == "PORT_OUT") { return PortType::PORT_OUT; }
	else if (node["Type"].as<std::string>() == "PORT_INOUT") { return PortType::PORT_INOUT; }
}


void Component2D::enableOutline()
{
	//shape->enableOutline();
	//border->enableOutline();
	title->enableOutline();
	for (auto poly : m_polygons) {
		poly->enableOutline();
	}
	for (auto line : m_lines) {
		line->enableOutline();
	}
	for (auto circ : m_circles) {
		circ->enableOutline();
	}
}

//void Component2D::destroy()
//{
	/*this->~Component2D();*/
	//shape->destroy();
	//border->destroy();
	//title->destroy();
//}

//=============================================================================================================================================//
//  EOF.																																	   //
//=============================================================================================================================================//