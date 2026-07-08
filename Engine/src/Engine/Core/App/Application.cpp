// (c) Nikita Rogalev. All rights reserved.

#include "Application.h"                            // Application class header
#include "Engine/Platform/PlatformUtils.h"          // Platform utilities (paths, etc.)
#include "Engine/Core/FileSystem/ResourceManager.h"            // Resource manager (textures, shaders, etc.)
#include "BaseEngine.h"
#include "Engine/Platform/IWindow.h"                // Window interface
#include "Engine/Input/Input.h"                     // Input system
#include "Engine/Core/Utilities/Time.h"                       // Time system (timers, delta time)
#include "Engine/Core/EngineConfig.h"               // Config file support

#include "Engine/Core/Integration/CustomGenerationInstance.h"               // Custom generation instance support

#include "Engine/Render/Camera.h"      
#include "Engine/Render/Renderer.h"    
#include "Engine/Render/RendererAPI.h"

#include <string>
#include <sstream>
#include <vector>

using namespace EngineConfig;

namespace Engine
{
    Application* Application::s_Instance=nullptr;                      // Static pointer to the single application instance (singleton)

    Application::Application() = default;                                   // Default constructor (empty, initialization happens in CreateNewApplication)
    Application::~Application() {CloseApp();}                               // Destructor: closes the app and frees resources

    TRef<Application> Application::CreateNewApplication()                        // Static method to create a new application instance (singleton)
    {
        ENGINE_LOG_INFO("Starting application...");                                     // Log the start of creation
        if (s_Instance)                                                                 // Check if an instance already exists
        {
            ENGINE_LOG_CRITICAL("CRITICAL ERROR! Dublicate application!");              // Error: duplicate
            return nullptr;
        }
        TRef<Application> LInstance = CreateRef<Application>();
        s_Instance = LInstance.get();                                    // Create a new object
        return LInstance;                                                              // Return pointer to the created instance
    }

    void Application::RunApp(TRef<Engine::EngineCore> InEngine)                         // Main application run method
    {
        ENGINE_LOG_INFO("Running app...");                                                          // Log the start of execution

        ENGINE_ASSERT(InEngine, "THE ENGINE FOR LAUNCHING THE APPLICATION WAS NOT FOUND!");

        m_Engine = InEngine;

        AddInstance(GenerateApplicationInstance());                              // Application instance

        if (!m_AppInstance)
        {
            ENGINE_LOG_CRITICAL("Application instance is not set!");
            return;
        }
    
        m_AppInstance.get()->OnStart();

        while (!EngineCore::GetEngineContext().GetWindowManager()->ShouldClose())                                                                 // MAIN GAME LOOP While window hasn't requested close
        {
            m_Engine.get()->FrameLoopUpdate();
        }
    }

    Application &Application::Get(){ return *s_Instance;}                                                   // Returns reference to the single application instance

    void Application::ExitApp()                                                                             // Request application exit (close window)
    {
        EngineCore::GetEngineContext().GetWindowManager()->CloseEngineWindow(EngineCore::GetEngineContext().GetWindowManager()->GetActivIWin());
    }

    void Application::AddInstance(TUniquePtr<ApplicationInstance> InInstance)
    {
        m_AppInstance=std::move(InInstance);
        if (m_AppInstance)
        {
            m_AppInstance.get()->OnInit(this);
        }
    }

    void Application::CloseApp() // Close application and free all resources
    {
        if (m_AppInstance)
        {
            m_AppInstance->DeInit();
            m_AppInstance.reset();
        }
        m_Engine.reset();
        s_Instance = nullptr;       // Reset instance pointer (singleton)
    }
}
