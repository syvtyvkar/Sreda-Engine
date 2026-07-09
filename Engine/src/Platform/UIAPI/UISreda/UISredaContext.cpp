#include "UISredaContext.h"
#include "Engine/Render/Renderer2D.h"
#include "Engine/Core/App/Application.h"
#include "Engine/Input/Input.h"

using namespace Engine;

namespace Engine::UI 
{
    UISredaContext::UISredaContext() : m_ui_camera(0.f,12800.f,720.f,0.f)
    {
        IWindow* LWin = EngineCore::GetEngineContext().GetWindowManager()->GetEngineWindow(m_WindowContext);
        ENGINE_ASSERT(LWin, "Error init UI Context. No valid window!");
        LWin->OnWindowReSize().Subscribe(this, &UISredaContext::CallOnWindowReSize);
        int w = LWin->GetWidth();
		int h = LWin->GetHeight();
		m_ui_camera.SetProjection(0.0f, (float)w, (float)h, 0.0f);
		m_ui_camera.SetPosition(Vector3(0.0f));
		m_ui_camera.SetRotation(0.0f);

        m_mousePressedHandle = InputSystem::GetInstance()->OnMouseButtonPressed().Subscribe(
            [this](InputKey button, int mods)
            {
                if (button != InputKey::MouseLeft) return;
                Vector2 mousePos = InputSystem::GetMousePosition();
                TRef<UIWidget> hit = HitTest(GetRootWidget(), mousePos);

                if (hit.get() == nullptr) return;

                if (m_FocusWidget != nullptr && m_FocusWidget != hit)
                {
                    m_FocusWidget->SetPressed(false);
                    m_FocusWidget->SetHovered(false);
                    if (m_hoveredWidget == m_FocusWidget)
                        m_hoveredWidget.reset();
                    if (hit)
                    {
                        hit->OnClick().Broadcast();
                    }
                }

                if (hit)
                {
                    hit->SetPressed(true);
                    m_LastPressWidget = hit;
                    hit->OnPressBegin().Broadcast();

                    if (hit->IsFocusable() && m_FocusWidget != hit)
                    {
                        m_FocusWidget = hit;
                    }
                }
                else if (m_FocusWidget)
                {
                    m_FocusWidget.reset();
                }
            });

        m_mouseReleasedHandle = InputSystem::GetInstance()->OnMouseButtonReleased().Subscribe(
            [this](InputKey button, int mods)
            {
                if (button != InputKey::MouseLeft) return;
                if (m_LastPressWidget)
                {
                    m_LastPressWidget->SetPressed(false);
                    m_LastPressWidget->OnPressEnd().Broadcast();
                    if (m_LastPressWidget == m_hoveredWidget)
                    {
                        m_hoveredWidget->OnClick().Broadcast();
                    }
                    m_LastPressWidget.reset();
                }
            });
    }

    UISredaContext::~UISredaContext()
    {
        //InputSystem::GetInstance()->OnMouseButtonPressed().Unsubscribe(m_mousePressedHandle);
        //InputSystem::GetInstance()->OnMouseButtonReleased().Unsubscribe(m_mouseReleasedHandle);
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
        InputSystem::GetInstance()->OnMouseButtonPressed().Unsubscribe(m_mousePressedHandle);
        InputSystem::GetInstance()->OnMouseButtonReleased().Unsubscribe(m_mouseReleasedHandle);
        GetRootWidget().reset();
    }
    void UISredaContext::InitContext(Engine::WindowContext InContext)
    {
        m_WindowContext = InContext;
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

    TRef<UIWidget> UISredaContext::HitTest(TRef<UIElement> root, const Vector2 &point)
    {
        if (root == nullptr) return nullptr;
        if (!root->IsVisible()) return nullptr;

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
    void UISredaContext::CallOnWindowReSize(WindowContext cntxt, int x, int y)
    {
        m_ui_camera.SetProjection(0.0f, (float)x, (float)y, 0.0f);
    }
}