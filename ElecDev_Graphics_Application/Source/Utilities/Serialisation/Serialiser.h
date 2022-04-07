#pragma once

/*
Handles the serialising and deserialising of data.  First use case will be for
storing circuit templates/diagrams in a YAML format. 
*/

//=============================================================================================================================================//
//  Includes.																																   //
//=============================================================================================================================================//

#include <string>
#include "glm/glm.hpp"
#include "External/YAML-CPP/Includes/yaml-cpp/yaml.h"
#include <unordered_map>
#include <filesystem>

//=============================================================================================================================================//
//  Forward decelrations.																													   //
//=============================================================================================================================================//

// Classes.
class Circuit;
class Design2DEngine;
class Port;
class Component2D;
class Cable;
class LineSegment;
class Circle;
class Polygon2D;
class Text;

// Enums.
enum class PortType;

//=============================================================================================================================================//
//  Utilities.																																   //
//=============================================================================================================================================//

// Overloads the << operator so that YAML-cpp can accept glm::vec2.
YAML::Emitter& operator<<(YAML::Emitter& emitter, glm::vec2& vec);
// Overloads the << operator so that YAML-cpp can accept glm::vec3.
YAML::Emitter& operator<<(YAML::Emitter& emitter, glm::vec3& vec);
// Overloads the << operator so that YAML-cpp can accept glm::vec4.
YAML::Emitter& operator<<(YAML::Emitter& emitter, glm::vec4& vec);

// Dictionaries.
YAML::Emitter& operator<<(YAML::Emitter& emitter, std::unordered_map<std::string, std::string>& dict);

//=============================================================================================================================================//
//  General.																																   //
//=============================================================================================================================================//

// The saving function is not a generic function, but it should be.

// Serialise a circuit to a YAML file.
void saveToYAML(std::shared_ptr<Circuit>& circuit, const std::filesystem::path& path);
// Serialise a component to a YAML file
void saveToYAML(std::shared_ptr<Component2D>& component, const std::filesystem::path& path);
// Load a circuit from a YAML file.
void loadFromYAML(const std::filesystem::path& path);

// Serialise a circuit to a BIN file.
void saveToBIN(std::shared_ptr<Circuit>& circuit, const std::filesystem::path& path);
// Deserialise a circuit from a BIN file.
void loadFromBIN(const std::filesystem::path& path);

//=============================================================================================================================================//
//  Circuits.																																   //
//=============================================================================================================================================//

// Overloads the << operator so that YAML-cpp can work with circuits.
YAML::Emitter& operator<<(YAML::Emitter& emitter, std::shared_ptr<Circuit>& circuit);
// Overloads the << operator so that YAML-cpp can work with circuits.
YAML::Emitter& operator<<(YAML::Emitter& emitter, std::vector<std::shared_ptr<Circuit>>& circuitVector);

// Deserialise a single circuit.
std::shared_ptr<Circuit> deserialiseCircuit(YAML::Node& yamlNode, bool newEngine = false);

//=============================================================================================================================================//
//  Ports.																																	   //
//=============================================================================================================================================//

// Overloads the << operator so that YAML-cpp can work with port types.
YAML::Emitter& operator<<(YAML::Emitter& emitter, PortType type);
// Serialise a single port.
YAML::Emitter& operator<<(YAML::Emitter& emitter, std::shared_ptr<Port>& port);
// Serialise a port vector.
YAML::Emitter& operator<<(YAML::Emitter& emitter, std::vector<std::shared_ptr<Port>>& portVector);

// Deserialise a single port.
void deserialise(YAML::Node& yamlNode, std::shared_ptr<Port>& port);
// Helper to get the port type.
PortType getPortType(YAML::Node node);

//=============================================================================================================================================//
//  Componenet 2D.																															   //
//=============================================================================================================================================//

// Serialise a single Component2D.
YAML::Emitter& operator<<(YAML::Emitter& emitter, std::shared_ptr<Component2D>& comp);
// Serialise a Component2D vecrtor.
YAML::Emitter& operator<<(YAML::Emitter& emitter, std::vector<std::shared_ptr<Component2D>>& compVector);

// Deserialise a single Componenet2D.
void deserialise(YAML::Node& yamlNode, std::shared_ptr<Component2D>& component);

//=============================================================================================================================================//
//  Cables.																																	   //
//=============================================================================================================================================//

// Serialise a single cable.
YAML::Emitter& operator<<(YAML::Emitter& emitter, std::shared_ptr<Cable>& cable);
// Serialise a Cable vector.
YAML::Emitter& operator<<(YAML::Emitter& emitter, std::vector<std::shared_ptr<Cable>>& cableVector);

// Deserialise a single cable.
void deserialise(YAML::Node& yamlNode, std::shared_ptr<Cable>& cable);

//=============================================================================================================================================//
//  Primitives.																																   //
//=============================================================================================================================================//

// Text.
YAML::Emitter& operator<<(YAML::Emitter& emitter, Text* text);
YAML::Emitter& operator<<(YAML::Emitter& emitter, std::unique_ptr<Text>& text);
YAML::Emitter& operator<<(YAML::Emitter& emitter, std::shared_ptr<Text>& text);
//std::unique_ptr<Text>&& deserialiseText(YAML::Node& node) {};

// Polygon2D.
YAML::Emitter& operator<<(YAML::Emitter& emitter, Polygon2D* polygon2D);
YAML::Emitter& operator<<(YAML::Emitter& emitter, std::unique_ptr<Polygon2D> polygon2D);
YAML::Emitter& operator<<(YAML::Emitter& emitter, std::shared_ptr<Polygon2D> polygon2D);

// Circle.
YAML::Emitter& operator<<(YAML::Emitter& emitter, Circle* circle);
YAML::Emitter& operator<<(YAML::Emitter& emitter, std::unique_ptr<Circle> circle);
YAML::Emitter& operator<<(YAML::Emitter& emitter, std::shared_ptr<Circle> circle);

// Line Segment.
YAML::Emitter& operator<<(YAML::Emitter& emitter, LineSegment* lineSegment);
YAML::Emitter& operator<<(YAML::Emitter& emitter, std::unique_ptr<LineSegment> lineSegment);
YAML::Emitter& operator<<(YAML::Emitter& emitter, std::shared_ptr<LineSegment> lineSegment);

//=============================================================================================================================================//
//  EOF.																																	   //
//=============================================================================================================================================//