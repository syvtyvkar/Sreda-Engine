#include "EngineCore.h"

#include "Engine/UI/Widgets/Base/UIInitWindow.h"

#include "Engine/Render/RenderCommand.h"
#include "Engine/Render/Renderer.h"
#include "Engine/Render/RenderAPIFactory.h"

namespace Engine
{
    EngineContext EngineCore::m_engineContext;
    EngineDelegates EngineCore::m_engineDelegates;

    void EngineCore::PreInit(int argc, char **argv)
    {
        Engine::Log::LogSystem::Init();

        ENGINE_LOG_INFO("Engine pre init...");

        m_engineContext.m_TimeSystem = CreateUniquePtr<Engine::Time::TimeSystem>();
        m_engineContext.m_TimeSystem.get()->Init();

        m_engineContext.m_VFS = CreateUniquePtr<Engine::NativeFileSystem>();

        Engine::ResourceManager::getInstance().init(Engine::PlatformUtils::GetExecutablePath());
        ENGINE_LOG_TRACE("Working Directory: {}", Engine::PlatformUtils::GetProjectDirectory());

        m_engineContext.m_VFS.get()->Mount("","/",0);


        m_engineContext.m_ConfigSystem = CreateUniquePtr<EngineConfig::ConfigSystem>();
        m_engineContext.m_ConfigSystem.get()->Init(EngineConfig::ENINGE_CONFIG_FILE);

        m_engineContext.m_CommandLine = CreateUniquePtr<EngineConfig::CommandLine>();

        m_engineContext.m_CommandLine.get()->Parse(argc, argv);

        m_engineContext.m_WindowManager = CreateUniquePtr<WindowManager>();

        m_engineContext.m_UISystem = CreateUniquePtr<Engine::UI::UISystem>();
        m_engineContext.m_UISystem.get()->Initialize();

        m_engineContext.m_InputSystem = CreateUniquePtr<Engine::InputSystem>();
        m_engineContext.m_InputSystem.get()->Init();

        RenderAPIFactory::Init();
    }

    void EngineCore::Init()
    {
        ENGINE_LOG_INFO("Engine init...");

        /*Engine::WindowConfig WindowConfig;
        WindowConfig.wight = 200;
        WindowConfig.height = 200;
        WindowConfig.title = "Init Engine...";
        WindowConfig.vsync = false;
	    WindowConfig.widget = std::make_shared<UIInitWindow>();

	    m_InitWindow = m_engineContext.m_WindowManager.get()->CreateEngineWindow(WindowConfig);

        Engine::Render::Renderer::Init();
        bool LWorkInit = true;
        while (LWorkInit)
        {
            Time::TimeSystem::Update();

            if (Time::TimeSystem::GetTime() >= 5.f)
            {
                LWorkInit = false;
            }
        }

        m_engineContext.m_WindowManager.get()->CloseEngineWindow(m_InitWindow);*/
    }

    void EngineCore::Shotdown()
    {
        Engine::Render::Renderer::Shutdown();

        m_engineContext.m_InputSystem.get()->Shutdown();
        m_engineContext.m_InputSystem.reset();

        m_engineContext.m_UISystem.get()->Shutdown();
        m_engineContext.m_UISystem.reset();

        m_engineContext.m_WindowManager.reset();

        m_engineContext.m_CommandLine.get()->DeInit();
        m_engineContext.m_CommandLine.reset();

        m_engineContext.m_ConfigSystem.get()->DeInit();
        m_engineContext.m_ConfigSystem.reset();

        m_engineContext.m_VFS.get()->UnMount("");

        m_engineContext.m_VFS.reset();

        m_engineContext.m_TimeSystem.get()->Shutdown();
        m_engineContext.m_TimeSystem.reset();

        m_engineDelegates.OnAppBeginRender().Clear();
        m_engineDelegates.OnAppUpdFrameDelta().Clear();
        m_engineDelegates.OnAppEndRender().Clear();

        ENGINE_LOG_INFO("Engine close");
    }

    void EngineCore::FrameLoopUpdate()
    {
        m_engineContext.GetTimeSystem()->Update();
        m_engineContext.GetWindowManager()->Update();
        m_engineContext.GetWindowManager()->BeginRender();
        m_engineDelegates.OnAppUpdFrameDelta().Broadcast(Time::TimeSystem::GetDeltaTime()); 
        m_engineDelegates.OnAppBeginRender().Broadcast();
        m_engineDelegates.OnAppRender().Broadcast();   
        m_engineDelegates.OnAppRenderUI().Broadcast();  
        m_engineDelegates.OnAppEndRender().Broadcast();
        m_engineContext.GetWindowManager()->EndRender();
    }
}