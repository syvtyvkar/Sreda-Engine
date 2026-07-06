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

#include "Engine/Core/Base/EngineCore.h"

#include <string>
#include <sstream>
#include <vector>

using namespace EngineConfig;

namespace Engine
{
    Application* Application::s_Instance=nullptr;                           // Static pointer to the single application instance (singleton)

    Application::Application() = default;                                   // Default constructor (empty, initialization happens in CreateNewApplication)
    Application::~Application() {CloseApp();}                               // Destructor: closes the app and frees resources

    Application* Application::CreateNewApplication()                        // Static method to create a new application instance (singleton)
    {
        ENGINE_LOG_INFO("Starting application...");                                     // Log the start of creation
        if (s_Instance)                                                                 // Check if an instance already exists
        {
            ENGINE_LOG_CRITICAL("CRITICAL ERROR! Dublicate application!");              // Error: duplicate
            return nullptr;
        }
        s_Instance = new Application;                                                   // Create a new object
        return s_Instance;                                                              // Return pointer to the created instance
    }

    void Application::RunApp(TRef<Engine::EngineCore> InEngine)                         // Main application run method
    {
        ENGINE_LOG_INFO("Running app...");                                                          // Log the start of execution

        ENGINE_ASSERT(InEngine, "THE ENGINE FOR LAUNCHING THE APPLICATION WAS NOT FOUND!");

        m_Engine = InEngine;
        
        Engine::ResourceManager::getInstance().init(Engine::PlatformUtils::GetExecutablePath());    // Initialize resource manager with path to executable

        auto LConfig = EngineConfig::ConfigSystem::Get();                                           // Get config system
        Engine::WindowConfig WindowConfig;                                                          // TODO: MOVE TO CONFIGS!!! Window parameter setup 
        WindowConfig.wight = LConfig->Get<int>("window.width", 800);
        WindowConfig.height = LConfig->Get<int>("window.height", 600);
        WindowConfig.title = "Init Engine...";
        WindowConfig.vsync = LConfig->Get<bool>("render.vsync",false);
        LConfig->Set<int>("window.width", 800);
        LConfig->Set<int>("window.height", 600);
        LConfig->SaveConfig(ENINGE_CONFIG_FILE);

        Engine::InputSystem::Init();   // Initialize input system with window pointer

        m_Engine.get()->GetEngineContext().GetWindowManager()->CreateEngineWindow(WindowConfig);

        ENGINE_LOG_TRACE("Working Directory: {}", Engine::PlatformUtils::GetProjectDirectory());

        Renderer::Init();

        AddInstance(GenerateApplicationInstance());                              // Application instance

        if (!m_AppInstance)
        {
            ENGINE_LOG_CRITICAL("Application instance is not set!");
            return;
        }

        GetWindow()->SetTittle(m_AppInstance->GetNameApp());
        GetWindow()->UpdateWindowName("");
    
        m_AppInstance.get()->OnStart();

        while (!GetWindow()->ShouldClose())                                                                 // MAIN GAME LOOP While window hasn't requested close
        {
            //if (!GetWindow()->GetCurrentRender()) return;                                                 // If renderer is lost, exit (abnormal)
            Time::TimeSystem::Update();                                                                     // Update time system

            GetWindow()->Update();
            if (m_AppInstance)
            {
                m_AppInstance->Update(Time::TimeSystem::GetDeltaTime());
            }  
            GetWindow()->BeginRender();    
            //GetWindow()->Render(); 
            if (m_AppInstance)
            {
                m_AppInstance->Update(Time::TimeSystem::GetDeltaTime());
                m_AppInstance->OnRender();
                m_AppInstance->OnRenderUI();
            }  
            GetWindow()->EndRender();  
        }
        Engine::InputSystem::Shutdown();                                                                     // Shutdown input system
    }

    Application &Application::Get(){ return *s_Instance;}                                                   // Returns reference to the single application instance

    void Application::ExitApp()                                                                             // Request application exit (close window)
    {
        Engine::Application::Get().GetWindow()->ExitApp();
    }

    void Application::AddInstance(TUniquePtr<ApplicationInstance> InInstance)
    {
        m_AppInstance=std::move(InInstance);
        if (m_AppInstance)
        {
            m_AppInstance.get()->OnInit(this);
        }
    }

    IWindow *Application::GetWindow() const
    {
        return EngineCore::GetEngineContext().GetWindowManager()->GetEngineWindow(EngineCore::GetEngineContext().GetWindowManager()->GetActivIWin());
    }

    class Engine::UI::UISystem& Application::GetUISystem()
    {
        return *Engine::Application::Get().m_AppInstance.get()->GetUISystem();
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
