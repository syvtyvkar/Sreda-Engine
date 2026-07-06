// (c) Nikita Rogalev. All rights reserved.

#pragma once // Preprocessor directive for multiple inclusion guard

#include "Engine/Core/Utilities/Types.h"
#include "Engine/Core/Utilities/LogSystem.h"
#include "Engine/Core/Utilities/Time.h"
#include "Engine/Core/FileSystem/NativeFileSystem.h"
#include "Engine/Core/EngineConfig.h"
#include "Engine/Core/Base/CommandLine.h"
#include "Engine/Platform/WindowManager.h"

namespace Engine
{
    struct EngineContext
    {
        TUniquePtr<Engine::Time::TimeSystem> m_TimeSystem;
        TUniquePtr<EngineConfig::ConfigSystem> m_ConfigSystem;
        TUniquePtr<Engine::IVirtualFileSystem> m_VFS;
        TUniquePtr<Engine::CommandLine> m_CommandLine;
        TUniquePtr<Engine::WindowManager> m_WindowManager;

        /*********************************************************************/
        Engine::Time::TimeSystem* GetTimeSystem() {return m_TimeSystem.get();}
        EngineConfig::ConfigSystem* GetConfigSystem() {return m_ConfigSystem.get();}
        Engine::IVirtualFileSystem* GetVFS() {return m_VFS.get();}
        Engine::CommandLine* GetEngineCommandLine() {return m_CommandLine.get();}
        Engine::WindowManager* GetWindowManager() {return m_WindowManager.get();}
    };

    class EngineCore
    {
    public:
        EngineCore() = default;
        ~EngineCore() = default;

        void PreInit(int argc, char **argv)
        {
            Engine::Log::LogSystem::Init();

            ENGINE_LOG_INFO("Engine pre init...");

            m_engineContext.m_TimeSystem = CreateUniquePtr<Engine::Time::TimeSystem>();
            m_engineContext.m_TimeSystem.get()->Init();

            m_engineContext.m_VFS = CreateUniquePtr<Engine::NativeFileSystem>();

            m_engineContext.m_VFS.get()->Mount("","/",0);


            m_engineContext.m_ConfigSystem = CreateUniquePtr<EngineConfig::ConfigSystem>();
            m_engineContext.m_ConfigSystem.get()->Init(EngineConfig::ENINGE_CONFIG_FILE);

            m_engineContext.m_CommandLine = CreateUniquePtr<EngineConfig::CommandLine>();

            ENGINE_LOG_INFO("Read arguments program:");
            for (int i = 0; i < argc; ++i) 
            {
                ENGINE_LOG_INFO("- Parameter argument: id: {} arg: {}", i, argv[i]);
            }
            m_engineContext.m_CommandLine.get()->Parse(argc, argv);

            m_engineContext.m_WindowManager = CreateUniquePtr<WindowManager>();
        };

        void Init()
        {
            ENGINE_LOG_INFO("Engine init...");
        };

        void Shotdown()
        {
            m_engineContext.m_WindowManager.reset();

            m_engineContext.m_CommandLine.get()->DeInit();
            m_engineContext.m_CommandLine.reset();

            m_engineContext.m_ConfigSystem.get()->DeInit();
            m_engineContext.m_ConfigSystem.reset();

            m_engineContext.m_VFS.get()->UnMount("");

            m_engineContext.m_VFS.reset();

            m_engineContext.m_TimeSystem.get()->Shutdown();
            m_engineContext.m_TimeSystem.reset();

            ENGINE_LOG_INFO("Engine close");
        };

        static EngineContext& GetEngineContext() {return m_engineContext;};

    private:
        static EngineContext m_engineContext;
    };
}