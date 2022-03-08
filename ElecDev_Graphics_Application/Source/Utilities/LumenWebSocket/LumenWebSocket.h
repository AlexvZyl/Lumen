#pragma once

//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include "boost/beast/core.hpp"
#include "boost/beast/websocket.hpp"
#include <string>

//==============================================================================================================================================//
//  Web Socket Class.																															//
//==============================================================================================================================================//

class LumenWebSocket 
{
public:

	// Constructor.
	LumenWebSocket(std::string& ip, unsigned short port);
	// Destructor.
	~LumenWebSocket();

	// Function that listens to web socket.
	static void listener(LumenWebSocket* socket);

private:

	boost::asio::ip::address m_socketAddress;
	unsigned short m_port = NULL;
	std::thread m_listenerThread;
};

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//
