
//=============================================================================================================================================//
//  Includes.																																   //
//=============================================================================================================================================//

#include "Design2DEngine.h"
#include "Peripherals/Component2D.h"
#include "Peripherals/Circuit.h"
#include "OpenGL/Renderer/RendererGL.h"
#include "OpenGL/SceneGL.h"
#include "External/GLFW/Includes/GLFW/glfw3.h"
#include "Graphics/Entities/EntityManager.h"
#include "Application/Application.h"

//=============================================================================================================================================//
//  Includes.																																   //
//=============================================================================================================================================//

void Design2DEngine::ComponentPlaceMode(glm::vec2 screenCoords)
{
	if (designerState != COMPONENT_PLACE)
	{
		designerState = COMPONENT_PLACE;
		// Add a dummy component.
		if (m_activeComponent) m_activeComponent->disableOutline();

		m_activeComponent = std::make_shared<Component2D>(screenCoords, m_circuit.get());
	}
}

void Design2DEngine::ComponentPlaceMode()
{
	glm::vec2 pos = getMouseLocalPosition();
	pos = pixelCoordsToWorldCoords(pos);
	ComponentPlaceMode(pos);
}

void Design2DEngine::deleteActiveComponent()
{
	if (!m_activeComponent) return;

	auto iterator = std::find(m_circuit->m_components.begin(), m_circuit->m_components.end(), m_activeComponent);
	if (iterator != m_circuit->m_components.end())
	{
		m_circuit->m_components.erase(iterator);
		m_activeComponent = nullptr;
		Lumen::getApp().m_guiState->active_component = nullptr;
		// Check if there is an hovered port.
		if (!m_hoveredPort) return;
		// Check to see if the hovered port was deleted.
		if (!EntityManager::getEntity(m_hoveredPort->m_entityID)) 
			m_hoveredPort = nullptr;
	}
}

void Design2DEngine::deleteActiveCable()
{
	if (!m_activeCable) return;

	auto iterator = std::find(m_circuit->m_cables.begin(), m_circuit->m_cables.end(), m_activeCable);
	if (iterator != m_circuit->m_cables.end())
	{
		m_circuit->m_cables.erase(iterator);
		m_activeCable = nullptr;
	}
}

void Design2DEngine::setActiveComponent(unsigned eID) 
{
	if (m_activeComponent)
	{
		m_activeComponent->disableOutline();
		m_activeComponent = NULL;
	}
	if ((eID == 0) || (eID == -1))
	{
		Lumen::getApp().m_guiState->clickedZone.background = true;
	}
	else
	{
		Lumen::getApp().m_guiState->clickedZone.background = false;
		Entity* currentEntity = EntityManager::getEntity(eID);
		if (!currentEntity) return;
		currentEntity->setContext();
		while (currentEntity->m_type != EntityType::COMPONENT)
		{
			currentEntity = currentEntity->m_parent;
			// Nothing was clicked.
			if (!currentEntity) return;
			if (currentEntity->m_parent == nullptr) return;
		}

		// This cast remains valid provided all entities on screen are decendents of components. 
		// If not, this needs to change.
		Component2D* cur = dynamic_cast<Component2D*>(currentEntity);
		m_activeComponent = *std::find_if(begin(m_circuit->m_components), end(m_circuit->m_components), [&](std::shared_ptr<Component2D> current)
			{
				return current.get() == cur;
			});
		m_activeComponent->enableOutline();

		// Pass the active component to the GUI state for editing.
		Lumen::getApp().m_guiState->active_component = m_activeComponent.get();
	}
}
void Design2DEngine::setActiveCable(unsigned eID) 
{
	if (m_activeCable)
	{
		m_activeCable->disableOutline();
		m_activeCable = NULL;
		Lumen::getApp().m_guiState->active_cable = nullptr;
	}
	if ((eID == 0) || (eID == -1)) 
	{
		Lumen::getApp().m_guiState->clickedZone.background = true;
	}
	else 
	{
		Lumen::getApp().m_guiState->clickedZone.background = false;
		Entity* currentEntity = EntityManager::getEntity(eID);

		if (!currentEntity) return;

		currentEntity->setContext();
		while (currentEntity->m_parent != nullptr) 
		{
			if (currentEntity->m_parent->m_type == EntityType::CABLE) 
			{
				// User clicked on a component. Set this cable as active, and send the primative to the component.
				Cable* cur = dynamic_cast<Cable*>(currentEntity->m_parent);
				//m_activeComponent = dynamic_cast<std::shared_ptr>(cur);
				m_activeCable = *std::find_if(begin(m_circuit->m_cables), end(m_circuit->m_cables), [&](std::shared_ptr<Cable> current)
					{
						return current.get() == cur;
					});
				m_activeCable->enableOutline();

				Lumen::getApp().m_guiState->active_cable = m_activeCable.get();
			}
			currentEntity = currentEntity->m_parent;
		}
	}
}

Port* Design2DEngine::getPort(unsigned eID)
{
	if ((eID == 0) || (eID == -1))
	{
		Lumen::getApp().m_guiState->clickedZone.background = true;
		return nullptr;
	}
	else
	{
		Lumen::getApp().m_guiState->clickedZone.background = false;
		Entity* currentEntity = EntityManager::getEntity(eID);
		if (!currentEntity) return nullptr;
		while (currentEntity->m_type != EntityType::PORT)
		{
			currentEntity = currentEntity->m_parent;
			if (!currentEntity) return nullptr;
		}

		// This cast remains valid provided all entities on screen are decendents of components. 
		// If not, this needs to change.
		Port* cur = dynamic_cast<Port*>(currentEntity);
		return cur;
	}
}

void Design2DEngine::setNameOfElements(const std::string& name) 
{
	m_circuit->m_label = name;
}

//=============================================================================================================================================//
//  EOF.																																	   //
//=============================================================================================================================================//