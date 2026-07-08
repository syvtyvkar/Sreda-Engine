#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "UILineEdit.h"
#include "Engine/Render/Renderer2D.h"
#include "Engine/Render/RenderCommand.h"
#include "Engine/Input/Input.h"
#include "Engine/Core/App/Application.h"
#include <algorithm>

namespace Engine::UI
{
    using namespace Engine::Render;

    static const char* ContextMenuLabels[] = {
        "Cut",
        "Copy",
        "Paste",
        "Delete",
        "Select All"
    };

    UILineEdit::UILineEdit(const std::string& InFontName)
    {
        m_FontName = InFontName;
        m_font = FontManager::GetFontManager().AddFont(InFontName);
        SetFocusable(true);

        m_charInputHandle = InputSystem::GetInstance()->OnCharInput().Subscribe(this, &UILineEdit::OnCallCharInput);
        m_clickHandle = OnClick().Subscribe(this, &UILineEdit::OnCallClick);
    }

    void UILineEdit::OnRender()
    {
        if (!IsVisible()) return;

        if (m_contextMenuVisible)
        {
            DrawContextMenu();
        }

        DrawTextContent();
        DrawControl();
       // DrawTextContent();

        UIWidget::OnRender();
    }

    void UILineEdit::OnUpdate(float deltaTime)
    {
        if (!IsVisible()) return;

        UpdateCursorBlink(deltaTime);
        m_timeSinceLastClick += deltaTime;

        Vector2 mousePos = InputSystem::GetMousePosition();
        bool overWidget = ContainsPoint(mousePos);

        if (m_contextMenuVisible)
        {
            HandleContextMenuInput();
            return;
        }

        if (InputSystem::IsMouseButtonJustPressed(InputKey::MouseRight) && overWidget)
        {
            Focus();
            ShowContextMenu();
            return;
        }

        if (m_focused)
        {
            HandleKeyInput();
        }

        bool mouseDown = InputSystem::IsMouseButtonPressed(InputKey::MouseLeft);

        if (mouseDown && overWidget)
        {
            if (!m_focused)
            {
                Focus();
            }

            if (!m_draggingSelection)
            {
                m_draggingSelection = true;
                m_wasDrag = false;
                m_dragStartPos = mousePos;
                Vector2 pos = GetComputedPosition();
                float localX = mousePos.x - pos.x - m_padding.left + m_scrollOffset;
                int glyphIndex = GetGlyphIndexAtX(localX);

                if (InputSystem::IsKeyPressed(InputKey::Shift))
                {
                    if (m_selectionStart < 0) m_selectionStart = m_cursorPos;
                    m_selectionEnd = glyphIndex;
                    m_cursorPos = glyphIndex;
                }
                else
                {
                    m_selectionStart = glyphIndex;
                    m_selectionEnd = glyphIndex;
                    m_cursorPos = glyphIndex;
                }
                m_cursorBlinkTime = 0.0f;
                m_cursorVisible = true;
                EnsureCursorVisible();
            }
            else
            {
                Vector2 pos = GetComputedPosition();
                float localX = mousePos.x - pos.x - m_padding.left + m_scrollOffset;
                int glyphIndex = GetGlyphIndexAtX(localX);

                float dx = mousePos.x - m_dragStartPos.x;
                float dy = mousePos.y - m_dragStartPos.y;
                if (dx * dx + dy * dy >= 16.0f)
                {
                    m_wasDrag = true;
                }

                m_selectionEnd = glyphIndex;
                m_cursorPos = glyphIndex;
                m_cursorBlinkTime = 0.0f;
                m_cursorVisible = true;
                EnsureCursorVisible();
            }
        }
        else if (m_draggingSelection)
        {
            m_draggingSelection = false;
            if (!m_wasDrag)
            {
                HandleMouseClick(mousePos);
            }
            m_wasDrag = false;
        }
    }

    void UILineEdit::OnCallCharInput(uint32_t codepoint)
    {
        if (m_focused && IsVisible())
        {
            HandleCharInput(codepoint);
        } 
    }

    void UILineEdit::OnCallClick()
    {
        if (IsVisible())
        {
            Focus();
        }
    }

    void UILineEdit::SetText(const std::string& text)
    {
        m_text = text;
        UpdateTextInternal();
        if (m_cursorPos > (int)m_wtext.length())
            m_cursorPos = (int)m_wtext.length();
        m_selectionStart = -1;
        m_selectionEnd = -1;
        EnsureCursorVisible();
    }

    void UILineEdit::SetFontSize(int size)
    {
        m_FontSize = size;
    }

    void UILineEdit::Focus()
    {
        m_focused = true;
        m_cursorBlinkTime = 0.0f;
        m_cursorVisible = true;
    }

    void UILineEdit::Blur()
    {
        m_focused = false;
        m_cursorVisible = false;
        m_selectionStart = -1;
        m_selectionEnd = -1;
        if (m_contextMenuVisible)
        {
            HideContextMenu();
        }
        if (!m_text.empty())
        {
            m_onTextCommitted.Broadcast(m_text);
        }
    }

    void UILineEdit::SetCursorPosition(int pos)
    {
        m_cursorPos = std::max(0, std::min(pos, (int)m_wtext.length()));
        m_cursorBlinkTime = 0.0f;
        m_cursorVisible = true;
        EnsureCursorVisible();
    }

    void UILineEdit::SelectAll()
    {
        m_selectionStart = 0;
        m_selectionEnd = (int)m_wtext.length();
    }

    void UILineEdit::ClearSelection()
    {
        m_selectionStart = -1;
        m_selectionEnd = -1;
    }

    std::string UILineEdit::GetSelectedText() const
    {
        if (!HasSelection()) return "";
        int start = std::min(m_selectionStart, m_selectionEnd);
        int end = std::max(m_selectionStart, m_selectionEnd);
        return m_text.substr(start, end - start);
    }

    void UILineEdit::UpdateTextInternal()
    {
        m_wtext.assign(m_text.begin(), m_text.end());
    }

    void UILineEdit::HandleKeyInput()
    {
        if (!m_focused) return;

        bool ctrl = InputSystem::IsKeyPressed(InputKey::Control);
        bool shift = InputSystem::IsKeyPressed(InputKey::Shift);

        if (InputSystem::IsKeyJustPressed(InputKey::Left)) { MoveCursorLeft(shift); }
        else if (InputSystem::IsKeyJustPressed(InputKey::Right)) { MoveCursorRight(shift); }
        else if (InputSystem::IsKeyJustPressed(InputKey::Home)) { MoveCursorHome(shift); }
        else if (InputSystem::IsKeyJustPressed(InputKey::End)) { MoveCursorEnd(shift); }
        else if (InputSystem::IsKeyJustPressed(InputKey::Backspace)) { Backspace(); }
        else if (InputSystem::IsKeyJustPressed(InputKey::Delete)) { DeleteForward(); }
        else if (InputSystem::IsKeyJustPressed(InputKey::Enter)) { Blur(); }
        else if (InputSystem::IsKeyJustPressed(InputKey::Escape)) { Blur(); }

        if (ctrl)
        {
            if (InputSystem::IsKeyJustPressed(InputKey::A)) { SelectAll(); }
            else if (InputSystem::IsKeyJustPressed(InputKey::C)) { CopySelection(); }
            else if (InputSystem::IsKeyJustPressed(InputKey::X)) { CutSelection(); }
            else if (InputSystem::IsKeyJustPressed(InputKey::V)) { PasteText(); }
        }
    }

    void UILineEdit::HandleCharInput(uint32_t codepoint)
    {
        if (!m_focused) return;

        if (codepoint >= 32 && codepoint != 127)
        {
            InsertAtCursor(codepoint);
        }
    }

    void UILineEdit::UpdateCursorBlink(float dt)
    {
        if (!m_focused) return;
        m_cursorBlinkTime += dt;
        if (m_cursorBlinkTime >= 0.5f)
        {
            m_cursorVisible = !m_cursorVisible;
            m_cursorBlinkTime = 0.0f;
        }
    }

    void UILineEdit::DrawControl()
    {
        Vector2 pos = GetComputedPosition();
        Vector2 size = GetComputedSize();

        TColor bgColor = m_BackgroundColor;
        if (m_focused) bgColor = TColor(35, 35, 40, 255);

        Renderer2D::DrawQuad(pos + size * 0.5f, size, bgColor);

        TColor borderColor = m_focused ? m_FocusBorderColor : m_BorderColor;
        Renderer2D::DrawRect({ pos.x + size.x * 0.5f, pos.y + size.y * 0.5f, 0.0f }, size, borderColor);
    }

    void UILineEdit::DrawCursor()
    {
        if (!m_cursorVisible || !m_focused) return;

        Vector2 pos = GetComputedPosition();
        Vector2 size = GetComputedSize();

        float cursorX = pos.x + m_padding.left + GetCursorXPosition() - m_scrollOffset;
        float cursorY = pos.y + m_padding.top + 2.0f;
        float cursorH = size.y - m_padding.top - m_padding.bottom - 4.0f;

        Renderer2D::DrawQuad({ cursorX, cursorY + cursorH * 0.5f }, { 1.5f, cursorH }, m_CursorColor);
    }

    void UILineEdit::DrawSelection()
    {
        if (!HasSelection()) return;

        Vector2 pos = GetComputedPosition();
        Vector2 size = GetComputedSize();

        int selStart = std::min(m_selectionStart, m_selectionEnd);
        int selEnd = std::max(m_selectionStart, m_selectionEnd);

        float scale = (float)m_FontSize / 48.0f;
        float startX = pos.x + m_padding.left;
        float x = 0.0f;

        for (int i = 0; i < (int)m_wtext.length(); i++)
        {
            wchar_t ch = m_wtext[i];
            auto it = m_font->GetGlyphs().find((uint32_t)ch);
            float advance = 0.0f;
            if (it != m_font->GetGlyphs().end())
            {
                advance = (it->second.advance >> 6) * scale;
            }
            else
            {
                advance = (m_FontSize / 48.0f) * 10.0f;
            }

            if (i >= selStart && i < selEnd)
            {
                float selX = startX + x - m_scrollOffset;
                float selY = pos.y + m_padding.top + 2.0f;
                float selH = size.y - m_padding.top - m_padding.bottom - 4.0f;

                Renderer2D::DrawQuad({ selX + advance * 0.5f, selY + selH * 0.5f }, { advance, selH }, m_SelectionColor);
            }

            x += advance;
        }

        if (selEnd == (int)m_wtext.length() && selEnd > selStart)
        {
            float selX = startX + x - m_scrollOffset;
            float selY = pos.y + m_padding.top + 2.0f;
            float selH = size.y - m_padding.top - m_padding.bottom - 4.0f;
            float endAdvance = (m_FontSize / 48.0f) * 8.0f;
            Renderer2D::DrawQuad({ selX + endAdvance * 0.5f, selY + selH * 0.5f }, { endAdvance, selH }, m_SelectionColor);
        }
    }

    void UILineEdit::DrawTextContent()
    {
        Vector2 pos = GetComputedPosition();
        Vector2 size = GetComputedSize();

        RenderCommand::EnableScissor(true);
        RenderCommand::SetScissor((int)pos.x, (int)pos.y, (int)size.x, (int)size.y);

        DrawSelection();
        DrawCursor();

        if (m_wtext.empty() && !m_focused && !m_placeholderText.empty())
        {
            std::wstring wplaceholder(m_placeholderText.begin(), m_placeholderText.end());
            float textX = pos.x + m_padding.left - m_scrollOffset;
            float textY = pos.y + m_padding.top + m_FontSize * 0.85f;
            Renderer2D::RenderDrawText(wplaceholder, m_font->GetAtlasTexture(), m_font->GetGlyphs(), textX, textY, m_FontSize, m_PlaceholderColor);
        }
        else if (!m_wtext.empty())
        {
            float textX = pos.x + m_padding.left - m_scrollOffset;
            float textY = pos.y + m_padding.top + m_FontSize * 0.85f;
            Renderer2D::RenderDrawText(m_wtext, m_font->GetAtlasTexture(), m_font->GetGlyphs(), textX, textY, m_FontSize, m_TextColor);
        }

        RenderCommand::EnableScissor(false);
    }

    int UILineEdit::GetGlyphIndexAtX(float x)
    {
        float scale = (float)m_FontSize / 48.0f;
        float currentX = 0.0f;
        for (int i = 0; i < (int)m_wtext.length(); i++)
        {
            auto it = m_font->GetGlyphs().find((uint32_t)m_wtext[i]);
            float advance = 0.0f;
            if (it != m_font->GetGlyphs().end())
            {
                advance = (it->second.advance >> 6) * scale;
            }
            else
            {
                advance = (m_FontSize / 48.0f) * 10.0f;
            }

            if (x < currentX + advance * 0.5f)
            {
                return i;
            }
            currentX += advance;
        }
        return (int)m_wtext.length();
    }

    float UILineEdit::GetCursorXPosition() const
    {
        float scale = (float)m_FontSize / 48.0f;
        float x = 0.0f;
        int end = std::min(m_cursorPos, (int)m_wtext.length());
        for (int i = 0; i < end; i++)
        {
            auto it = m_font->GetGlyphs().find((uint32_t)m_wtext[i]);
            if (it != m_font->GetGlyphs().end())
            {
                x += (it->second.advance >> 6) * scale;
            }
            else
            {
                x += (m_FontSize / 48.0f) * 10.0f;
            }
        }
        return x;
    }

    void UILineEdit::EnsureCursorVisible()
    {
        float cursorX = GetCursorXPosition();
        Vector2 size = GetComputedSize();
        float visibleWidth = size.x - m_padding.left - m_padding.right;

        if (cursorX - m_scrollOffset < 0.0f)
        {
            m_scrollOffset = cursorX;
        }
        else if (cursorX - m_scrollOffset > visibleWidth - 5.0f)
        {
            m_scrollOffset = cursorX - visibleWidth + 20.0f;
        }
    }

    void UILineEdit::DeleteSelection()
    {
        if (!HasSelection()) return;

        int start = std::min(m_selectionStart, m_selectionEnd);
        int end = std::max(m_selectionStart, m_selectionEnd);

        m_text.erase(start, end - start);
        m_cursorPos = start;
        m_selectionStart = -1;
        m_selectionEnd = -1;
        UpdateTextInternal();
        m_onTextChanged.Broadcast();
        EnsureCursorVisible();
    }

    void UILineEdit::InsertAtCursor(uint32_t codepoint)
    {
        if (HasSelection())
        {
            DeleteSelection();
        }

        wchar_t wc = (wchar_t)codepoint;
        std::wstring before = m_wtext.substr(0, m_cursorPos);
        std::wstring after = m_wtext.substr(m_cursorPos);
        m_wtext = before + wc + after;

        m_text.clear();
        m_text.assign(m_wtext.begin(), m_wtext.end());

        m_cursorPos++;
        m_selectionStart = -1;
        m_selectionEnd = -1;
        m_onTextChanged.Broadcast();
        EnsureCursorVisible();
    }

    void UILineEdit::Backspace()
    {
        if (HasSelection())
        {
            DeleteSelection();
            return;
        }

        if (m_cursorPos > 0)
        {
            std::wstring before = m_wtext.substr(0, m_cursorPos - 1);
            std::wstring after = m_wtext.substr(m_cursorPos);
            m_wtext = before + after;

            m_text.clear();
            m_text.assign(m_wtext.begin(), m_wtext.end());

            m_cursorPos--;
            m_onTextChanged.Broadcast();
            EnsureCursorVisible();
        }
    }

    void UILineEdit::DeleteForward()
    {
        if (HasSelection())
        {
            DeleteSelection();
            return;
        }

        if (m_cursorPos < (int)m_wtext.length())
        {
            std::wstring before = m_wtext.substr(0, m_cursorPos);
            std::wstring after = m_wtext.substr(m_cursorPos + 1);
            m_wtext = before + after;

            m_text.clear();
            m_text.assign(m_wtext.begin(), m_wtext.end());

            m_onTextChanged.Broadcast();
            EnsureCursorVisible();
        }
    }

    void UILineEdit::MoveCursorLeft(bool shift)
    {
        if (shift)
        {
            if (m_selectionStart < 0)
            {
                m_selectionStart = m_cursorPos;
                m_selectionEnd = m_cursorPos;
            }
            if (m_cursorPos > 0) m_cursorPos--;
            m_selectionEnd = m_cursorPos;
        }
        else
        {
            if (HasSelection())
            {
                m_cursorPos = std::min(m_selectionStart, m_selectionEnd);
                ClearSelection();
            }
            else if (m_cursorPos > 0)
            {
                m_cursorPos--;
            }
        }
        m_cursorBlinkTime = 0.0f;
        m_cursorVisible = true;
        EnsureCursorVisible();
    }

    void UILineEdit::MoveCursorRight(bool shift)
    {
        if (shift)
        {
            if (m_selectionStart < 0)
            {
                m_selectionStart = m_cursorPos;
                m_selectionEnd = m_cursorPos;
            }
            if (m_cursorPos < (int)m_wtext.length()) m_cursorPos++;
            m_selectionEnd = m_cursorPos;
        }
        else
        {
            if (HasSelection())
            {
                m_cursorPos = std::max(m_selectionStart, m_selectionEnd);
                ClearSelection();
            }
            else if (m_cursorPos < (int)m_wtext.length())
            {
                m_cursorPos++;
            }
        }
        m_cursorBlinkTime = 0.0f;
        m_cursorVisible = true;
        EnsureCursorVisible();
    }

    void UILineEdit::MoveCursorHome(bool shift)
    {
        if (shift)
        {
            if (m_selectionStart < 0)
            {
                m_selectionStart = m_cursorPos;
            }
            m_cursorPos = 0;
            m_selectionEnd = 0;
        }
        else
        {
            ClearSelection();
            m_cursorPos = 0;
        }
        m_cursorBlinkTime = 0.0f;
        m_cursorVisible = true;
        EnsureCursorVisible();
    }

    void UILineEdit::MoveCursorEnd(bool shift)
    {
        if (shift)
        {
            if (m_selectionStart < 0)
            {
                m_selectionStart = m_cursorPos;
            }
            m_cursorPos = (int)m_wtext.length();
            m_selectionEnd = m_cursorPos;
        }
        else
        {
            ClearSelection();
            m_cursorPos = (int)m_wtext.length();
        }
        m_cursorBlinkTime = 0.0f;
        m_cursorVisible = true;
        EnsureCursorVisible();
    }

    void UILineEdit::CopySelection()
    {
        if (!HasSelection()) return;

        std::string selected = GetSelectedText();
        if (!selected.empty())
        {
            if (OpenClipboard(nullptr))
            {
                EmptyClipboard();
                HGLOBAL hg = GlobalAlloc(GMEM_MOVEABLE, selected.size() + 1);
                if (hg)
                {
                    memcpy(GlobalLock(hg), selected.c_str(), selected.size() + 1);
                    GlobalUnlock(hg);
                    SetClipboardData(CF_TEXT, hg);
                }
                CloseClipboard();
            }
        }
    }

    void UILineEdit::CutSelection()
    {
        if (!HasSelection()) return;
        CopySelection();
        DeleteSelection();
    }

    void UILineEdit::PasteText()
    {
        if (!OpenClipboard(nullptr)) return;

        HANDLE hData = GetClipboardData(CF_TEXT);
        if (hData)
        {
            char* pszText = static_cast<char*>(GlobalLock(hData));
            if (pszText)
            {
                std::string pasteStr(pszText);
                GlobalUnlock(hData);

                if (HasSelection()) DeleteSelection();

                for (char c : pasteStr)
                {
                    if (c == '\n' || c == '\r') continue;
                    uint32_t codepoint = (unsigned char)c;
                    if (codepoint >= 32 && codepoint != 127)
                    {
                        InsertAtCursor(codepoint);
                    }
                }
            }
        }
        CloseClipboard();
    }

    void UILineEdit::ShowContextMenu()
    {
        Vector2 mousePos = InputSystem::GetMousePosition();
        m_contextMenuPos = mousePos;

        ENGINE_ASSERT(GetUIContext(), "No valid UIContext!");
        IWindow* LWin = EngineCore::GetEngineContext().GetWindowManager()->GetEngineWindow(GetUIContext().get()->GetWindowContext());
        ENGINE_ASSERT(LWin, "Error init EditorMainWidget. No valid window!");

        int windowW = LWin->GetWidth();
        int windowH = LWin->GetHeight();
        float menuH = (int)ContextMenuAction::Count * m_contextMenuItemHeight + 4.0f;

        if (m_contextMenuPos.x + m_contextMenuWidth > (float)windowW)
            m_contextMenuPos.x = (float)windowW - m_contextMenuWidth - 4.0f;
        if (m_contextMenuPos.x < 0.0f)
            m_contextMenuPos.x = 4.0f;
        if (m_contextMenuPos.y + menuH > (float)windowH)
            m_contextMenuPos.y = (float)windowH - menuH - 4.0f;
        if (m_contextMenuPos.y < 0.0f)
            m_contextMenuPos.y = 4.0f;

        m_contextMenuVisible = true;
        m_contextMenuHoverIndex = -1;
    }

    void UILineEdit::HideContextMenu()
    {
        m_contextMenuVisible = false;
        m_contextMenuHoverIndex = -1;
    }

    void UILineEdit::DrawContextMenu()
    {
        if (!m_contextMenuVisible) return;

        float menuW = m_contextMenuWidth;
        float itemH = m_contextMenuItemHeight;
        int itemCount = (int)ContextMenuAction::Count;
        float menuH = itemCount * itemH + 4.0f;

        Vector2 bgPos = m_contextMenuPos + Vector2(menuW * 0.5f, menuH * 0.5f);
        //Renderer2D::DrawQuad(bgPos, { menuW, menuH }, TColor(45, 45, 48, 240));
       // Renderer2D::DrawRect({ bgPos.x, bgPos.y, 0.0f }, { menuW, menuH }, TColor(100, 100, 100, 255));

        for (int i = 0; i < itemCount; i++)
        {
            float itemY = m_contextMenuPos.y + 2.0f + i * itemH;

            if (i == m_contextMenuHoverIndex)
            {
                Renderer2D::DrawQuad(
                    { m_contextMenuPos.x + menuW * 0.5f, itemY + itemH * 0.5f },
                    { menuW - 2.0f, itemH },
                    TColor(70, 70, 75, 255));
            }

            std::string label = ContextMenuLabels[i];
            std::wstring wlabel(label.begin(), label.end());
            float textX = m_contextMenuPos.x + 8.0f;
            float textY = itemY + itemH * 0.5f + 4.0f;
            Renderer2D::RenderDrawText(wlabel, m_font->GetAtlasTexture(), m_font->GetGlyphs(),
                textX, textY, 14, TColor(220, 220, 220, 255));
        }

        Renderer2D::DrawQuad(bgPos, { menuW, menuH }, TColor(45, 45, 48, 240));
        Renderer2D::DrawRect({ bgPos.x, bgPos.y, 0.0f }, { menuW, menuH }, TColor(100, 100, 100, 255));
    }

    void UILineEdit::HandleContextMenuInput()
    {
        Vector2 mousePos = InputSystem::GetMousePosition();

        float menuW = m_contextMenuWidth;
        float itemH = m_contextMenuItemHeight;
        int itemCount = (int)ContextMenuAction::Count;
        float menuH = itemCount * itemH + 4.0f;

        bool overMenu = mousePos.x >= m_contextMenuPos.x &&
                        mousePos.x <= m_contextMenuPos.x + menuW &&
                        mousePos.y >= m_contextMenuPos.y &&
                        mousePos.y <= m_contextMenuPos.y + menuH;

        if (overMenu)
        {
            int hoverIndex = (int)((mousePos.y - m_contextMenuPos.y - 2.0f) / itemH);
            if (hoverIndex >= 0 && hoverIndex < itemCount)
            {
                m_contextMenuHoverIndex = hoverIndex;
            }
            else
            {
                m_contextMenuHoverIndex = -1;
            }
        }
        else
        {
            m_contextMenuHoverIndex = -1;
        }

        if (InputSystem::IsMouseButtonJustPressed(InputKey::MouseLeft))
        {
            if (overMenu && m_contextMenuHoverIndex >= 0)
            {
                ContextMenuAction action = (ContextMenuAction)m_contextMenuHoverIndex;
                switch (action)
                {
                    case ContextMenuAction::Cut: CutSelection(); break;
                    case ContextMenuAction::Copy: CopySelection(); break;
                    case ContextMenuAction::Paste: PasteText(); break;
                    case ContextMenuAction::Delete: DeleteSelection(); break;
                    case ContextMenuAction::SelectAll: SelectAll(); break;
                    default: break;
                }
            }
            HideContextMenu();
        }
        else if (InputSystem::IsMouseButtonJustPressed(InputKey::MouseRight))
        {
        //    HideContextMenu();
        }

       // if (InputSystem::IsKeyJustPressed(InputKey::Escape))
       // {
       //     HideContextMenu();
       // }
    }

    void UILineEdit::SelectWordAtCursor()
    {
        if (m_wtext.empty()) return;

        int pos = std::min(m_cursorPos, (int)m_wtext.length());

        auto isWordChar = [](wchar_t ch) -> bool {
            return ch != L' ' && ch != L'\t' && ch != L'\n' && ch != L'\r' &&
                   ch != L'.' && ch != L',' && ch != L';' && ch != L':' &&
                   ch != L'!' && ch != L'?' && ch != L'(' && ch != L')' &&
                   ch != L'[' && ch != L']' && ch != L'{' && ch != L'}' &&
                   ch != L'\"' && ch != L'\'' && ch != L'-' && ch != L'_';
        };

        int start = pos;
        while (start > 0 && isWordChar(m_wtext[start - 1]))
            start--;

        int end = pos;
        while (end < (int)m_wtext.length() && isWordChar(m_wtext[end]))
            end++;

        if (start < end)
        {
            m_selectionStart = start;
            m_selectionEnd = end;
            m_cursorPos = end;
            m_cursorBlinkTime = 0.0f;
            m_cursorVisible = true;
            EnsureCursorVisible();
        }
    }

    void UILineEdit::HandleMouseClick(const Vector2& mousePos)
    {
        Focus();
        Vector2 pos = GetComputedPosition();
        float localX = mousePos.x - pos.x - m_padding.left + m_scrollOffset;
        int glyphIndex = GetGlyphIndexAtX(localX);

        float dx = mousePos.x - m_lastClickPos.x;
        float dy = mousePos.y - m_lastClickPos.y;
        bool samePos = (dx * dx + dy * dy) < 25.0f;

        if (m_timeSinceLastClick < 0.3f && samePos)
        {
            m_clickCount++;
            if (m_clickCount == 2)
            {
                SelectWordAtCursor();
                m_timeSinceLastClick = 0.0f;
                m_lastClickPos = mousePos;
                return;
            }
            else if (m_clickCount >= 3)
            {
                SelectAll();
                m_cursorPos = (int)m_wtext.length();
                m_timeSinceLastClick = 0.0f;
                m_lastClickPos = mousePos;
                EnsureCursorVisible();
                return;
            }
        }
        else
        {
            m_clickCount = 1;
        }

        if (!InputSystem::IsKeyPressed(InputKey::Shift))
        {
            m_cursorPos = glyphIndex;
            ClearSelection();
        }
        else
        {
            if (m_selectionStart < 0) m_selectionStart = m_cursorPos;
            m_cursorPos = glyphIndex;
            m_selectionEnd = glyphIndex;
        }

        m_lastClickPos = mousePos;
        m_timeSinceLastClick = 0.0f;
        m_cursorBlinkTime = 0.0f;
        m_cursorVisible = true;
        EnsureCursorVisible();
    }
}
