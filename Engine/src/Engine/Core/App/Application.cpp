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

    void Application::RunApp(std::string InNameApp)                         // Main application run method
    {
        ENGINE_LOG_INFO("Running app...");                                                          // Log the start of execution
        
        Engine::ResourceManager::getInstance().init(Engine::PlatformUtils::GetExecutablePath());    // Initialize resource manager with path to executable
        Time::TimeSystem::Init();                                                                   // Initialize time system

        auto& config = EngineConfig::ConfigSystem::Get();                                           // Get config system
        config.Init(ENINGE_CONFIG_FILE);                                                            // Initialize engine config

        m_AppWindow = WindowAPIFactory::create();                                                     // Create window through factory (platform-dependent)
        Engine::WindowConfig WindowConfig;                                                          // TODO: MOVE TO CONFIGS!!! Window parameter setup 
        WindowConfig.wight = config.Get<int>("window.width", 800);
        WindowConfig.height = config.Get<int>("window.height", 600);
        WindowConfig.title = "Init Sreda Engine...";
        WindowConfig.vsync = config.Get<bool>("render.vsync",false);
        config.Set<int>("window.width", 800);
        config.Set<int>("window.height", 600);
        config.SaveConfig(ENINGE_CONFIG_FILE);

        if (!m_AppWindow->Init(WindowConfig))                                                         // Attempt to initialize window with given parameters
        {                                                       
            ENGINE_LOG_CRITICAL("Failed to create window!");                                        // Window creation error
            return;
        }

        Engine::InputSystem::Init(m_AppWindow.get());                                                  // Initialize input system with window pointer

        ENGINE_LOG_TRACE("Working Directory: {}", Engine::PlatformUtils::GetProjectDirectory());

        Renderer::Init();

        AddInstance(GenerateApplicationInstance());                              // Application instance

        if (!m_AppInstance)
        {
            ENGINE_LOG_CRITICAL("Application instance is not set!");
            return;
        }

        m_AppWindow->SetTittle(m_AppInstance->GetNameApp());
        m_AppWindow->UpdateWindowName("");
    
        m_AppInstance.get()->OnStart();

        while (!m_AppWindow->ShouldClose())                                                                   // MAIN GAME LOOP While window hasn't requested close
        {
            //if (!m_AppWindow->GetCurrentRender()) return;                                                     // If renderer is lost, exit (abnormal)
            Time::TimeSystem::Update();                                                                     // Update time system
            m_AppWindow->Update();
            if (m_AppInstance)
            {
                m_AppInstance->Update(Time::TimeSystem::GetDeltaTime());
            }  
            m_AppWindow->BeginRender();    
            //m_AppWindow->Render(); 
            if (m_AppInstance)
            {
                m_AppInstance->Update(Time::TimeSystem::GetDeltaTime());
                m_AppInstance->OnRender();
                m_AppInstance->OnRenderUI();
            }  
            m_AppWindow->EndRender();  
        }
        Engine::InputSystem::Shutdown();                                                                     // Shutdown input system
        Time::TimeSystem::Shutdown();                                                                       // Shutdown time system
        config.DeInit();
    }

    Application &Application::Get(){ return *s_Instance;}                                                   // Returns reference to the single application instance

    void Application::ExitApp()                                                                             // Request application exit (close window)
    {
        Engine::Application::Get().m_AppWindow->ExitApp();
    }

    void Application::AddInstance(TUniquePtr<ApplicationInstance> InInstance)
    {
        m_AppInstance=std::move(InInstance);
        if (m_AppInstance)
        {
            m_AppInstance.get()->OnInit(this);
        }
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
        m_AppWindow.reset();          // Destroy window (destructor call)
        s_Instance = nullptr;       // Reset instance pointer (singleton)
    }
}
