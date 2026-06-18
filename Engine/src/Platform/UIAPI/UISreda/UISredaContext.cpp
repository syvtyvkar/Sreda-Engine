#include "UISredaContext.h"
#include "Engine/Render/Renderer2D.h"
#include "Engine/Core/Application.h"

using namespace Engine;

namespace Engine::UI 
{
    UISredaContext::UISredaContext() : m_ui_camera(0.f,12800.f,0.,720.f)
    {
        Application::Get().GetWindow()->OnWindowReSize().Subscribe([&,this](int x, int y) 
		{
			m_ui_camera.SetProjection(0.0f, (float)x, 0.0f, (float)y);
		});
        int w = Application::Get().GetWindow()->GetWidth();
		int h = Application::Get().GetWindow()->GetHeight();
		m_ui_camera.SetProjection(0.0f, (float)w, 0.0f, (float)h);
		m_ui_camera.SetPosition(Vector3(0.0f));
		m_ui_camera.SetRotation(0.0f);
    }

    void UISredaContext::BeginFrame()
    {
    }
    void UISredaContext::EndFrame()
    {
    }
    void UISredaContext::Render()
    {
        Renderer2D::BeginScene(m_ui_camera);

        RenderUIElements(GetRootWidget());

        Renderer2D::EndScene();
    }
    void UISredaContext::Shutdown()
    {
        GetRootWidget().reset();
    }
    void UISredaContext::InitContext()
    {
        //SetRootWidget(CreateUniquePtr<UIWidget>());
    }

    void UISredaContext::Show()
    {
        m_bIsVisible =true;
    }
    void UISredaContext::Hide()
    {
        m_bIsVisible = false;
    }
    bool UISredaContext::IsVisible() const
    {
        return m_bIsVisible;
    }

    void UISredaContext::Update(float DeltaTime)
    {
        if (!IsVisible()) return;
        if (GetRootWidget().get() != nullptr)
        {
            GetRootWidget().get()->OnUpdate(DeltaTime);
        }
    }
}