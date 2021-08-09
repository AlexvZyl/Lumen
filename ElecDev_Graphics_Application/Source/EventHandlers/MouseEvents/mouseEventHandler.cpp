/*=======================================================================================================================================*/
/* Includes                                                                                                                              */
/*=======================================================================================================================================*/

// Handlers.
#include "mouseEventHandler.h"

/*=======================================================================================================================================*/
/* Functions.                                                                                                                        */
/*=======================================================================================================================================*/

// Default constructor.
MouseEventHandler::MouseEventHandler() {};

// Constructor.
MouseEventHandler::MouseEventHandler(GraphicsHandler* graphicsHandler)
{
	// Give the mouse event handler access to the graphics handler.
	this->graphicsHandler = graphicsHandler;
};

/*=======================================================================================================================================*/
/* EOF.                                                                                                                              */
/*=======================================================================================================================================*/