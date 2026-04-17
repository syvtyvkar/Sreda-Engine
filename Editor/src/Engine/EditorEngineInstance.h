#pragma once

#include <Engine.h>

#include "../Editor/Input/EditorInputComponent.h"

#include <memory>
#include <functional>

class EditorAppInstance : public Engine::ApplicationInstance
{
    void OnStart() override;
    void OnEnd() override;
    virtual std::string GetNameApp() override {return "Sreda Engine Editor";}

private:

    std::unique_ptr<EditorInputComponent> m_EditorInputHotKey;
};