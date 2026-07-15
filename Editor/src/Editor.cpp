#include "Engine/EditorEngineInstance.h"
#include "Engine/EditorInstance.h"
#include "Engine/EditorApp.h"
#include <memory>
#include <functional>

TUniquePtr<ApplicationInstance> GenerateApplicationInstance()
{
    return CreateUniquePtr<EditorInstance>();
}

TUniquePtr<Application> GenerateApplication()
{
    return CreateUniquePtr<EditorApp>();
}