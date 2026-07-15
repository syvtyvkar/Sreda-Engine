#pragma once
#include "Engine/Core/Utilities/Types.h"    // Types
//#include "Engine/Core/App/Application.h"    // Types
#include "Engine/Core/App/EngineInstance.h"    // Types

extern Engine::TUniquePtr<Engine::ApplicationInstance> GenerateApplicationInstance(); // For connecting project code
extern Engine::TUniquePtr<Engine::Application> GenerateApplication();