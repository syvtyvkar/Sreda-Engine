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

class EditorApp : public Application
    {
    public:
        EditorApp();                                  
        virtual ~EditorApp() override;                   
        virtual void RunApp(TRef<Engine::EngineCore> InEngine);               
    };