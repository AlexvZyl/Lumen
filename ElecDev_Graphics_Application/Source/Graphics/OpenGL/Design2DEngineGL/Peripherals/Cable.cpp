#include "Cable.h"
#include "GUI/GUIState.h"
#include "Circuit.h"


Cable::Cable(Port* startPort, VertexArrayObject<VertexData>* VAO, Circuit* parent):Entity(EntityType::CABLE, parent)
{
	//Keep the VAO and start port
	engine_VAO = VAO;
	m_startPort = startPort;

	//get the initial points and orientation of the first segment
	glm::vec2 endPt = m_startPort->centre;
	float initial_length = 0.01f;
	switch (startPort->m_position) {
	case PortPosition::TOP:
		m_curOrientation = LineOrientation::VERTICAL;
		//endPt += glm::vec2(0.f, initial_length);
		break;
	case PortPosition::BOTTOM:
		m_curOrientation = LineOrientation::VERTICAL;
		//endPt += glm::vec2(0.f, -initial_length);
		break;
	case PortPosition::LEFT:
		m_curOrientation = LineOrientation::HORIZONTAL;
		//endPt += glm::vec2(-initial_length, 0.f);
		break;
	case PortPosition::RIGHT:
		m_curOrientation = LineOrientation::HORIZONTAL;
		//endPt += glm::vec2(initial_length, 0.f);
		break;
	}
	
	//Add the first line segment.
	m_lines.push_back(std::make_shared<LineSegment>(m_startPort->centre, endPt, engine_VAO, this, m_thickness, m_colour));
	//Add the second (perpendicular) line segment.
	m_lines.push_back(std::make_shared<LineSegment>(m_startPort->centre, endPt, engine_VAO, this, m_thickness, m_colour));

}

void Cable::extendPrevSegment(glm::vec2 nextPoint)
{
	glm::vec2 endPt = m_lines.back()->m_start;
	switch (m_curOrientation) {
		//Update X-values for horizontal lines
	case LineOrientation::HORIZONTAL:
		endPt.x = nextPoint.x;
		break;
		//Update Y-values for vertical lines
	case LineOrientation::VERTICAL:
		endPt.y = nextPoint.y;
		break;
	}

	m_lines.end()[-2] = std::make_shared<LineSegment>(m_lines.end()[-2]->m_start, endPt, engine_VAO, this, m_thickness, m_colour);
}

void Cable::extendSegment(glm::vec2 nextPoint)
{
	//Extend the pevious segment 
	extendPrevSegment(nextPoint);
	m_lines.back() = std::make_shared<LineSegment>(m_lines.end()[-2]->m_end, nextPoint, engine_VAO, this, m_thickness, m_colour);
}

void Cable::addSegment(glm::vec2 nextPoint)
{
	extendSegment(nextPoint);
	switch (m_curOrientation) {
	case LineOrientation::HORIZONTAL:
		//switch the orientation for the next line
		m_curOrientation = LineOrientation::VERTICAL;
		break;
		//Update Y-values for vertical lines
	case LineOrientation::VERTICAL:
		//switch the orientation for the next line
		m_curOrientation = LineOrientation::HORIZONTAL;
		break;
	}
	m_lines.push_back(std::make_shared<LineSegment>(m_lines.back()->m_end, nextPoint, engine_VAO, this, m_thickness, m_colour));
}


void Cable::setContext(GUIState* guiState)
{
	guiState->clickedZone.component = true;
}

void Cable::attach(Port* endPort)
{
	m_endPort = endPort;
	extendSegment(m_endPort->centre);
}

void Cable::setColour(glm::vec4 colour)
{
	m_colour = colour;
	for (int i = 0; i < m_lines.size(); i++) {
		LineSegment* line = m_lines[i].get();
		line->setColor(m_colour);
	}
}


