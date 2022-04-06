#pragma once

//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include "Events.h"
#include <memory>

//==============================================================================================================================================//
//  Event Log.																																	//
//==============================================================================================================================================//

// This is used to log the events as they happen per frame.
class EventLog
{

public:

	// Log an event to be handled in the next frame.
	template <typename EventType>
	inline static void log(const Event& event) 
	{
		if (typeid(EventType) == typeid(MouseMoveEvent))
		{
			mouseMove = std::make_unique<MouseMoveEvent>(dynamic_cast<const MouseMoveEvent&>(event));
		}
		else if (typeid(EventType) == typeid(MouseDragEvent))
		{
			// If event exists, increment delta.
			if (mouseDrag)
			{
				glm::vec2 delta = mouseDrag->currentFrameDelta;
				mouseDrag = std::make_unique<MouseDragEvent>(dynamic_cast<const MouseDragEvent&>(event));
				mouseDrag->currentFrameDelta += delta;
			}
			else 
			{
				mouseDrag = std::make_unique<MouseDragEvent>(dynamic_cast<const MouseDragEvent&>(event));
			}
		}
		else if (typeid(EventType) == typeid(MouseScrollEvent))
		{
			mouseScroll = std::make_unique<MouseScrollEvent>(dynamic_cast<const MouseScrollEvent&>(event));
		}
		else
		{
			events.emplace_back(std::make_unique<EventType>(dynamic_cast<const EventType&>(event)));
		}
	}

	// Setup the event log.
	inline static void init() 
	{
		events.reserve(15);
	}
	
	// Clears all of the events from the event log.
	inline static void clear() 
	{
		events.clear();
		mouseMove = nullptr;
		mouseDrag = nullptr;
		mouseScroll = nullptr;
	}

	// Store all of the events that occurred.
	inline static std::vector<std::unique_ptr<Event>> events;

	// These events are kept seperate, since we only want to 
	// handle one of them per frame.
	inline static std::unique_ptr<MouseMoveEvent> mouseMove = nullptr;
	inline static std::unique_ptr<MouseScrollEvent> mouseScroll = nullptr;
	inline static std::unique_ptr<MouseDragEvent> mouseDrag = nullptr;

private:

	EventLog();

};

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//
