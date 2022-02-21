#pragma once

/*=======================================================================================================================================*/
/* Includes.																															 */
/*=======================================================================================================================================*/

#include "imgui/imgui.h"
#include "ComponentEditor.h"
#include "ImGui/misc/cpp/imgui_stdlib.h"
#include "OpenGL/RendererGL.h"
#include "Engines/Design2DEngine/Peripherals/Component2D.h"
#include "Engines/Design2DEngine/Peripherals/Port.h"
#include "GUI/GuiElementCore/GuiElementCore.h"
#include "Application/Application.h"	
#include "ComponentEditorPopup/ComponentEditorPopup.h"

/*=======================================================================================================================================*/
/* Component Editor.																													 */
/*=======================================================================================================================================*/

ComponentEditor::ComponentEditor(std::string name, int windowFlags)
	: GuiElementCore(name, windowFlags)
{

}

void ComponentEditor::begin() 
{
	// Place editor at correct position.
	/*ImGui::SetNextWindowPos(m_guiState->popUpPosition);*/
	// FIX ME!! The wondow size should be set dynamically
	ImVec4 newCol = ImVec4(0.05f, 0.05f, 0.07f, 0.9f);
	ImGui::PushStyleColor(ImGuiCol_WindowBg, newCol);
	ImGui::SetNextWindowSize(ImVec2{ 600.f, 600.f }, ImGuiCond_Once);
	ImGui::Begin(m_name.c_str(), &m_isOpen, m_imguiWindowFlags);
}

void ComponentEditor::onRender()
{
	//	Fetch The active component.
	Component2D* activeComponent = Lumen::getApp().m_guiState->active_component;
	//check that the active component exists. Close if not.
	if (!activeComponent)
	{
		Lumen::getApp().m_guiState->componentEditor = false;
		return;
	}

	ImGui::Text(" Name:\t");
	ImGui::SameLine();
	if (ImGui::InputText("##ComponentName", &activeComponent->titleString))
	{
		activeComponent->title->updateText(activeComponent->titleString);
	}

	ImGui::Text(" Type:\t");
	ImGui::SameLine();
	ImGui::InputText("##Equipment Type", &activeComponent->equipType);

	// Get Active component type to change component editor based on type

	ImGui::SetNextItemOpen(true, ImGuiCond_Once);
	if (ImGui::TreeNode("Ports"))
	{
		ImGui::BeginTable("Current ports", 4, ImGuiTableFlags_Resizable | ImGuiTableFlags_SizingFixedFit);
		//ImGui::SetColumnWidth(1, 20.f);

			//Setup table
		ImGui::TableSetupColumn("Location    ");
		ImGui::TableSetupColumn("Port Name");
		ImGui::TableSetupColumn("I/O Type       ");
		ImGui::TableHeadersRow();

		ImGui::TableNextRow();
		std::vector<std::vector<std::shared_ptr<Port>>> allPorts = { activeComponent->portsWest,
																	 activeComponent->portsEast,
																	 activeComponent->portsNorth,
																	 activeComponent->portsSouth };

		static std::vector<std::string> portPositions = { "Left", "Right", "Top", "Bottom" };

		for (int i = 0; i < allPorts.size(); i++)
		{
			std::vector<std::shared_ptr<Port>> portsSide = allPorts[i];
			int j = 0;
			for (std::shared_ptr<Port> port : portsSide)
			{
				// Table labels.
				char labelName[20];
				sprintf_s(labelName, "##N%d,%d", i, j);
				char labelPos[20];
				sprintf_s(labelPos, "##P%d,%d", i, j);
				char labelType[20];
				sprintf_s(labelType, "##T%d,%d", i, j);
				char labelRemove[20];
				sprintf_s(labelRemove, "Remove##%d,%d", i, j++);

				// Port entry in table.
				ImGui::TableNextRow();
				ImGui::TableNextColumn();

				// Position.
				ImGui::PushItemWidth(-1);
				ImGui::Text(portPositions[i].c_str());
				ImGui::PopItemWidth();
				ImGui::TableNextColumn();

				// Name.
				ImGui::PushItemWidth(185.f);
				if (ImGui::InputText(labelName, &port->m_label))
					port->title->updateText(port->m_label);
			
				ImGui::PopItemWidth();
				ImGui::TableNextColumn();

				// Type.
				ImGui::PushItemWidth(-1);
				int* typeval = (int*)&port->m_type;
				ImGui::Combo(labelType, typeval, "IN\0OUT\0IN/OUT");
				ImGui::PopItemWidth();
				ImGui::TableNextColumn();

				// Remove.
				if (ImGui::Button(labelRemove)) 
					activeComponent->removePort(port);

			}
			if (j) ImGui::Separator();
		}

		if (addingPort) 
		{
			//create a table entry for the port to be added
			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			//Add the position
			ImGui::PushItemWidth(-1);
			ImGui::Combo("##newPos", &newPos, "Left\0Right\0Top\0Bottom");
			ImGui::PopItemWidth();
			ImGui::TableNextColumn();
			//Add the Name
			ImGui::PushItemWidth(-1);
			ImGui::InputText("##newName", &newName);
			ImGui::PopItemWidth();
			ImGui::TableNextColumn();
			//Add the type
			ImGui::PushItemWidth(-1);
			ImGui::Combo("##newType", &newType, "IN\0OUT\0IN/OUT");
			ImGui::PopItemWidth();
			ImGui::TableNextColumn();
			//Add a "Confirm" button
			if (ImGui::Button("Confirm"))
			{
				// Add the port to the component.
				activeComponent->addPort(newPos, (PortType)newType, newName);
				addingPort = false;
			}
		}
		ImGui::EndTable();
		if (!addingPort)
		{
			if (ImGui::Button("New Port"))
			{
				addingPort = true;
			}
		}
		ImGui::TreePop();
	}

	// --------------------- //
	//  D A T A   T A B L E  //
	// --------------------- //

	ImGui::SetNextItemOpen(true, ImGuiCond_Once);
	if (ImGui::TreeNode("Data Automation"))
	{
		// Add dict entry.
		static std::string entryToAdd;
		ImGui::Text("Add an attribute to the dictionary:");
		ImGui::InputText("##DictEntry", &entryToAdd);
		ImGui::SameLine();
		if (ImGui::Button("Add"))
		{
			activeComponent->cableDict.insert({entryToAdd, "From(Circuit Database)"});
			entryToAdd = "";
		}

		// Setup table.
		ImGui::BeginTable("Columns to specify", 3,	  ImGuiTableFlags_Resizable		| ImGuiTableFlags_SizingFixedFit 
													| ImGuiTableFlags_ScrollX		| ImGuiTableFlags_RowBg
													| ImGuiTableFlags_Borders);
		
		// Setup header.
		ImGui::TableSetupColumn("Attribute");
		ImGui::TableSetupColumn("Function");
		ImGui::TableSetupColumn("Action");
		ImGui::TableHeadersRow();

		// Store entries to be removed.
		static std::vector<std::string> toRemove;
		toRemove.reserve(1);

		// Table.
		ImGui::PushItemWidth(-1);
		for (auto& [key, val]: activeComponent->cableDict) 
		{
			// ID.
			ImGui::PushID((int)key.c_str());

			// Selectable.
			bool isOpen = true;
			ImGui::TableNextRow();
			
			// Dict data.
			ImGui::TableSetColumnIndex(0);
			/*if (ImGui::Selectable(key.c_str(), isOpen, ImGuiSelectableFlags_SpanAllColumns)) 
			{
				ComponentEditorPopup* popup = Lumen::getApp().pushGuiLayer<ComponentEditorPopup>("PopUp")->getGui();
				popup->setComponentEditor(this);
				popup->setPosition(getMousePosition());
			}*/
			ImGui::Text(key.c_str());
			ImGui::TableSetColumnIndex(1);
			ImGui::InputText("##Input", &val);
			ImGui::TableSetColumnIndex(2);

			// Remove button.
			if (ImGui::Button("Remove"))
				toRemove.push_back(key);

			// ID.
			ImGui::PopID();
		}
		ImGui::PopItemWidth();

		// Cleanup table.
		ImGui::EndTable();
		ImGui::TreePop();

		// Remove entries.
		for (auto& key : toRemove)
			activeComponent->cableDict.erase(key);
		toRemove.clear();
	}
}

void ComponentEditor::end()
{
	ImGui::PopStyleColor();
	ImGui::End();
}

/*=======================================================================================================================================*/
/* EOF.																																	 */
/*=======================================================================================================================================*/