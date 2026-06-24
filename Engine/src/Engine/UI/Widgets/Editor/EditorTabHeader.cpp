#include "EditorTabHeader.h"
#include "Engine/Render/Renderer2D.h"
#include "Engine/Render/Font/Font.h"
#include "Engine/Input/Input.h"

namespace Engine::UI
{
    using namespace Engine::Render;

    EditorTabHeader::EditorTabHeader(const std::string& text)
        : m_tabText(text)
    {
        SetSize({ 120.0f, TAB_HEIGHT });
        GetStyle().backgroundColor = TColor(60, 60, 60, 0);
        GetStyle().hoveredBackgroundColor = TColor(80, 80, 80, 0);
        GetStyle().pressedBackgroundColor = TColor(50, 50, 50, 0);
        GetStyle().borderColor = TColor(40, 40, 40, 0);
        GetStyle().hoveredBorderColor = TColor(100, 100, 100, 0);
        GetStyle().borderWidth = 0.0f;
        GetStyle().cornerRadius = 0.0f;
    }

    void EditorTabHeader::OnInit()
    {
        __super::OnInit();

        OnClick().Subscribe([this]()
        {
            Vector2 mousePos = InputSystem::GetMousePosition();
            if (IsPointInCloseArea(mousePos))
            {
                m_onTabClose.Broadcast(m_tabIndex);
            }
            else
            {
                m_onTabSelect.Broadcast(m_tabIndex);
            }
        });
    }

    void EditorTabHeader::OnRender()
    {
        if (!IsVisible()) return;

        Vector2 pos = GetComputedPosition();
        Vector2 size = GetComputedSize();

        TColor bgColor(45, 45, 45, 255);
        if (m_isActive)
            bgColor = TColor(45, 45, 45, 255);
        else if (IsPressed())
            bgColor = TColor(50, 50, 50, 255);
        else if (IsHovered())
            bgColor = TColor(70, 70, 70, 255);
        else
            bgColor = TColor(55, 55, 55, 255);

        //Renderer2D::DrawQuad(pos + size * 0.5f, size, bgColor);

        if (m_isActive)
        {
            Renderer2D::DrawRect(
                Vector3(pos.x + size.x * 0.5f, pos.y + size.y - 2.0f, 0.0f),
                Vector2(size.x, 2.0f),
                TColor(200, 120, 40, 255));
        }

        TRef<Font> font = FontManager::GetFontManager().GetFontDefault();
        if (font)
        {
            Renderer2D::RenderDrawText(
                std::wstring(m_tabText.begin(), m_tabText.end()),
                font->GetAtlasTexture(),
                font->GetGlyphs(),
                pos.x + 8.0f, pos.y + size.y * 0.5f + 6.0f,
                9, 
                TColor(220, 220, 220, 255));
        }

        DrawCloseButton(pos, size);
    }

    void EditorTabHeader::DrawCloseButton(const Vector2& pos, const Vector2& size)
    {
        float cx = pos.x + size.x - CLOSE_BUTTON_SIZE - 4.0f;
        float cy = pos.y + (size.y - CLOSE_BUTTON_SIZE) * 0.5f;

        Vector2 mousePos = InputSystem::GetMousePosition();
        bool hovered = mousePos.x >= cx && mousePos.x <= cx + CLOSE_BUTTON_SIZE &&
                       mousePos.y >= cy && mousePos.y <= cy + CLOSE_BUTTON_SIZE;

        TColor closeColor = hovered ? TColor(255, 80, 80, 255) : TColor(160, 160, 160, 255);

        TRef<Font> font = FontManager::GetFontManager().GetFontDefault();
        if (font)
        {
            Renderer2D::RenderDrawText(
                std::wstring(L"\u00D7"),
                font->GetAtlasTexture(),
                font->GetGlyphs(),
                cx + 2.0f, cy + 2.0f,
                9, closeColor);
        }
    }

    bool EditorTabHeader::IsPointInCloseArea(const Vector2& point) const
    {
        Vector2 pos = GetComputedPosition();
        Vector2 size = GetComputedSize();
        float cx = pos.x + size.x - CLOSE_BUTTON_SIZE - 4.0f;
        float cy = pos.y + (size.y - CLOSE_BUTTON_SIZE) * 0.5f;
        return point.x >= cx && point.x <= cx + CLOSE_BUTTON_SIZE &&
               point.y >= cy && point.y <= cy + CLOSE_BUTTON_SIZE;
    }
}
