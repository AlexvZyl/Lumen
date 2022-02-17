/*=======================================================================================================================================*/
/* Includes.																															 */
/*=======================================================================================================================================*/

#include "PopUpMenu.h"
#include <iostream>
#include "Core/imgui.h"
#include "Utilities/Windows/WindowsUtilities.h"
#include "Utilities/Serialisation/Serialiser.h"
#include "Engines/Design2DEngine/Design2DEngine.h"
#include "Engines/Design2DEngine/Peripherals/Circuit.h"
#include "Lumen.h"
#include "Application/Application.h"
#include "GUI/ComponentEditor/ComponentEditor.h"

/*=======================================================================================================================================*/
/* PopUp Menu.																															 */
/*=======================================================================================================================================*/

PopUpMenu::PopUpMenu(std::string name, int imguiWindowFlags)
    : GuiElementCore(name, imguiWindowFlags)
{
    m_imguiWindowFlags  |= ImGuiWindowFlags_NoDecoration
                        | ImGuiWindowFlags_NoMove
                        | ImGuiWindowFlags_NoDocking
                        | ImGuiWindowFlags_AlwaysAutoResize;
}

PopUpMenu::~PopUpMenu() 
{
    Lumen::getApp().m_guiState->clickedZone.background = false;
    Lumen::getApp().m_guiState->clickedZone.component = false;
    Lumen::getApp().m_guiState->clickedZone.primative= false;
    Lumen::getApp().m_guiState->clickedZone.port = false;
    Lumen::getApp().m_guiState->popUpMenu = false;
}

void PopUpMenu::setInitialPosition(glm::vec2& pos) 
{
    m_initialPos = pos;
}

void PopUpMenu::setEngine(Design2DEngine* engine)
{
    m_engine = engine;
}

void PopUpMenu::begin() 
{
    // Place at mouse position.
    ImGui::SetNextWindowPos(ImVec2(m_initialPos.x, m_initialPos.y));
    ImGui::Begin(m_name.c_str(), &m_isOpen, m_imguiWindowFlags);
}

/*=======================================================================================================================================*/
/* Rendering.																															 */
/*=======================================================================================================================================*/

// Render call.
void PopUpMenu::onRender()
{ 
    Application& app = Lumen::getApp();

    // --------------------- //
    //  B A C K G R O U N D  //
    // --------------------- //
    
    // Render menu items.
    if (m_engine->m_activeComponent == nullptr)
    {
        if (ImGui::MenuItem("Place component", "P"))
        {   
            // Place a dummy component.
            m_engine->ComponentPlaceMode();
            // Remove popup.
            app.queuePopLayer(m_name);
        }
    }

    // ------------------- //
    //  C O M P O N E N T  //
    // ------------------- //

    else if (m_engine->m_activeComponent)
    {
        if (ImGui::MenuItem("Component Editor", "E"))
        {
            // Pushing this GUI layer defocuses the popup, causing a 
            // defocus event, which removes the popup event.
            app.pushGuiLayer<ComponentEditor>("Component Editor");
        }
        //if (ImGui::MenuItem("Edit Ports", "P"))
        //{
        //    // Remove popup.
        //    Lumen::getApp().queuePopLayer(m_name);
        //}
        /*if (ImGui::MenuItem("Add Cable", "C"))
        {
            //
            // Remove popup.
            app.queuePopLayer(m_name);
        }*/
        if (ImGui::MenuItem("Remove component", "DEL"))
        {
            m_engine->deleteActiveComponent();

            // Remove popup.
            app.queuePopLayer(m_name);
        }
    }
    ImGui::Separator();

    // --------------- //
    //  D E F A U L T  //
    // --------------- //

    if (ImGui::MenuItem("Load Circuit...", "Ctrl+L"))
    {
        /*m_graphicsHandler->m_loadEvent.eventTrigger = true;
        m_graphicsHandler->m_loadEvent.path = selectFile("Lumen Load Circuit", "", "", "Load");*/
        //close();

        // Remove popup.
        app.queuePopLayer(m_name);
    }
    if (ImGui::MenuItem("Save Circuit...", "Ctrl+S"))
    {
        /*m_graphicsHandler->m_saveEvent.eventTrigger = true;
        m_graphicsHandler->m_saveEvent.saveEngine = m_windowContext;
        Design2DEngine* activeEngine = reinterpret_cast<Design2DEngine*>(m_graphicsHandler->m_activeWindow->engineGL.get());
        m_graphicsHandler->m_saveEvent.path = selectFile("Lumen Save Circuit", "", activeEngine->m_circuit->m_label, "Save");*/
        //close();

        // Remove popup.
        app.queuePopLayer(m_name);
    }
}

void PopUpMenu::end() 
{
    ImGui::End();
}

/*=======================================================================================================================================*/
/* EVents.																															     */
/*=======================================================================================================================================*/

void PopUpMenu::onDefocusEvent(LayerEvent& event) 
{
    Lumen::getApp().queuePopLayer(m_name);
}

/*=======================================================================================================================================*/
/* EOF.																																	 */
/*=======================================================================================================================================*/
