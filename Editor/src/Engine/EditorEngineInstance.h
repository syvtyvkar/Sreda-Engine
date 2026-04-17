#pragma once

#include <Engine.h>

#include "../Editor/Input/EditorInputComponent.h"
#include "Engine/Core/Utilities/Types.h"

#include <memory>
#include <functional>

class EditorAppInstance : public Engine::ApplicationInstance
{
    void OnStart() override;
    void OnEnd() override;
    virtual std::string GetNameApp() override {return "Sreda Engine Editor";}

private:

    TUniquePtr<EditorInputComponent> m_EditorInputHotKey;
};