//=============================================================================================================================================//
//  Includes.			     																												   //
//=============================================================================================================================================//

#include "../Serialiser.h"
#include "Engines/Design2DEngine/Peripherals/Component2D.h"
#include "Graphics/Fonts/FontLoader.h"

//=============================================================================================================================================//
//  Component 2D serialiser.   																												   //
//=============================================================================================================================================//

YAML::Emitter& operator<<(YAML::Emitter& emitter, std::shared_ptr<Component2D>& comp) 
{
	// Begin component data.
	emitter << YAML::BeginMap;

	// Component data.
	emitter << YAML::Key << "Centre" << YAML::Value << comp->centre;
	emitter << YAML::Key << "Entity ID" << YAML::Value << comp->m_entityID;
	emitter << YAML::Key << "Component Layer" << YAML::Value << comp->componentLayer;
	emitter << YAML::Key << "Border layer offset" << YAML::Value << comp->borderLayerOffset;
	emitter << YAML::Key << "Layer offset" << YAML::Value << comp->portLayerOffset;
	emitter << YAML::Key << "Number of ports" << YAML::Value << comp->numPorts;
	emitter << YAML::Key << "Internal circuit" << YAML::Value << "Test AE 234";
	emitter << YAML::Key << "Dictionary" << YAML::Value << comp->dataDict;

	// Title.
	emitter << YAML::Key << "Title" << YAML::Value << comp->title;

	// Ports.
	emitter << YAML::Key << "Ports" << YAML::Value;
	emitter << YAML::BeginMap;
		emitter << YAML::Key << "Ports" << YAML::Value << comp->ports;
	emitter << YAML::EndMap;

	// Polygons.
	emitter << YAML::Key << "Polygons" << YAML::Value;
	emitter << YAML::BeginMap;
	int index = 0;
	for (auto& poly : comp->m_polygons)
	{
		emitter << YAML::Key << "Polygon " + std::to_string(index) << YAML::Value << poly;
		index++;
	}
	emitter << YAML::EndMap;

	// End componenet data.
	emitter << YAML::EndMap;
	return emitter;
}

//=============================================================================================================================================//
//  Component2D Vector serialiser.   																										   //
//=============================================================================================================================================//

YAML::Emitter& operator<<(YAML::Emitter& emitter, std::vector<std::shared_ptr<Component2D>>& compVector) 
{
	// Begin.
	emitter << YAML::BeginMap;
	int componentIndex = 0;
	for (std::shared_ptr<Component2D> comp : compVector)
	{
		// Begin component.
		emitter << YAML::Key << "Component " + std::to_string(componentIndex) << YAML::Value << comp;
		componentIndex++;
	}
	// Done.
	emitter << YAML::EndMap;
	return emitter;
}

//=============================================================================================================================================//
//  Deserialise single Component2D  																										   //
//=============================================================================================================================================//

void deserialise(YAML::Node& yamlNode, std::shared_ptr<Component2D>& component)
{

}

//=============================================================================================================================================//
//  EOF.				     																												   //
//=============================================================================================================================================//
