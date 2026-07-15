#pragma once

#include <Engine.h>

#include "../Editor/Input/EditorInputComponent.h"
#include "Engine/Core/Utilities/Types.h"
#include "Engine/Render/Font/Font.h"

#include <memory>
#include <functional>

#include "Engine/Render/OrthographicCameraController.h"
#include "Engine/Render/Vertex.h"
#include "Engine/Render/Shader.h"
#include "Engine/Render/Texture.h"

using namespace Engine::Render;

class EditorInstance : public Engine::ApplicationInstance
{
public:

    EditorInstance();
    /*{
        
    };*/

    virtual ~EditorInstance()
    {
        
    };

    virtual void OnInit(class Application* InOwnerApp) override;
    virtual void DeInit()override;

    void OnStart() override;
    void OnEnd() override;
    virtual std::string GetNameApp() override {return "Editor";}

    virtual void Update(float DeltaTime) override;
    virtual void OnRender() override;
    void OnRenderUI() override;

    void CallOnWindowReSize(int x, int y);
private:
};