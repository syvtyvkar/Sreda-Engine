#include "Engine/EditorEngineInstance.h"
#include <memory>
#include <functional>

std::unique_ptr<Engine::ApplicationInstance> GenerateApplicationInstance()
{
    return std::make_unique<EditorAppInstance>();
}