/*#pragma once

#include "Engine/UI/Framework/UIWidget.h"

#include "Engine/Render/Font/Font.h"
#include <vector>

namespace Engine::UI
{
    ADD_MULTICAST_DELEGATE(DOnUIEditTextChanged);
    ADD_MULTICAST_DELEGATE_ONE_PARAM(DOnUIEditTextCommitted, const std::string&);
    
    class UITextEdit : public UIWidget
    {
    public:
        UITextEdit(const std::string& InFontName);

        void OnRender() override;
        void OnUpdate(float deltaTime) override;

        void SetText(const std::string& text);
        std::string GetText() const { return m_text; }

        void SetFontSize(int size);
        int GetFontSize() const { return m_FontSize; }

        void SetPlaceholderText(const std::string& text) { m_placeholderText = text; }
        std::string GetPlaceholderText() const { return m_placeholderText; }

        void Focus();
        void Blur();

        void SelectAll();
        void ClearSelection();
        bool HasSelection() const;
        std::string GetSelectedText() const;

        DOnUIEditTextChanged& OnTextChanged() { return m_onTextChanged; }
        DOnUIEditTextCommitted& OnTextCommitted() { return m_onTextCommitted; }

    private:
        struct LineInfo
        {
            std::wstring text;
            float width = 0.0f;
        };

        enum class ContextMenuAction { Cut, Copy, Paste, Delete, SelectAll, Count };

        void UpdateTextInternal();
        void WrapLines();
        void HandleKeyInput();
        void HandleCharInput(uint32_t codepoint);
        void UpdateCursorBlink(float dt);
        void DrawBackground();
        void DrawScrollbar();
        void DrawContent();
        void DrawCursor();
        void DrawSelection();
        void DrawTextLines();

        int GetLineCount() const { return (int)m_lines.size(); }
        float GetLineHeight() const;
        float GetContentHeight() const;
        void EnsureCursorVisible();
        void ClampScroll();

        void DeleteSelection();
        void InsertAtCursor(uint32_t codepoint);
        void Backspace();
        void DeleteForward();
        void MoveCursorLeft(bool shift);
        void MoveCursorRight(bool shift);
        void MoveCursorUp(bool shift);
        void MoveCursorDown(bool shift);
        void MoveCursorHome(bool shift);
        void MoveCursorEnd(bool shift);
        void MoveCursorPageUp(bool shift);
        void MoveCursorPageDown(bool shift);
        void MoveCursorToLineCol(int line, int col, bool shift);
        int GetColumnFromX(const std::wstring& lineText, float x);
        float GetXFromColumn(const std::wstring& lineText, int col);
        int GetLineFromY(float y);
        void CopySelection();
        void CutSelection();
        void PasteText();

        void ShowContextMenu();
        void HideContextMenu();
        void DrawContextMenu();
        void HandleContextMenuInput();
        void SelectWordAtCursor();
        void HandleMouseClick(const Vector2& mousePos);
        int GetFlatIndexFromLineCol(int line, int col) const;
        void GetLineColFromFlatIndex(int flatIndex, int& outLine, int& outCol) const;

        std::string m_text;
        std::wstring m_wtext;
        std::string m_FontName;
        std::string m_placeholderText;
        int m_FontSize = 16;
        TRef<Engine::Render::Font> m_font;

        std::vector<LineInfo> m_lines;

        TColor m_TextColor = TColor(220, 220, 220, 255);
        TColor m_PlaceholderColor = TColor(120, 120, 120, 255);
        TColor m_SelectionColor = TColor(60, 120, 210, 180);
        TColor m_CursorColor = TColor(200, 200, 200, 255);
        TColor m_BackgroundColor = TColor(25, 25, 25, 255);
        TColor m_BorderColor = TColor(70, 70, 70, 255);
        TColor m_FocusBorderColor = TColor(80, 140, 220, 255);

        TColor m_scrollbarBgColor = TColor(50, 50, 50, 150);
        TColor m_scrollbarThumbColor = TColor(100, 100, 100, 180);
        TColor m_scrollbarThumbHoverColor = TColor(130, 130, 130, 180);

        int m_cursorLine = 0;
        int m_cursorColumn = 0;
        int m_desiredColumnX = -1;

        int m_selStartLine = -1;
        int m_selStartCol = -1;
        int m_selEndLine = -1;
        int m_selEndCol = -1;

        float m_cursorBlinkTime = 0.0f;
        bool m_cursorVisible = true;
        float m_scrollY = 0.0f;
        float m_scrollbarWidth = 12.0f;
        bool m_showVertical = false;
        bool m_hoveringScrollbar = false;
        bool m_draggingScrollbar = false;
        float m_dragStartScroll = 0.0f;
        Vector2 m_dragStartMouse;

        Engine::DelegateHandle m_charInputHandle;
        DOnUIEditTextChanged m_onTextChanged;
        DOnUIEditTextCommitted m_onTextCommitted;

        bool m_contextMenuVisible = false;
        Vector2 m_contextMenuPos;
        int m_contextMenuHoverIndex = -1;
        float m_contextMenuWidth = 170.0f;
        float m_contextMenuItemHeight = 26.0f;

        bool m_draggingSelection = false;
        bool m_wasDrag = false;

        float m_timeSinceLastClick = 0.0f;
        Vector2 m_dragStartPos;
        Vector2 m_lastClickPos;
        int m_clickCount = 0;
        Engine::DelegateHandle m_clickHandle;
    };
}
*/