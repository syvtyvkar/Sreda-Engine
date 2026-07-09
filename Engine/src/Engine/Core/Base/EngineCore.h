// (c) Nikita Rogalev. All rights reserved.

#pragma once // Preprocessor directive for multiple inclusion guard

#include "Engine/Core/Utilities/Types.h"
#include "Engine/Core/Utilities/Event.h"

#include "Engine/Core/Utilities/LogSystem.h"
#include "Engine/Core/Utilities/Time.h"
#include "Engine/Core/FileSystem/NativeFileSystem.h"
#include "Engine/Core/EngineConfig.h"
#include "Engine/Core/Base/CommandLine.h"
#include "Engine/Platform/WindowManager.h"
#include "Engine/UI/Framework/UISystem.h"
#include "Engine/Input/Input.h"
#include "Engine/Render/Font/Font.h"

namespace Engine
{
    ADD_DELEGATE_ONE_PARAM(DOnAppUpdFrameDelta, float)
    ADD_DELEGATE(DOnAppUpdFrame)
    
    struct EngineContext
    {  
    friend class EngineCore;
        //Systems
        Engine::Time::TimeSystem* GetTimeSystem() {return m_TimeSystem.get();}
        EngineConfig::ConfigSystem* GetConfigSystem() {return m_ConfigSystem.get();}
        IVirtualFileSystem* GetVFS() {return m_VFS.get();}
        CommandLine* GetEngineCommandLine() {return m_CommandLine.get();}
        WindowManager* GetWindowManager() {return m_WindowManager.get();}
        Engine::UI::UISystem* GetUISystem() {return m_UISystem.get();}
        Engine::InputSystem* GetInputSystem() {return m_InputSystem.get();}
        AssetManager* GetAssetManager() {return m_AssetManager.get();}
        Engine::Render::FontManager* GetFontManager() {return m_FontManager.get();}

        /*********************************************************************/
    protected:
        //Systems
        TUniquePtr<Engine::Time::TimeSystem> m_TimeSystem;
        TUniquePtr<EngineConfig::ConfigSystem> m_ConfigSystem;
        TUniquePtr<IVirtualFileSystem> m_VFS;
        TUniquePtr<AssetManager> m_AssetManager;
        TUniquePtr<CommandLine> m_CommandLine;
        TUniquePtr<WindowManager> m_WindowManager;
        TUniquePtr<Engine::UI::UISystem> m_UISystem;
        TUniquePtr<Engine::InputSystem> m_InputSystem;
        TUniquePtr<Engine::Render::FontManager> m_FontManager;
    };

    struct EngineDelegates
    {
        DOnAppUpdFrameDelta& OnAppUpdFrameDelta() {return s_OnAppUpdFrameDelta;}
        DOnAppUpdFrame& OnAppBeginRender() {return s_OnAppBeginRender;}
        DOnAppUpdFrame& OnAppEndRender() {return s_OnAppEndRender;}
        DOnAppUpdFrame& OnAppRender() {return s_OnAppRender;}
        DOnAppUpdFrame& OnAppRenderUI() {return s_OnAppRenderUI;}
    private:
        DOnAppUpdFrameDelta s_OnAppUpdFrameDelta;
        DOnAppUpdFrame s_OnAppBeginRender;
        DOnAppUpdFrame s_OnAppEndRender;
        DOnAppUpdFrame s_OnAppRender;
        DOnAppUpdFrame s_OnAppRenderUI;
    };

    class EngineCore
    {
    public:
        EngineCore() = default;
        ~EngineCore()  = default;

        void PreInit(int argc, char **argv);

        void Init();

        void Shotdown();

        void FrameLoopUpdate();

        static EngineContext& GetEngineContext() {return m_engineContext;};
        static EngineDelegates& GetEngineDelegates() {return m_engineDelegates;};

    private:
        static EngineContext m_engineContext;
        static EngineDelegates m_engineDelegates;
        WindowContext m_InitWindow = WindowContext(-1);
    };
}