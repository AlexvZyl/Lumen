#pragma once

//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include <chrono>
#include <iostream>
#include "Lumen/Lumen.h"

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

	// Constructor.
	Timer(const char* name);

	// Destructor.
	~Timer();

	// Stop the timer.
	void stop();

private:

	const char* m_name = nullptr;
	std::chrono::time_point<std::chrono::high_resolution_clock> m_startPoint;
	bool m_profilerActiveOnStart = false;
};

//==============================================================================================================================================//
//  Macro.																																		//
//==============================================================================================================================================//

#ifdef LUMEN_PROFILING_FUNCTIONS

	#define LUMEN_PROFILE_SCOPE(name) Timer timer##__LINE__(name)

#else

	#define LUMEN_PROFILE_SCOPE(name)

#endif

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//