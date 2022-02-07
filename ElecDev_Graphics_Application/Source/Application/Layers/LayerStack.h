#pragma once

//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include <iostream>
#include "External/Misc/ConsoleColor.h"
#include <vector>
#include <memory>
#include "Application/Layers/Layer.h"

//==============================================================================================================================================//
//  Layer Stack.																																//
//==============================================================================================================================================//

class LayerStack 
{
public:

	// Constructor.
	LayerStack() = default;

	// Add a layer to the front of the stack.
	template<typename LayerType>
	void pushLayerToFront(Layer& layer);

	// Pop the layer in front of the stack.
	void popTopLayer();

	// Moves the specified layer to the front.
	void moveLayerToFront(Layer& layer);

	// Pop the specified layer.
	void popLayer(Layer& layer);

	// Get the layers in the stack.
	std::vector<std::unique_ptr<Layer>>& getLayers();

private:

	// Vector containing all of the layers.
	std::vector<std::unique_ptr<Layer>> m_layers;

};

//==============================================================================================================================================//
//  Operators.																																	//
//==============================================================================================================================================//

bool operator==(const std::unique_ptr<Layer>& layerUPtr, const Layer& layer);
bool operator!=(const std::unique_ptr<Layer>& layerUPtr, const Layer& layer);

//==============================================================================================================================================//
//  Templates.																																	//
//==============================================================================================================================================//

template<typename LayerType>
void LayerStack::pushLayerToFront(Layer& layer)
{
	m_layers.reserve(m_layers.size() + 2);  // Save one extra space to allow moving layers one at a time.
	m_layers.emplace_back(std::make_unique<LayerType>(std::move(dynamic_cast<LayerType&>(layer))));
}

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//