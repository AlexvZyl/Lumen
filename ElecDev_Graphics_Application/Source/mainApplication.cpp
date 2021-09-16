/*=======================================================================================================================================*/
/* Includes                                                                                                                              */
/*=======================================================================================================================================*/

// General.
#include <stdio.h>
#include <vector>
#include <iostream>
#include <string>
#include <thread>

// ImGUI (GUI software). 
#include "Core/imgui.h"
#include "Implementations/imgui_impl_glfw.h"
#include "Implementations/imgui_impl_opengl3.h"
#include "stateMachine.h"

// GLAD (OpenGL loader).
#include <glad/glad.h>

// Include GLFW (window) after OpenGL definition.
#include <GLFW/glfw3.h>

// GUI includes.
#include "GUI/guiHandler.h"

// Graphics handler include.
#include <../Graphics/graphicsHandler.h>

// Resources.
#include <Misc/stb_image.h>

/*=======================================================================================================================================*/
/* Compiler settings.                                                                                                                    */
/*=======================================================================================================================================*/

// This removes the console that keeps opening with the app if it is not in debug mode.
#ifdef	_DEBUG
#pragma comment(linker, "/SUBSYSTEM:console /ENTRY:mainCRTStartup")
#else
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif

/*=======================================================================================================================================*/
/* Variables/Globals/Defines.                                                                                                            */
/*=======================================================================================================================================*/

// Defined here, assigned in the main function where it has access to the window.
// Used as a global variable so that the mouse event callbacks from GLFW can have
// access to it.
GraphicsHandler* graphicsHandler;

/*=======================================================================================================================================*/
/* Functions.                                                                                                                            */
/*=======================================================================================================================================*/

// GLFW error handler.
static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "[GLFW ERROR] %d: %s\n", error, description);
}

// First part untill ";" is the function name
void procesInput(std::string inString, stateMachine* states, GraphicsHandler* graphicsHandler, GUIHandler* guiHandler) {

    std::string functionCall = inString.substr(inString.find("(:"), inString.length());

    functionCall = inString.substr(2, functionCall.find(":)")-2);

    if (functionCall == "StartMain") {
        states->startMainGraphics = true;
    }
    if (functionCall == "EndMain") {
        states->startMainGraphics = false;
    }

    std::cout << functionCall << std::endl;
}

void readingIn(stateMachine* states, GraphicsHandler* graphicsHandler, GUIHandler* guiHandler) {

    std::string temp;

    while (true) {

        std::cin >> temp;
        if (temp != "") {
            procesInput(temp, states, graphicsHandler, guiHandler);
        }
        temp = "";

    }
}

/*=======================================================================================================================================*/
/* Mouse events callbacks.                                                                                                               */
/*=======================================================================================================================================*/

// Handle mouse press events from GLFW.
void mousePressEvent(GLFWwindow* window, int button, int action, int mods)
{
    graphicsHandler->mousePressEvent(window, button, action, mods);
}

// Handle mouse press events from GLFW.
void mouseMoveEvent(GLFWwindow* window, double xpos, double ypos)
{
    graphicsHandler->mouseMoveEvent(window, xpos, ypos);
}

// Handle mouse press scroll. from GLFW.
void mouseScrollEvent(GLFWwindow* window, double xoffset, double yoffset)
{
    graphicsHandler->mouseScrollEvent(window, xoffset, yoffset);
}

/*=======================================================================================================================================*/
/* Main                                                                                                                                  */
/*=======================================================================================================================================*/

int main(int, char**)
{
    // Setup window.
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;

    /*-----------------------------------------------------------------------------------------------------------------------------------*/
    // OpenGL Version select.
    /*-----------------------------------------------------------------------------------------------------------------------------------*/

    #if defined(IMGUI_IMPL_OPENGL_ES2)
        // GL ES 2.0 + GLSL 100.
        const char* glsl_version = "#version 100";
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
        glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
    #elif defined(__APPLE__)
        // GL 3.2 + GLSL 150.
        const char* glsl_version = "#version 150";
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only.
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac.
    #else
        // GL 3.0 + GLSL 130.
        const char* glsl_version = "#version 450";
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
        //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only.
        //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only.
    #endif

    /*-----------------------------------------------------------------------------------------------------------------------------------*/
    /* GLFW setup.                                                                                                                       */
    /*-----------------------------------------------------------------------------------------------------------------------------------*/

    // Enable 16x MSAA.
    glfwWindowHint(GLFW_SAMPLES, 16);
    // Create GLFW window.
    GLFWwindow* window = glfwCreateWindow(1280, 720, "ElecDev", NULL, NULL);
    if (window == NULL)
        return 1;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync.

    // Set window icon.
    GLFWimage icon;
    icon.pixels = stbi_load("Source\\Resources\\Icons\\circuitIcon.bmp", &icon.width, &icon.height, NULL, STBI_rgb_alpha);
    glfwSetWindowIcon(window, 1, &icon);

    /*-----------------------------------------------------------------------------------------------------------------------------------*/
    // Initialize OpenGL loader.
    /*-----------------------------------------------------------------------------------------------------------------------------------*/

    #if defined(IMGUI_IMPL_OPENGL_LOADER_GL3W)
        bool err = gl3wInit() != 0;
    #elif defined(IMGUI_IMPL_OPENGL_LOADER_GLEW)
        bool err = glewInit() != GLEW_OK;
    #elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD)
        bool err = gladLoadGL() == 0;
    #elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD2)
        bool err = gladLoadGL(glfwGetProcAddress) == 0; // glad2 recommend using the windowing library loader instead of the (optionally) bundled one.
    #elif defined(IMGUI_IMPL_OPENGL_LOADER_GLBINDING2)
        bool err = false;
        glbinding::Binding::initialize();
    #elif defined(IMGUI_IMPL_OPENGL_LOADER_GLBINDING3)
        bool err = false;
        glbinding::initialize([](const char* name) { return (glbinding::ProcAddress)glfwGetProcAddress(name); });
    #else
        bool err = false; // If you use IMGUI_IMPL_OPENGL_LOADER_CUSTOM, your loader is likely to requires some form of initialization.
    #endif

    // OpenGL loader error handler.
    if (err)
    {
        fprintf(stderr, "[OPENGL][ERROR] Failed to initialize OpenGL loader!\n");
        return 1;
    }

    // Print OpenGL version.
    std::cout << "[OPENGL][INFO] Loaded OpenGL version: " << glGetString(GL_VERSION) << ".\n\n";

    /*-----------------------------------------------------------------------------------------------------------------------------------*/
    // ImGUI & OpenGL setup. 
    /*-----------------------------------------------------------------------------------------------------------------------------------*/

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.IniFilename = NULL;
    // Enable keyboard controls.
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
    //io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
    //io.ConfigViewportsNoAutoMerge = true;
    //io.ConfigViewportsNoTaskBarIcon = true;

    // Setup ImGui style.
    ImGui::StyleColorsDark();

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    // Setup Platform/Renderer backends.
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);
    glfwMakeContextCurrent(window);

    // Viewport.
    int screen_width, screen_height;
    glfwGetFramebufferSize(window, &screen_width, &screen_height);
    glViewport(0, 0, screen_width, screen_height);
    // Variables used in loop.
    int display_w, display_h;

    // Setup mouse callbacks.
    glfwSetMouseButtonCallback(window, mousePressEvent);    // Mouse press event.
    glfwSetCursorPosCallback(window, mouseMoveEvent);       // Mouse move event.
    glfwSetScrollCallback(window, mouseScrollEvent);        // Mouse scroll event.

    // Enable MSAA in OpenGL.
    glEnable(GL_MULTISAMPLE);

    // Create the state machine variables.
    stateMachine* states = new stateMachine();
    states->toolsExpanded = false;
    states->toolsMode = 0;

    // Create graphics handler object.
    // For now a global variable is used to be able to have mouse callbacks with a method.
    // The callbacks cannot be used with a method, so it has to call a normal function.
    graphicsHandler = new GraphicsHandler(window, states);

    // Create GUI handler object.
    GUIHandler guiHandler(states, graphicsHandler);

    /*===================================================================================================================================*/
    /* Loop                                                                                                                              */
    /*===================================================================================================================================*/

    // Loop variables.
    bool wait = false;      // Can be used to tell the app to wait for events and not run
                            // continuously in the background.
                            
    // Create thread that handles the Python/C++ interface.
    std::thread interfaceThread(readingIn,states,graphicsHandler,&guiHandler);

    // Graphics pipeline that draws OpenGL and ImGUI.
    while (!glfwWindowShouldClose(window))
    {
        // Event checking.
        if (wait) { glfwWaitEvents(); }   // App only runs when events occur.
        else { glfwPollEvents(); }        // App runs continuously.
    
        // Init colors for OpenGL.
        glClear(GL_COLOR_BUFFER_BIT);

        // Handle graphics (OpenGL engines: Drawing and Designing).
        graphicsHandler->renderGraphics();

        // Render ImGUI to screen.
        /*if (states->startMainGraphics)
        { guiHandler.renderGui(io); }*/
        guiHandler.renderGui(io);
        
        // Assign values to viewport (This can be moved to a GLFW callback for optimization).
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        // Swap the OpenGL buffers.
        glfwSwapBuffers(window);
    }

    /*===================================================================================================================================*/

    // Cleanup.
    interfaceThread.join();         // Join thread with main.
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    // Close application.
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

/*=======================================================================================================================================*/
/* EOF                                                                                                                                   */
/*=======================================================================================================================================*/