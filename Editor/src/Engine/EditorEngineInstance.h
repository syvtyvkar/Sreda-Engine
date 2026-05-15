#pragma once

#include <Engine.h>

#include "../Editor/Input/EditorInputComponent.h"
#include "Engine/Core/Utilities/Types.h"

#include <memory>
#include <functional>

#include "Engine/Render/OrthographicCameraController.h"
#include "Engine/Render/Vertex.h"
#include "Engine/Render/Shader.h"
#include "Engine/Render/Texture.h"

using namespace Engine::Render;

class EditorAppInstance : public Engine::ApplicationInstance
{
public:

    EditorAppInstance();
    /*{
        
    };*/

    virtual ~EditorAppInstance()
    {
        
    };

    virtual void OnInit(class Application* InOwnerApp) override;
    virtual void DeInit()override;

    void OnStart() override;
    void OnEnd() override;
    virtual std::string GetNameApp() override {return "Sreda Engine Editor";}

    virtual void Update(float DeltaTime) override;
    virtual void OnRender() override;
    virtual void OnRenderUI() override;

private:

    TUniquePtr<EditorInputComponent> m_EditorInputHotKey;

    ShaderLibrary m_ShaderLibrary;
	TRef<Shader> m_Shader;
	TRef<VertexArray> m_VertexArray;

	TRef<Shader> m_FlatColorShader;
	TRef<VertexArray> m_SquareVA;

	TRef<Texture2D> m_Texture, m_CheckerboardTexture;

	OrthographicCameraController m_CameraController;
	glm::vec3 m_SquareColor = { 0.2f, 0.3f, 0.8f };

    float m_Anim_x,m_Anim_y,m_Anim_z = 0.f;
    bool m_AnimIs_x,m_AnimIs_y,m_AnimIs_z = false;
};