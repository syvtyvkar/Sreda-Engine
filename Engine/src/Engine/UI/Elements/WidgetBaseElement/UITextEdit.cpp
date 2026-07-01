/*#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "UITextEdit.h"
#include "Engine/Render/Renderer2D.h"
#include "Engine/Render/RenderCommand.h"
#include "Engine/Input/Input.h"
#include "Engine/Core/Application.h"
#include <algorithm>
#include <cwchar>

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

    UITextEdit::UITextEdit(const std::string& InFontName)
    {
        m_FontName = InFontName;
        m_font = FontManager::GetFontManager().AddFont(InFontName);
        SetFocusable(true);

        m_charInputHandle = InputSystem::GetInstance().OnCharInput().Subscribe(
            [this](uint32_t codepoint)
            {
                if (m_focused && IsVisible())
                {
                    HandleCharInput(codepoint);
                }
            });

        m_clickHandle = OnClick().Subscribe([this]()
        {
            if (IsVisible())
            {
                Focus();
            }
        });
    }

    void UITextEdit::OnRender()
    {
        if (!IsVisible()) return;

        DrawBackground();
        DrawContent();
        DrawScrollbar();

        if (m_contextMenuVisible)
        {
            DrawContextMenu();
        }

        UIWidget::OnRender();
    }

    void UITextEdit::OnUpdate(float deltaTime)
    {
        if (!IsVisible()) return;

        UpdateCursorBlink(deltaTime);
        m_timeSinceLastClick += deltaTime;

        Vector2 mousePos = InputSystem::GetMousePosition();
        Vector2 pos = GetComputedPosition();
        Vector2 size = GetComputedSize();
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

        float sbX = pos.x + size.x - m_scrollbarWidth;
        bool overSb = m_showVertical && mousePos.x >= sbX && mousePos.x <= sbX + m_scrollbarWidth &&
                      mousePos.y >= pos.y && mousePos.y <= pos.y + size.y;

        if (overSb != m_hoveringScrollbar)
        {
            m_hoveringScrollbar = overSb;
        }

        if (InputSystem::IsMouseButtonJustPressed(InputKey::MouseLeft) && overSb)
        {
            m_draggingScrollbar = true;
            m_dragStartScroll = m_scrollY;
            m_dragStartMouse = mousePos;
        }

        if (m_draggingScrollbar)
        {
            if (InputSystem::IsMouseButtonPressed(InputKey::MouseLeft))
            {
                float contentH = GetContentHeight();
                float viewH = size.y;
                float maxScroll = std::max(0.0f, contentH - viewH);
                float thumbH = (viewH / std::max(contentH, viewH)) * (size.y);
                float trackH = size.y - thumbH;
                if (trackH > 0.0f)
                {
                    Vector2 mouseDelta = InputSystem::GetMousePosition() - m_dragStartMouse;
                    m_scrollY = m_dragStartScroll + (mouseDelta.y / trackH) * maxScroll;
                    ClampScroll();
                }
            }
            else
            {
                m_draggingScrollbar = false;
            }
        }

        float scrollDelta = (float)InputSystem::GetMouseScrollDelta();
        if (scrollDelta != 0.0f && ContainsPoint(mousePos))
        {
            m_scrollY -= scrollDelta * 40.0f;
            ClampScroll();
        }

        bool mouseDown = InputSystem::IsMouseButtonPressed(InputKey::MouseLeft);

        if (mouseDown && overWidget && !overSb)
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
                float localY = mousePos.y - pos.y - m_padding.top + m_scrollY;
                int line = GetLineFromY(localY);
                if (line >= GetLineCount()) line = std::max(0, GetLineCount() - 1);
                if (line < 0) line = 0;
                float localX = mousePos.x - pos.x - m_padding.left;
                int col = 0;
                if (!m_lines.empty() && line < (int)m_lines.size())
                    col = GetColumnFromX(m_lines[line].text, localX);

                if (InputSystem::IsKeyPressed(InputKey::Shift))
                {
                    if (m_selStartLine < 0)
                    {
                        m_selStartLine = m_cursorLine;
                        m_selStartCol = m_cursorColumn;
                    }
                    m_selEndLine = line;
                    m_selEndCol = col;
                }
                else
                {
                    m_selStartLine = line;
                    m_selStartCol = col;
                    m_selEndLine = line;
                    m_selEndCol = col;
                }
                m_cursorLine = line;
                m_cursorColumn = col;
                m_desiredColumnX = -1;
                m_cursorBlinkTime = 0.0f;
                m_cursorVisible = true;
                EnsureCursorVisible();
            }
            else
            {
                float localY = mousePos.y - pos.y - m_padding.top + m_scrollY;
                int line = GetLineFromY(localY);
                if (line >= GetLineCount()) line = std::max(0, GetLineCount() - 1);
                if (line < 0) line = 0;
                float localX = mousePos.x - pos.x - m_padding.left;
                int col = 0;
                if (!m_lines.empty() && line < (int)m_lines.size())
                    col = GetColumnFromX(m_lines[line].text, localX);

                float dx = mousePos.x - m_dragStartPos.x;
                float dy = mousePos.y - m_dragStartPos.y;
                if (dx * dx + dy * dy >= 16.0f)
                {
                    m_wasDrag = true;
                }

                m_selEndLine = line;
                m_selEndCol = col;
                m_cursorLine = line;
                m_cursorColumn = col;
                m_desiredColumnX = -1;
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

    void UITextEdit::SetText(const std::string& text)
    {
        m_text = text;
        UpdateTextInternal();
        WrapLines();

        if (m_cursorLine >= GetLineCount()) m_cursorLine = std::max(0, GetLineCount() - 1);
        if (!m_lines.empty() && m_cursorColumn > (int)m_lines[m_cursorLine].text.length())
            m_cursorColumn = (int)m_lines[m_cursorLine].text.length();

        m_selStartLine = -1;
        m_selStartCol = -1;
        m_selEndLine = -1;
        m_selEndCol = -1;
        EnsureCursorVisible();
    }

    void UITextEdit::SetFontSize(int size)
    {
        m_FontSize = size;
        WrapLines();
    }

    void UITextEdit::Focus()
    {
        m_focused = true;
        m_cursorBlinkTime = 0.0f;
        m_cursorVisible = true;
    }

    void UITextEdit::Blur()
    {
        m_focused = false;
        m_cursorVisible = false;
        m_selStartLine = -1;
        m_selStartCol = -1;
        m_selEndLine = -1;
        m_selEndCol = -1;
        if (m_contextMenuVisible)
        {
            HideContextMenu();
        }
        if (!m_text.empty())
        {
            m_onTextCommitted.Broadcast(m_text);
        }
    }

    void UITextEdit::SelectAll()
    {
        m_selStartLine = 0;
        m_selStartCol = 0;
        m_selEndLine = GetLineCount() - 1;
        if (m_selEndLine >= 0 && !m_lines.empty())
            m_selEndCol = (int)m_lines.back().text.length();
        else
            m_selEndCol = 0;
    }

    void UITextEdit::ClearSelection()
    {
        m_selStartLine = -1;
        m_selStartCol = -1;
        m_selEndLine = -1;
        m_selEndCol = -1;
    }

    bool UITextEdit::HasSelection() const
    {
        return m_selStartLine >= 0 && m_selEndLine >= 0 &&
               (m_selStartLine != m_selEndLine || m_selStartCol != m_selEndCol);
    }

    std::string UITextEdit::GetSelectedText() const
    {
        if (!HasSelection()) return "";

        int flatStart = 0, flatEnd = 0;
        int pos = 0;
        for (int l = 0; l < GetLineCount(); l++)
        {
            if (l == m_selStartLine) flatStart = pos + m_selStartCol;
            if (l == m_selEndLine) flatEnd = pos + m_selEndCol;
            pos += (int)m_lines[l].text.length() + 1;
        }

        if (flatStart > flatEnd) std::swap(flatStart, flatEnd);

        std::wstring wsel;
        pos = 0;
        for (int l = 0; l < GetLineCount(); l++)
        {
            int lineLen = (int)m_lines[l].text.length();
            if (pos + lineLen > flatStart && pos < flatEnd)
            {
                int start = std::max(0, flatStart - pos);
                int end = std::min(lineLen, flatEnd - pos);
                if (start < end)
                    wsel += m_lines[l].text.substr(start, end - start);
                if (pos + lineLen < flatEnd && l < GetLineCount() - 1)
                    wsel += L'\n';
            }
            pos += lineLen + 1;
        }

        std::string result;
        result.assign(wsel.begin(), wsel.end());
        return result;
    }

    void UITextEdit::UpdateTextInternal()
    {
        m_wtext.assign(m_text.begin(), m_text.end());
    }

    void UITextEdit::WrapLines()
    {
        m_lines.clear();

        if (m_wtext.empty())
        {
            m_lines.push_back({ L"", 0.0f });
            return;
        }

        float scale = (float)m_FontSize / 48.0f;
        Vector2 size = GetComputedSize();
        float maxWidth = size.x - m_padding.left - m_padding.right - m_scrollbarWidth - 4.0f;
        if (maxWidth < 10.0f) maxWidth = 10.0f;

        std::vector<std::wstring> hardLines;
        std::wstring currentHard;
        for (wchar_t ch : m_wtext)
        {
            if (ch == L'\n')
            {
                hardLines.push_back(currentHard);
                currentHard.clear();
            }
            else
            {
                currentHard += ch;
            }
        }
        hardLines.push_back(currentHard);

        for (const auto& hardLine : hardLines)
        {
            if (hardLine.empty())
            {
                m_lines.push_back({ L"", 0.0f });
                continue;
            }

            std::wstring remaining = hardLine;
            while (!remaining.empty())
            {
                float lineWidth = 0.0f;
                int breakPos = -1;
                int lastSpace = -1;
                int i = 0;

                for (wchar_t ch : remaining)
                {
                    float chWidth = 0.0f;
                    auto it = m_font->GetGlyphs().find((uint32_t)ch);
                    if (it != m_font->GetGlyphs().end())
                        chWidth = (it->second.advance >> 6) * scale;
                    else
                        chWidth = 10.0f * scale;

                    if (lineWidth + chWidth > maxWidth)
                    {
                        if (lastSpace > 0)
                        {
                            breakPos = lastSpace;
                        }
                        else if (i > 0)
                        {
                            breakPos = i;
                        }
                        break;
                    }

                    if (ch == L' ') lastSpace = i;

                    lineWidth += chWidth;
                    i++;
                }

                if (breakPos < 0)
                {
                    m_lines.push_back({ remaining, lineWidth });
                    remaining.clear();
                }
                else
                {
                    m_lines.push_back({ remaining.substr(0, breakPos), 0.0f });
                    remaining = remaining.substr(breakPos);
                    if (!remaining.empty() && remaining[0] == L' ')
                        remaining = remaining.substr(1);
                }
            }
        }

        for (auto& line : m_lines)
        {
            float w = 0.0f;
            for (wchar_t ch : line.text)
            {
                auto it = m_font->GetGlyphs().find((uint32_t)ch);
                if (it != m_font->GetGlyphs().end())
                    w += (it->second.advance >> 6) * scale;
                else
                    w += 10.0f * scale;
            }
            line.width = w;
        }

        if (m_lines.empty())
            m_lines.push_back({ L"", 0.0f });
    }

    void UITextEdit::HandleKeyInput()
    {
        if (!m_focused) return;

        bool ctrl = InputSystem::IsKeyPressed(InputKey::Control);
        bool shift = InputSystem::IsKeyPressed(InputKey::Shift);

        if (InputSystem::IsKeyJustPressed(InputKey::Left)) { MoveCursorLeft(shift); }
        else if (InputSystem::IsKeyJustPressed(InputKey::Right)) { MoveCursorRight(shift); }
        else if (InputSystem::IsKeyJustPressed(InputKey::Up)) { MoveCursorUp(shift); }
        else if (InputSystem::IsKeyJustPressed(InputKey::Down)) { MoveCursorDown(shift); }
        else if (InputSystem::IsKeyJustPressed(InputKey::Home)) { MoveCursorHome(shift); }
        else if (InputSystem::IsKeyJustPressed(InputKey::End)) { MoveCursorEnd(shift); }
        else if (InputSystem::IsKeyJustPressed(InputKey::PageUp)) { MoveCursorPageUp(shift); }
        else if (InputSystem::IsKeyJustPressed(InputKey::PageDown)) { MoveCursorPageDown(shift); }
        else if (InputSystem::IsKeyJustPressed(InputKey::Backspace)) { Backspace(); }
        else if (InputSystem::IsKeyJustPressed(InputKey::Delete)) { DeleteForward(); }
        else if (InputSystem::IsKeyJustPressed(InputKey::Enter))
        {
            if (HasSelection()) DeleteSelection();
            InsertAtCursor(L'\n');
        }
        else if (InputSystem::IsKeyJustPressed(InputKey::Escape)) { Blur(); }

        if (ctrl)
        {
            if (InputSystem::IsKeyJustPressed(InputKey::A)) { SelectAll(); }
            else if (InputSystem::IsKeyJustPressed(InputKey::C)) { CopySelection(); }
            else if (InputSystem::IsKeyJustPressed(InputKey::X)) { CutSelection(); }
            else if (InputSystem::IsKeyJustPressed(InputKey::V)) { PasteText(); }
        }
    }

    void UITextEdit::HandleCharInput(uint32_t codepoint)
    {
        if (!m_focused) return;
        if (codepoint >= 32 && codepoint != 127)
        {
            InsertAtCursor(codepoint);
        }
    }

    void UITextEdit::UpdateCursorBlink(float dt)
    {
        if (!m_focused) return;
        m_cursorBlinkTime += dt;
        if (m_cursorBlinkTime >= 0.5f)
        {
            m_cursorVisible = !m_cursorVisible;
            m_cursorBlinkTime = 0.0f;
        }
    }

    void UITextEdit::DrawBackground()
    {
        Vector2 pos = GetComputedPosition();
        Vector2 size = GetComputedSize();

        TColor bg = m_focused ? TColor(35, 35, 40, 255) : m_BackgroundColor;
        Renderer2D::DrawQuad(pos + size * 0.5f, size, bg);

        TColor borderColor = m_focused ? m_FocusBorderColor : m_BorderColor;
        Renderer2D::DrawRect({ pos.x + size.x * 0.5f, pos.y + size.y * 0.5f, 0.0f }, size, borderColor);
    }

    void UITextEdit::DrawScrollbar()
    {
        if (!m_showVertical) return;

        Vector2 pos = GetComputedPosition();
        Vector2 size = GetComputedSize();
        float contentH = GetContentHeight();
        float viewH = size.y;

        float sbX = pos.x + size.x - m_scrollbarWidth;
        float sbY = pos.y;
        float sbH = size.y;

        Renderer2D::DrawQuad({ sbX + m_scrollbarWidth * 0.5f, sbY + sbH * 0.5f }, { m_scrollbarWidth, sbH }, m_scrollbarBgColor);

        float thumbH = (viewH / contentH) * sbH;
        float maxScroll = contentH - viewH;
        float thumbY = sbY + (m_scrollY / maxScroll) * (sbH - thumbH);

        TColor tc = m_hoveringScrollbar ? m_scrollbarThumbHoverColor : m_scrollbarThumbColor;
        Renderer2D::DrawQuad({ sbX + m_scrollbarWidth * 0.5f, thumbY + thumbH * 0.5f }, { m_scrollbarWidth - 2.0f, thumbH }, tc);
    }

    void UITextEdit::DrawContent()
    {
        Vector2 pos = GetComputedPosition();
        Vector2 size = GetComputedSize();

        RenderCommand::EnableScissor(true);
        RenderCommand::SetScissor((int)pos.x, (int)pos.y, (int)size.x, (int)size.y);

        DrawSelection();
        DrawTextLines();
        DrawCursor();

        RenderCommand::EnableScissor(false);
    }

    void UITextEdit::DrawCursor()
    {
        if (!m_cursorVisible || !m_focused) return;
        if (m_lines.empty()) return;

        Vector2 pos = GetComputedPosition();
        float lineH = GetLineHeight();

        float cx = pos.x + m_padding.left + GetXFromColumn(m_lines[m_cursorLine].text, m_cursorColumn);
        float cy = pos.y + m_padding.top + m_cursorLine * lineH - m_scrollY;

        Renderer2D::DrawQuad({ cx, cy + lineH * 0.5f }, { 1.5f, lineH - 2.0f }, m_CursorColor);
    }

    void UITextEdit::DrawSelection()
    {
        if (!HasSelection()) return;
        if (m_lines.empty()) return;

        Vector2 pos = GetComputedPosition();
        float lineH = GetLineHeight();

        int sLine = m_selStartLine, sCol = m_selStartCol;
        int eLine = m_selEndLine, eCol = m_selEndCol;

        if (sLine > eLine || (sLine == eLine && sCol > eCol))
        {
            std::swap(sLine, eLine);
            std::swap(sCol, eCol);
        }

        for (int l = sLine; l <= eLine && l < GetLineCount(); l++)
        {
            float x1 = pos.x + m_padding.left;
            float x2 = pos.x + m_padding.left;

            if (l == sLine)
                x1 += GetXFromColumn(m_lines[l].text, sCol);
            else
                x1 += 0.0f;

            if (l == eLine)
                x2 += GetXFromColumn(m_lines[l].text, eCol);
            else
                x2 += m_lines[l].width;

            float y = pos.y + m_padding.top + l * lineH - m_scrollY;
            float selW = std::max(x2 - x1, 0.0f);
            if (selW > 0.0f)
            {
                Renderer2D::DrawQuad({ x1 + selW * 0.5f, y + lineH * 0.5f }, { selW, lineH }, m_SelectionColor);
            }
        }
    }

    void UITextEdit::DrawTextLines()
    {
        Vector2 pos = GetComputedPosition();
        float lineH = GetLineHeight();
        float scale = (float)m_FontSize / 48.0f;

        float textY = pos.y + m_padding.top + m_FontSize * 0.85f - m_scrollY;

        if (m_lines.empty() || (m_lines.size() == 1 && m_lines[0].text.empty() && !m_focused && !m_placeholderText.empty()))
        {
            if (!m_placeholderText.empty())
            {
                std::wstring wp(m_placeholderText.begin(), m_placeholderText.end());
                Renderer2D::RenderDrawText(wp, m_font->GetAtlasTexture(), m_font->GetGlyphs(),
                    pos.x + m_padding.left, textY, m_FontSize, m_PlaceholderColor);
            }
            return;
        }

        for (int i = 0; i < GetLineCount(); i++)
        {
            if (!m_lines[i].text.empty())
            {
                Renderer2D::RenderDrawText(m_lines[i].text, m_font->GetAtlasTexture(), m_font->GetGlyphs(),
                    pos.x + m_padding.left, textY, m_FontSize, m_TextColor);
            }
            textY += lineH;
        }
    }

    float UITextEdit::GetLineHeight() const
    {
        return m_FontSize * 1.3f;
    }

    float UITextEdit::GetContentHeight() const
    {
        return GetLineCount() * GetLineHeight();
    }

    void UITextEdit::EnsureCursorVisible()
    {
        Vector2 size = GetComputedSize();
        float viewH = size.y;
        float contentH = GetContentHeight();

        float cursorY = m_cursorLine * GetLineHeight();

        if (cursorY < m_scrollY)
            m_scrollY = cursorY;
        else if (cursorY + GetLineHeight() > m_scrollY + viewH)
            m_scrollY = cursorY + GetLineHeight() - viewH;

        ClampScroll();
    }

    void UITextEdit::ClampScroll()
    {
        float contentH = GetContentHeight();
        float viewH = GetComputedSize().y;
        m_scrollY = std::max(0.0f, std::min(m_scrollY, std::max(0.0f, contentH - viewH)));
        m_showVertical = contentH > viewH;
    }

    void UITextEdit::DeleteSelection()
    {
        if (!HasSelection()) return;
        if (m_lines.empty()) return;

        int sLine = m_selStartLine, sCol = m_selStartCol;
        int eLine = m_selEndLine, eCol = m_selEndCol;
        if (sLine > eLine || (sLine == eLine && sCol > eCol))
        {
            std::swap(sLine, eLine);
            std::swap(sCol, eCol);
        }

        int flatStart = 0, flatEnd = 0, pos = 0;
        for (int l = 0; l < GetLineCount(); l++)
        {
            int lineLen = (int)m_lines[l].text.length();
            if (l == sLine) flatStart = pos + sCol;
            if (l == eLine) flatEnd = pos + eCol;
            pos += lineLen + 1;
        }

        if (flatStart > flatEnd) std::swap(flatStart, flatEnd);
        if (flatStart >= (int)m_wtext.length()) return;
        if (flatEnd > (int)m_wtext.length()) flatEnd = (int)m_wtext.length();

        m_wtext.erase(flatStart, flatEnd - flatStart);
        m_text.assign(m_wtext.begin(), m_wtext.end());
        ClearSelection();

        WrapLines();

        int newPos = flatStart;
        int charCount = 0;
        for (int l = 0; l < GetLineCount(); l++)
        {
            int lineLen = (int)m_lines[l].text.length();
            if (charCount + lineLen >= newPos)
            {
                m_cursorLine = l;
                m_cursorColumn = newPos - charCount;
                if (m_cursorColumn > lineLen) m_cursorColumn = lineLen;
                break;
            }
            charCount += lineLen + 1;
        }

        m_desiredColumnX = -1;
        m_onTextChanged.Broadcast();
        EnsureCursorVisible();
    }

    void UITextEdit::InsertAtCursor(uint32_t codepoint)
    {
        if (HasSelection()) DeleteSelection();
        if (m_lines.empty()) return;

        int flatPos = 0;
        for (int l = 0; l < m_cursorLine; l++)
            flatPos += (int)m_lines[l].text.length() + 1;
        flatPos += m_cursorColumn;

        wchar_t wc = (wchar_t)codepoint;
        m_wtext.insert(flatPos, 1, wc);
        m_text.assign(m_wtext.begin(), m_wtext.end());

        WrapLines();

        int newPos = flatPos + 1;
        int charCount = 0;
        for (int l = 0; l < GetLineCount(); l++)
        {
            int lineLen = (int)m_lines[l].text.length();
            if (charCount + lineLen >= newPos)
            {
                m_cursorLine = l;
                m_cursorColumn = newPos - charCount;
                if (m_cursorColumn > lineLen) m_cursorColumn = lineLen;
                break;
            }
            charCount += lineLen + 1;
        }

        m_desiredColumnX = -1;
        ClearSelection();
        m_onTextChanged.Broadcast();
        EnsureCursorVisible();
    }

    void UITextEdit::Backspace()
    {
        if (HasSelection()) { DeleteSelection(); return; }
        if (m_lines.empty()) return;

        int flatPos = 0;
        for (int l = 0; l < m_cursorLine; l++)
            flatPos += (int)m_lines[l].text.length() + 1;
        flatPos += m_cursorColumn;

        if (flatPos <= 0) return;

        m_wtext.erase(flatPos - 1, 1);
        m_text.assign(m_wtext.begin(), m_wtext.end());

        WrapLines();

        int newPos = flatPos - 1;
        int charCount = 0;
        for (int l = 0; l < GetLineCount(); l++)
        {
            int lineLen = (int)m_lines[l].text.length();
            if (charCount + lineLen >= newPos)
            {
                m_cursorLine = l;
                m_cursorColumn = newPos - charCount;
                if (m_cursorColumn > lineLen) m_cursorColumn = lineLen;
                break;
            }
            charCount += lineLen + 1;
        }

        m_desiredColumnX = -1;
        m_onTextChanged.Broadcast();
        EnsureCursorVisible();
    }

    void UITextEdit::DeleteForward()
    {
        if (HasSelection()) { DeleteSelection(); return; }
        if (m_lines.empty()) return;

        int flatPos = 0;
        for (int l = 0; l < m_cursorLine; l++)
            flatPos += (int)m_lines[l].text.length() + 1;
        flatPos += m_cursorColumn;

        if (flatPos >= (int)m_wtext.length()) return;

        m_wtext.erase(flatPos, 1);
        m_text.assign(m_wtext.begin(), m_wtext.end());

        WrapLines();

        m_onTextChanged.Broadcast();
        EnsureCursorVisible();
    }

    void UITextEdit::MoveCursorLeft(bool shift)
    {
        if (m_lines.empty()) return;

        if (shift)
        {
            if (m_selStartLine < 0)
            {
                m_selStartLine = m_cursorLine;
                m_selStartCol = m_cursorColumn;
            }
            if (m_cursorColumn > 0) m_cursorColumn--;
            else if (m_cursorLine > 0)
            {
                m_cursorLine--;
                m_cursorColumn = (int)m_lines[m_cursorLine].text.length();
            }
            m_selEndLine = m_cursorLine;
            m_selEndCol = m_cursorColumn;
        }
        else
        {
            if (HasSelection())
            {
                int sLine = m_selStartLine, sCol = m_selStartCol;
                int eLine = m_selEndLine, eCol = m_selEndCol;
                if (sLine > eLine || (sLine == eLine && sCol > eCol))
                {
                    std::swap(sLine, eLine);
                    std::swap(sCol, eCol);
                }
                m_cursorLine = sLine;
                m_cursorColumn = sCol;
                ClearSelection();
            }
            else if (m_cursorColumn > 0) m_cursorColumn--;
            else if (m_cursorLine > 0)
            {
                m_cursorLine--;
                m_cursorColumn = (int)m_lines[m_cursorLine].text.length();
            }
        }
        m_desiredColumnX = -1;
        m_cursorBlinkTime = 0.0f;
        m_cursorVisible = true;
        EnsureCursorVisible();
    }

    void UITextEdit::MoveCursorRight(bool shift)
    {
        if (m_lines.empty()) return;

        if (shift)
        {
            if (m_selStartLine < 0)
            {
                m_selStartLine = m_cursorLine;
                m_selStartCol = m_cursorColumn;
            }
            if (m_cursorColumn < (int)m_lines[m_cursorLine].text.length()) m_cursorColumn++;
            else if (m_cursorLine < GetLineCount() - 1)
            {
                m_cursorLine++;
                m_cursorColumn = 0;
            }
            m_selEndLine = m_cursorLine;
            m_selEndCol = m_cursorColumn;
        }
        else
        {
            if (HasSelection())
            {
                int sLine = m_selStartLine, sCol = m_selStartCol;
                int eLine = m_selEndLine, eCol = m_selEndCol;
                if (sLine > eLine || (sLine == eLine && sCol > eCol))
                {
                    std::swap(sLine, eLine);
                    std::swap(sCol, eCol);
                }
                m_cursorLine = eLine;
                m_cursorColumn = eCol;
                ClearSelection();
            }
            else if (m_cursorColumn < (int)m_lines[m_cursorLine].text.length()) m_cursorColumn++;
            else if (m_cursorLine < GetLineCount() - 1)
            {
                m_cursorLine++;
                m_cursorColumn = 0;
            }
        }
        m_desiredColumnX = -1;
        m_cursorBlinkTime = 0.0f;
        m_cursorVisible = true;
        EnsureCursorVisible();
    }

    void UITextEdit::MoveCursorUp(bool shift)
    {
        if (m_lines.empty() || m_cursorLine <= 0) return;

        if (m_desiredColumnX < 0)
            m_desiredColumnX = (int)GetXFromColumn(m_lines[m_cursorLine].text, m_cursorColumn);

        MoveCursorToLineCol(m_cursorLine - 1, 0, true);
        m_cursorColumn = GetColumnFromX(m_lines[m_cursorLine].text, (float)m_desiredColumnX);
        if (m_cursorColumn > (int)m_lines[m_cursorLine].text.length())
            m_cursorColumn = (int)m_lines[m_cursorLine].text.length();

        if (!shift) ClearSelection();

        m_cursorBlinkTime = 0.0f;
        m_cursorVisible = true;
        EnsureCursorVisible();
    }

    void UITextEdit::MoveCursorDown(bool shift)
    {
        if (m_lines.empty() || m_cursorLine >= GetLineCount() - 1) return;

        if (m_desiredColumnX < 0)
            m_desiredColumnX = (int)GetXFromColumn(m_lines[m_cursorLine].text, m_cursorColumn);

        MoveCursorToLineCol(m_cursorLine + 1, 0, true);
        m_cursorColumn = GetColumnFromX(m_lines[m_cursorLine].text, (float)m_desiredColumnX);
        if (m_cursorColumn > (int)m_lines[m_cursorLine].text.length())
            m_cursorColumn = (int)m_lines[m_cursorLine].text.length();

        if (!shift) ClearSelection();

        m_cursorBlinkTime = 0.0f;
        m_cursorVisible = true;
        EnsureCursorVisible();
    }

    void UITextEdit::MoveCursorHome(bool shift)
    {
        if (m_lines.empty()) return;

        if (shift)
        {
            if (m_selStartLine < 0)
            {
                m_selStartLine = m_cursorLine;
                m_selStartCol = m_cursorColumn;
            }
            m_cursorColumn = 0;
            m_selEndLine = m_cursorLine;
            m_selEndCol = m_cursorColumn;
        }
        else
        {
            ClearSelection();
            m_cursorColumn = 0;
        }
        m_desiredColumnX = -1;
        m_cursorBlinkTime = 0.0f;
        m_cursorVisible = true;
        EnsureCursorVisible();
    }

    void UITextEdit::MoveCursorEnd(bool shift)
    {
        if (m_lines.empty()) return;

        if (shift)
        {
            if (m_selStartLine < 0)
            {
                m_selStartLine = m_cursorLine;
                m_selStartCol = m_cursorColumn;
            }
            m_cursorColumn = (int)m_lines[m_cursorLine].text.length();
            m_selEndLine = m_cursorLine;
            m_selEndCol = m_cursorColumn;
        }
        else
        {
            ClearSelection();
            m_cursorColumn = (int)m_lines[m_cursorLine].text.length();
        }
        m_desiredColumnX = -1;
        m_cursorBlinkTime = 0.0f;
        m_cursorVisible = true;
        EnsureCursorVisible();
    }

    void UITextEdit::MoveCursorPageUp(bool shift)
    {
        if (m_lines.empty() || m_cursorLine <= 0) return;

        Vector2 size = GetComputedSize();
        int linesPerPage = std::max(1, (int)(size.y / GetLineHeight()));
        int newLine = std::max(0, m_cursorLine - linesPerPage);

        if (m_desiredColumnX < 0)
            m_desiredColumnX = (int)GetXFromColumn(m_lines[m_cursorLine].text, m_cursorColumn);

        MoveCursorToLineCol(newLine, 0, true);
        m_cursorColumn = GetColumnFromX(m_lines[m_cursorLine].text, (float)m_desiredColumnX);
        if (m_cursorColumn > (int)m_lines[m_cursorLine].text.length())
            m_cursorColumn = (int)m_lines[m_cursorLine].text.length();

        if (!shift) ClearSelection();
        m_scrollY = std::max(0.0f, m_scrollY - size.y);
        m_cursorBlinkTime = 0.0f;
        m_cursorVisible = true;
        EnsureCursorVisible();
    }

    void UITextEdit::MoveCursorPageDown(bool shift)
    {
        if (m_lines.empty() || m_cursorLine >= GetLineCount() - 1) return;

        Vector2 size = GetComputedSize();
        int linesPerPage = std::max(1, (int)(size.y / GetLineHeight()));
        int newLine = std::min(GetLineCount() - 1, m_cursorLine + linesPerPage);

        if (m_desiredColumnX < 0)
            m_desiredColumnX = (int)GetXFromColumn(m_lines[m_cursorLine].text, m_cursorColumn);

        MoveCursorToLineCol(newLine, 0, true);
        m_cursorColumn = GetColumnFromX(m_lines[m_cursorLine].text, (float)m_desiredColumnX);
        if (m_cursorColumn > (int)m_lines[m_cursorLine].text.length())
            m_cursorColumn = (int)m_lines[m_cursorLine].text.length();

        if (!shift) ClearSelection();
        m_scrollY = std::min(GetContentHeight(), m_scrollY + size.y);
        m_cursorBlinkTime = 0.0f;
        m_cursorVisible = true;
        EnsureCursorVisible();
    }

    void UITextEdit::MoveCursorToLineCol(int line, int col, bool shift)
    {
        if (shift && m_selStartLine < 0)
        {
            m_selStartLine = m_cursorLine;
            m_selStartCol = m_cursorColumn;
        }

        m_cursorLine = std::max(0, std::min(line, GetLineCount() - 1));
        m_cursorColumn = std::max(0, std::min(col, (int)m_lines[m_cursorLine].text.length()));

        if (shift)
        {
            m_selEndLine = m_cursorLine;
            m_selEndCol = m_cursorColumn;
        }
    }

    int UITextEdit::GetColumnFromX(const std::wstring& lineText, float x)
    {
        float scale = (float)m_FontSize / 48.0f;
        float curX = 0.0f;
        for (int i = 0; i < (int)lineText.length(); i++)
        {
            auto it = m_font->GetGlyphs().find((uint32_t)lineText[i]);
            float adv = 0.0f;
            if (it != m_font->GetGlyphs().end())
                adv = (it->second.advance >> 6) * scale;
            else
                adv = 10.0f * scale;

            if (x < curX + adv * 0.5f)
                return i;
            curX += adv;
        }
        return (int)lineText.length();
    }

    float UITextEdit::GetXFromColumn(const std::wstring& lineText, int col)
    {
        float scale = (float)m_FontSize / 48.0f;
        float x = 0.0f;
        int end = std::min(col, (int)lineText.length());
        for (int i = 0; i < end; i++)
        {
            auto it = m_font->GetGlyphs().find((uint32_t)lineText[i]);
            if (it != m_font->GetGlyphs().end())
                x += (it->second.advance >> 6) * scale;
            else
                x += 10.0f * scale;
        }
        return x;
    }

    int UITextEdit::GetLineFromY(float y)
    {
        float lineH = GetLineHeight();
        int line = (int)(y / lineH);
        return std::max(0, std::min(line, GetLineCount() - 1));
    }

    void UITextEdit::CopySelection()
    {
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

    void UITextEdit::CutSelection()
    {
        CopySelection();
        DeleteSelection();
    }

    void UITextEdit::PasteText()
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
                    uint32_t codepoint = (unsigned char)c;
                    if (codepoint == '\n' || codepoint == '\r')
                    {
                        InsertAtCursor(L'\n');
                    }
                    else if (codepoint >= 32 && codepoint != 127)
                    {
                        InsertAtCursor(codepoint);
                    }
                }
            }
        }
        CloseClipboard();
    }

    void UITextEdit::ShowContextMenu()
    {
        Vector2 mousePos = InputSystem::GetMousePosition();
        m_contextMenuPos = mousePos;

        int windowW = Application::Get().GetWindow()->GetWidth();
        int windowH = Application::Get().GetWindow()->GetHeight();
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

    void UITextEdit::HideContextMenu()
    {
        m_contextMenuVisible = false;
        m_contextMenuHoverIndex = -1;
    }

    void UITextEdit::DrawContextMenu()
    {
        if (!m_contextMenuVisible) return;

        float menuW = m_contextMenuWidth;
        float itemH = m_contextMenuItemHeight;
        int itemCount = (int)ContextMenuAction::Count;
        float menuH = itemCount * itemH + 4.0f;

        Vector2 bgPos = m_contextMenuPos + Vector2(menuW * 0.5f, menuH * 0.5f);
        Renderer2D::DrawQuad(bgPos, { menuW, menuH }, TColor(45, 45, 48, 240));
        Renderer2D::DrawRect({ bgPos.x, bgPos.y, 0.0f }, { menuW, menuH }, TColor(100, 100, 100, 255));

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
    }

    void UITextEdit::HandleContextMenuInput()
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
            HideContextMenu();
        }

        if (InputSystem::IsKeyJustPressed(InputKey::Escape))
        {
            HideContextMenu();
        }
    }

    int UITextEdit::GetFlatIndexFromLineCol(int line, int col) const
    {
        int flat = 0;
        for (int l = 0; l < line; l++)
            flat += (int)m_lines[l].text.length() + 1;
        flat += col;
        return flat;
    }

    void UITextEdit::GetLineColFromFlatIndex(int flatIndex, int& outLine, int& outCol) const
    {
        int charCount = 0;
        for (int l = 0; l < GetLineCount(); l++)
        {
            int lineLen = (int)m_lines[l].text.length();
            if (charCount + lineLen >= flatIndex)
            {
                outLine = l;
                outCol = flatIndex - charCount;
                if (outCol > lineLen) outCol = lineLen;
                return;
            }
            charCount += lineLen + 1;
        }
        outLine = std::max(0, GetLineCount() - 1);
        outCol = 0;
        if (!m_lines.empty())
            outCol = (int)m_lines[outLine].text.length();
    }

    void UITextEdit::SelectWordAtCursor()
    {
        if (m_wtext.empty()) return;

        int flatPos = GetFlatIndexFromLineCol(m_cursorLine, m_cursorColumn);
        if (flatPos > (int)m_wtext.length()) flatPos = (int)m_wtext.length();

        auto isWordChar = [](wchar_t ch) -> bool {
            return ch != L' ' && ch != L'\t' && ch != L'\n' && ch != L'\r' &&
                   ch != L'.' && ch != L',' && ch != L';' && ch != L':' &&
                   ch != L'!' && ch != L'?' && ch != L'(' && ch != L')' &&
                   ch != L'[' && ch != L']' && ch != L'{' && ch != L'}' &&
                   ch != L'\"' && ch != L'\'' && ch != L'-' && ch != L'_';
        };

        int start = flatPos;
        while (start > 0 && isWordChar(m_wtext[start - 1]))
            start--;

        int end = flatPos;
        while (end < (int)m_wtext.length() && isWordChar(m_wtext[end]))
            end++;

        if (start < end)
        {
            GetLineColFromFlatIndex(start, m_selStartLine, m_selStartCol);
            GetLineColFromFlatIndex(end, m_selEndLine, m_selEndCol);
            m_cursorLine = m_selEndLine;
            m_cursorColumn = m_selEndCol;
            m_desiredColumnX = -1;
            m_cursorBlinkTime = 0.0f;
            m_cursorVisible = true;
            EnsureCursorVisible();
        }
    }

    void UITextEdit::HandleMouseClick(const Vector2& mousePos)
    {
        Focus();

        Vector2 pos = GetComputedPosition();
        Vector2 size = GetComputedSize();

        float localY = mousePos.y - pos.y - m_padding.top + m_scrollY;
        int line = GetLineFromY(localY);
        if (line >= GetLineCount()) line = std::max(0, GetLineCount() - 1);
        if (line < 0) line = 0;
        float localX = mousePos.x - pos.x - m_padding.left;
        int col = 0;
        if (!m_lines.empty() && line < (int)m_lines.size())
            col = GetColumnFromX(m_lines[line].text, localX);

        float dx = mousePos.x - m_lastClickPos.x;
        float dy = mousePos.y - m_lastClickPos.y;
        bool samePos = (dx * dx + dy * dy) < 25.0f;

        if (m_timeSinceLastClick < 0.3f && samePos)
        {
            m_clickCount++;
            if (m_clickCount == 2)
            {
                m_cursorLine = line;
                m_cursorColumn = col;
                SelectWordAtCursor();
                m_timeSinceLastClick = 0.0f;
                m_lastClickPos = mousePos;
                return;
            }
            else if (m_clickCount >= 3)
            {
                SelectAll();
                m_cursorLine = GetLineCount() - 1;
                if (!m_lines.empty())
                    m_cursorColumn = (int)m_lines.back().text.length();
                else
                    m_cursorColumn = 0;
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
            m_cursorLine = line;
            m_cursorColumn = col;
            ClearSelection();
        }
        else
        {
            if (m_selStartLine < 0)
            {
                m_selStartLine = m_cursorLine;
                m_selStartCol = m_cursorColumn;
            }
            m_cursorLine = line;
            m_cursorColumn = col;
            m_selEndLine = line;
            m_selEndCol = col;
        }

        m_desiredColumnX = -1;
        m_lastClickPos = mousePos;
        m_timeSinceLastClick = 0.0f;
        m_cursorBlinkTime = 0.0f;
        m_cursorVisible = true;
        EnsureCursorVisible();
    }
}
*/