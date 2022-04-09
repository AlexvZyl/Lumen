//==============================================================================================================================================//
//  Includes.  																																    //
//==============================================================================================================================================//

#include "Circuit.h"
#include "Component2D.h"

//==============================================================================================================================================//
//  Constructor & Destructor.																												    //
//==============================================================================================================================================//

Circuit::Circuit(std::string label, std::string type)
	: Entity(EntityType::CIRCUIT), m_label(label), m_type(type)
{}

Circuit::~Circuit()
{
	m_components.clear();
	m_cables.clear();
}

Circuit::Circuit(const YAML::Node& node) 
	: Entity(EntityType::CIRCUIT)
{
	// Load components.
	for (const auto& component : node["Components"])
	{
		m_components.emplace_back(std::make_shared<Component2D>(component.second, this));
		m_components.back()->disableOutline();
	}
}

//==============================================================================================================================================//
//  EOF.    																																    //
//==============================================================================================================================================//