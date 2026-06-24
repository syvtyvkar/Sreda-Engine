#include "UISredaContext.h"
#include "Engine/Render/Renderer2D.h"
#include "Engine/Core/Application.h"
#include "Engine/Input/Input.h"

using namespace Engine;

namespace Engine::UI 
{
    UISredaContext::UISredaContext() : m_ui_camera(0.f,12800.f,720.f,0.f)
    {
        Application::Get().GetWindow()->OnWindowReSize().Subscribe([&,this](int x, int y) 
		{
			m_ui_camera.SetProjection(0.0f, (float)x, (float)y, 0.0f);
		});
        int w = Application::Get().GetWindow()->GetWidth();
		int h = Application::Get().GetWindow()->GetHeight();
		m_ui_camera.SetProjection(0.0f, (float)w, (float)h, 0.0f);
		m_ui_camera.SetPosition(Vector3(0.0f));
		m_ui_camera.SetRotation(0.0f);

        m_mousePressedHandle = InputSystem::GetInstance().OnMouseButtonPressed().Subscribe(
            [this](InputKey button, int mods)
            {
                if (button != InputKey::MouseLeft) return;
                Vector2 mousePos = InputSystem::GetMousePosition();
                TRef<UIWidget> hit = HitTest(GetRootWidget(), mousePos);
                if (hit)
                {
                    hit->SetPressed(true);
                    m_pressedWidget = hit;
                    hit->OnPressBegin().Broadcast();
                }
            });

        m_mouseReleasedHandle = InputSystem::GetInstance().OnMouseButtonReleased().Subscribe(
            [this](InputKey button, int mods)
            {
                if (button != InputKey::MouseLeft) return;
                if (m_pressedWidget)
                {
                    m_pressedWidget->SetPressed(false);
                    m_pressedWidget->OnPressEnd().Broadcast();
                    if (m_pressedWidget == m_hoveredWidget)
                    {
                        m_pressedWidget->OnClick().Broadcast();
                    }
                    m_pressedWidget = nullptr;
                }
            });
    }

    UISredaContext::~UISredaContext()
    {
        //InputSystem::GetInstance().OnMouseButtonPressed().Unsubscribe(m_mousePressedHandle);
        //InputSystem::GetInstance().OnMouseButtonReleased().Unsubscribe(m_mouseReleasedHandle);
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
        InputSystem::GetInstance().OnMouseButtonPressed().Unsubscribe(m_mousePressedHandle);
        InputSystem::GetInstance().OnMouseButtonReleased().Unsubscribe(m_mouseReleasedHandle);
        GetRootWidget().reset();
    }
    void UISredaContext::InitContext()
    {
    }

    void UISredaContext::Show()
    {
        m_bIsVisible = true;
    }
    void UISredaContext::Hide()
    {
        m_bIsVisible = false;
    }
    bool UISredaContext::IsVisible() const
    {
        return m_bIsVisible;
    }

    TRef<UIWidget> UISredaContext::HitTest(TRef<UIElement> root, const Vector2& point)
    {
        if (!root || !root->IsVisible()) return nullptr;

        for (auto it = root->GetChildren().rbegin(); it != root->GetChildren().rend(); ++it)
        {
            auto result = HitTest(*it, point);
            if (result) return result;
        }

        if (root->ContainsPoint(point))
        {
            auto widget = std::dynamic_pointer_cast<UIWidget>(root);
            if (widget) return widget;
        }

        return nullptr;
    }

    void UISredaContext::HandleInput()
    {
        Vector2 mousePos = InputSystem::GetMousePosition();
        if (mousePos.x == m_lastMousePos.x && mousePos.y == m_lastMousePos.y)
            return;
        m_lastMousePos = mousePos;

        TRef<UIWidget> newHovered = HitTest(GetRootWidget(), mousePos);

        if (newHovered != m_hoveredWidget)
        {
            if (m_hoveredWidget)
            {
                m_hoveredWidget->SetHovered(false);
                m_hoveredWidget->OnHoverEnd().Broadcast();
            }
            m_hoveredWidget = newHovered;
            if (m_hoveredWidget)
            {
                m_hoveredWidget->SetHovered(true);
                m_hoveredWidget->OnHoverBegin().Broadcast();
            }
        }
    }

    void UISredaContext::Update(float DeltaTime)
    {
        if (!IsVisible()) return;
        HandleInput();
        if (GetRootWidget().get() != nullptr)
        {
            GetRootWidget().get()->OnUpdate(DeltaTime);
        }
    }
}