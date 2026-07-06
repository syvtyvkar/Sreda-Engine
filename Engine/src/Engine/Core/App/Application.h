// (c) Nikita Rogalev. All rights reserved.

#pragma once // Preprocessor directive for multiple inclusion guard

#include <string>
#include "Engine/Platform/IWindow.h"        // Window interface
#include "Engine/Core/App/EngineInstance.h"     // Instances
#include "Engine/Core/Base/EngineCore.h"
#include <vector>

namespace Engine
{
    // Main application class (implements Singleton pattern)
    class Application
    {
    public:
        Application();                                          // Constructor
        ~Application();                                         // Destructor
        static Application* CreateNewApplication();             // Static method to create a single application instance
        void RunApp(TRef<Engine::EngineCore> InEngine);         // Run the main application loop, InNameApp - window title
        static Application& Get();                              // Get reference to the single instance (singleton)
        void ExitApp();                                         // Request application exit (close window)
        void AddInstance(TUniquePtr<ApplicationInstance> InInstance);
        IWindow* GetWindow() const;

        static class Engine::UI::UISystem& GetUISystem();
    private:
        void CloseApp();                                        // Internal method to free resources on close

        static Application* s_Instance;                         // Pointer to the single instance (singleton)
        bool m_running=true;                                    // Application running flag (controls the loop)

        TUniquePtr<ApplicationInstance>  m_AppInstance;       // Application instance
        TRef<Engine::EngineCore>         m_Engine;            // Engine        
    };
}
