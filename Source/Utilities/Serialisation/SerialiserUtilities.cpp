//=============================================================================================================================================//
//  Includes.																																   //
//=============================================================================================================================================//

#include "Serialiser.h"

//=============================================================================================================================================//
//  GLM serialisation.																														   //
//=============================================================================================================================================//

YAML::Emitter& operator<<(YAML::Emitter& emitter, glm::vec2& vec)
{
	return emitter << YAML::Flow << YAML::BeginSeq << vec[0] << vec[1] << YAML::EndSeq;
}

YAML::Emitter& operator<<(YAML::Emitter& emitter, glm::vec3& vec)
{
	return emitter << YAML::Flow << YAML::BeginSeq << vec[0] << vec[1] << vec[2] << YAML::EndSeq;
}

YAML::Emitter& operator<<(YAML::Emitter& emitter, glm::vec4& vec)
{
	return emitter << YAML::Flow << YAML::BeginSeq << vec[0] << vec[1] << vec[2] << vec[3] << YAML::EndSeq;
}

//=============================================================================================================================================//
//  Dicts.																																	   //
//=============================================================================================================================================//

YAML::Emitter& operator<<(YAML::Emitter& emitter, std::unordered_map<std::string, std::string>& dict)
{
	// Begin.
	emitter << YAML::BeginMap;

	// Data.
	for (auto& [key, value] : dict)
		emitter << YAML::Key << key << YAML::Value << value;

	// Done.
	emitter << YAML::EndMap;
	return emitter;
}

//=============================================================================================================================================//
//  EOF.																																	   //
//=============================================================================================================================================//