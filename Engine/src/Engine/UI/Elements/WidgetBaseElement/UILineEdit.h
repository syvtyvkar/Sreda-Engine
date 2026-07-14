#pragma once

#include "Engine/UI/Framework/UIWidget.h"
#include "Engine/Render/Font/Font.h"

namespace Engine::UI
{
    ADD_MULTICAST_DELEGATE(DOnUILineEditTextChanged);
    ADD_MULTICAST_DELEGATE_ONE_PARAM(DOnUILineEditTextCommitted, const std::string&);

    class UILineEdit : public UIWidget
    {
    public:
        UILineEdit(const std::string& InFontName);

        void OnRender() override;
        void OnUpdate(float deltaTime) override;

        void OnCallCharInput(uint32_t codepoint);
        void OnCallClick();

        virtual void OnSelfUICollectCommand(UICommandList& InCmd) override;

        void SetText(const std::string& text);
        std::string GetText() const { return m_text; }

        void SetFontSize(int size);
        int GetFontSize() const { return m_FontSize; }

        void SetPlaceholderText(const std::string& text) { m_placeholderText = text; }
        std::string GetPlaceholderText() const { return m_placeholderText; }

        void Focus();
        void Blur();

        void SetCursorPosition(int pos);
        int GetCursorPosition() const { return m_cursorPos; }

        void SelectAll();
        void ClearSelection();
        bool HasSelection() const { return m_selectionStart >= 0 && m_selectionEnd >= 0 && m_selectionStart != m_selectionEnd; }
        std::string GetSelectedText() const;

        DOnUILineEditTextChanged& OnTextChanged() { return m_onTextChanged; }
        DOnUILineEditTextCommitted& OnTextCommitted() { return m_onTextCommitted; }

    private:
        void UpdateTextInternal();
        void HandleKeyInput();
        void HandleCharInput(uint32_t codepoint);
        void UpdateCursorBlink(float dt);
        void DrawControl(UICommandList &InCmd);
        void DrawCursor(UICommandList &InCmd);
        void DrawSelection(UICommandList &InCmd);
        void DrawTextContent(UICommandList &InCmd);
        int GetGlyphIndexAtX(float x);
        float GetCursorXPosition() const;
        void EnsureCursorVisible();
        void DeleteSelection();
        void InsertAtCursor(uint32_t codepoint);
        void Backspace();
        void DeleteForward();
        void MoveCursorLeft(bool shift);
        void MoveCursorRight(bool shift);
        void MoveCursorHome(bool shift);
        void MoveCursorEnd(bool shift);
        void CopySelection();
        void CutSelection();
        void PasteText();

        void ShowContextMenu();
        void SelectWordAtCursor();
        void HandleMouseClick(const Vector2& mousePos);

        std::string m_text;
        std::wstring m_wtext;
        std::string m_FontName;
        std::string m_placeholderText;
        int m_FontSize = 16;
        TRef<Engine::Render::Font> m_font;

        TColor m_TextColor = TColor(220, 220, 220, 255);
        TColor m_PlaceholderColor = TColor(120, 120, 120, 255);
        TColor m_SelectionColor = TColor(60, 120, 210, 180);
        TColor m_CursorColor = TColor(200, 200, 200, 255);
        TColor m_BackgroundColor = TColor(25, 25, 25, 255);
        TColor m_BorderColor = TColor(70, 70, 70, 255);
        TColor m_FocusBorderColor = TColor(80, 140, 220, 255);

        int m_cursorPos = 0;
        int m_selectionStart = -1;
        int m_selectionEnd = -1;
        float m_cursorBlinkTime = 0.0f;
        bool m_cursorVisible = true;
        float m_scrollOffset = 0.0f;

        Engine::DelegateHandle m_charInputHandle;
        DOnUILineEditTextChanged m_onTextChanged;
        DOnUILineEditTextCommitted m_onTextCommitted;

        bool m_draggingSelection = false;
        bool m_wasDrag = false;

        float m_timeSinceLastClick = 0.0f;
        Vector2 m_dragStartPos;
        Vector2 m_lastClickPos;
        int m_clickCount = 0;
        Engine::DelegateHandle m_clickHandle;
    };
}
