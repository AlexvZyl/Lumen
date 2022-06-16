//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include "ComponentDesignerColorEditor.h"
#include "Lumen/Lumen.h"
#include "Application/Application.h"
#include "Engines/CircuitDesigner/Peripherals/Cable.h"
#include "OpenGL/Primitives/Primitive.h"
#include "Graphics/Entities/Entity.h"
#include "Engines/CircuitDesigner/Peripherals/Port.h"
#include "Application/ApplicationTemplates.h"
#include "Graphics/OpenGL/Primitives/Circle.h"
#include "Graphics/OpenGL/Primitives/PolyLine.h"
#include "Graphics/OpenGL/Primitives/Polygon.h"
#include "Graphics/OpenGL/Primitives/Text.h"

//==============================================================================================================================================//
//  Popup menu.																																	//
//==============================================================================================================================================//

ComponentDesignerColorEditor::ComponentDesignerColorEditor(std::string name, int imguiWindowFlags, glm::vec4* const target)
	: LumenWindow(name, imguiWindowFlags), m_target(target)
{
	addImGuiWindowFlags(ImGuiWindowFlags_AlwaysAutoResize);
}


void ComponentDesignerColorEditor::onImGuiBegin()
{
	ImGui::SetNextWindowPos(m_initialPosition, ImGuiCond_Once);
	ImGui::Begin(getImGuiName(), &m_isOpen, getImGuiWindowFlags());
}

void ComponentDesignerColorEditor::onImGuiRender()
{
	ComponentDesigner* engine = Lumen::getApp().getActiveEngine<ComponentDesigner>();
	if (!engine) 
	{
		ImGui::Text("No active engine.");
		return;
	}

	// Get the active element.
	IPrimitive* activePrimitive = nullptr;
	Port* activePort = nullptr;
	Cable* activeCable = nullptr;
	if (engine->m_activeCircle)
	{
		activePrimitive = engine->m_activeCircle;
	}
	else if(engine->m_activeLine) 
	{
		activePrimitive = engine->m_activeLine;
	}
	else if (engine->m_activePoly) 
	{
		activePrimitive = engine->m_activePoly;
	}
	else if (engine->m_activeText) 
	{
		activePrimitive = engine->m_activeText;
	}
	else if(engine->m_activePort)
	{
		activePort = engine->m_activePort.get();
	}
	else if (false) 
	{
		// Should be the active cable.
	}

	// Set the color to be edited.
	glm::vec4* color = m_target;
	if (activePrimitive)
	{
		color = &activePrimitive->m_colour;
	}
	else if (activePort) 
	{
		color = &activePort->body->m_colour;
	}
	else if (activeCable)
	{
		color = &activeCable->m_colour;
	}

	// Open color editor.
	if (color)
	{
		ImGui::SameLine();
		if (ImGui::ColorPicker4("##ColorEditor", &color->r, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreviewHalf))
		{
			if (activePrimitive)
			{
				activePrimitive->setColor(*color);
			}
			else if (activeCable)
			{
				activeCable->m_colour = *color;
				activeCable->setColour(*color);
			}
			else if (activePort) 
			{
				activePort->body->setColor(*color);
			}
		}
	}
	else 
	{
		ImGui::Text("No active element.");
	}
}

void ComponentDesignerColorEditor::onImGuiEnd()
{
	ImGui::End();
}

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//