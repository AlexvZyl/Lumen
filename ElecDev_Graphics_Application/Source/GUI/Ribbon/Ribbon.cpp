//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include "Ribbon.h"
#include <Core/imgui.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include <cfenv>
#define STB_IMAGE_IMPLEMENTATION
#include <External/Misc/stb_image.h>

#define IMGUI_DEFINE_MATH_OPERATORS
#include <Core/imgui_internal.h>

#include "Lumen.h"

//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

// Constructor.
Ribbon::Ribbon(std::string name, int windowFlags)
    : GuiElementCore(name, windowFlags)
{
    m_imguiWindowFlags  |=  ImGuiWindowFlags_NoMove
                        |   ImGuiWindowFlags_NoDecoration;        

    // Load texture 1. 
    static BITMAP texture1BM = loadImageFromResource(DRAW_MCC_PNG);
    image1_width = texture1BM.bmWidth;
    image1_height = texture1BM.bmHeight;
    image1_texture = loadBitmapToGL(texture1BM);

    // Load texture 2.
    static BITMAP texture2BM = loadImageFromResource(COMPONENT_PNG);
    image2_width = texture2BM.bmWidth;
    image2_height = texture2BM.bmHeight;
    image2_texture = loadBitmapToGL(texture2BM);

    // Load texture 3.
    static BITMAP texture3BM = loadImageFromResource(DRAW_CIRCUIT_BUCKETS_PNG);
    image3_width = texture3BM.bmWidth;
    image3_height = texture3BM.bmHeight;
    image3_texture = loadBitmapToGL(texture3BM);

    // Load texture 4. 
    static BITMAP texture4BM = loadImageFromResource(CIRCUIT_CIRCLE_JPEG);
    image4_width = texture4BM.bmWidth;
    image4_height = texture4BM.bmHeight;
    image4_texture = loadBitmapToGL(texture4BM);
}

void Ribbon::begin()
{
    // Remove rounding so that it docks nicely.
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    // Setup ribbon.
    ImGui::SetNextWindowBgAlpha(1);
    ImGui::Begin(m_name.c_str(), &m_isOpen, m_imguiWindowFlags);
}

void Ribbon::renderBody()
{
    // -------------- //
    //  B U T T O N S //
    // -------------- //

    // Draw MCC.
    if (ImGui::ImageButton((void*)image1_texture, ImVec2(30, 30)))
    {

    }

    ImGui::SameLine();

    // Block diagram.
    if (ImGui::ImageButton((void*)image2_texture, ImVec2(30, 30)))
    {

    }
 
    ImGui::SameLine();

    // Circuit bucket.
    if (ImGui::ImageButton((void*)image3_texture, ImVec2(30, 30))) 
    { 

    }

    ImGui::SameLine();

    // Add design engine.
    if (ImGui::ImageButton((void*)image4_texture, ImVec2(30, 30))) 
    {

    }
}

void Ribbon::end() 
{
    ImGui::End();
    // Pop Window rounding.
    ImGui::PopStyleVar();
}

//==============================================================================================================================================//
//  EOF.																																	    //
//==============================================================================================================================================//