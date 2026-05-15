#include "Engine/EditorEngineInstance.h"
#include <memory>
#include <functional>

TUniquePtr<ApplicationInstance> GenerateApplicationInstance()
{
    return CreateUniquePtr<EditorAppInstance>();
}

/*TUniquePtr<Application> GenerateApplication()
{
    return CreateUniquePtr<Application>();
}*/