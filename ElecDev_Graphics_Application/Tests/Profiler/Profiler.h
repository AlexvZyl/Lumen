#pragma once

//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include <chrono>

//==============================================================================================================================================//
//  Timer.																																		//
//==============================================================================================================================================//

struct ProfileResult 
{
	// Constructor.
	ProfileResult(const char* name, float time) 
		: name(name), msTime(time)
	{}

	// Data.
	const char* name = nullptr;
	float msTime = 0;
};

//==============================================================================================================================================//
//  Timer.																																		//
//==============================================================================================================================================//

class Timer
{
public:

	// Constructor (starts the timer).
	Timer(const char* name);

	// Destructor (stop the timer).
	~Timer();

private:

	// Data.
	const char* m_name = nullptr;
	std::chrono::time_point<std::chrono::high_resolution_clock> m_startPoint;
};

//==============================================================================================================================================//
//  Macro.																																		//
//==============================================================================================================================================//

#define PROFILE_SCOPE(name) Timer timer##__LINE__(name)

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//