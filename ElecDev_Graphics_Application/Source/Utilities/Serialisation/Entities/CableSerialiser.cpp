//=============================================================================================================================================//
//  Includes.			     																												   //
//=============================================================================================================================================//

#include "../Serialiser.h"
#include "Graphics/OpenGL/Design2DEngineGL/Peripherals/Cable.h"

//=============================================================================================================================================//
//  Cable serialiser.     																													   //
//=============================================================================================================================================//

YAML::Emitter& operator<<(YAML::Emitter& emitter, std::shared_ptr<Cable> cable) 
{
	return emitter;
}

//=============================================================================================================================================//
//  Cable vector serialiser.																												   //
//=============================================================================================================================================//

YAML::Emitter& operator<<(YAML::Emitter& emitter, std::vector<std::shared_ptr<Cable>> cableVector)
{
	emitter << YAML::BeginMap;
	int cablesIndex = 0;
	for (std::shared_ptr<Cable> cable : cableVector)
	{
		// Begin cable.
		emitter << YAML::Key << "Cable " + std::to_string(cablesIndex) << YAML::Value << cable;
		cablesIndex++;
	}
	// End cables.
	emitter << YAML::EndMap;

	return emitter;
}

//=============================================================================================================================================//
//  EOF.				     																												   //
//=============================================================================================================================================//