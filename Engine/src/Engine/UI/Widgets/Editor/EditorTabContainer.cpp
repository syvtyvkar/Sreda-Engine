#include "EditorTabContainer.h"
#include "Engine/Render/Renderer2D.h"
#include "Engine/Render/Font/Font.h"
#include "Engine/Input/Input.h"
#include <algorithm>

namespace Engine::UI
{
    using namespace Engine::Render;

    static float ComputeTabWidth(const std::string& text)
    {
        float minWidth = 80.0f;
        float textWidth = (float)text.length() * 8.0f;
        return std::max(minWidth, textWidth + 40.0f);
    }

    EditorTabContainer::EditorTabContainer()
    {
        SetSize({ 800, 600 });
        GetStyle().backgroundColor = TColor(35, 35, 35, 255);
        GetStyle().borderWidth = 0.0f;
    }

    void EditorTabContainer::OnInit()
    {
        m_tabBarWidget = CreateRef<UIWidget>();
        m_tabBarWidget->SetSize({ GetSize().x, EditorTabHeader::TAB_HEIGHT });
        m_tabBarWidget->SetPosition({ 0, 0 });
        m_tabBarWidget->GetStyle().backgroundColor = TColor(50, 50, 50, 255);
        m_tabBarWidget->GetStyle().borderWidth = 0.0f;
        AddChild(m_tabBarWidget);

        m_contentArea = CreateRef<UIWidget>();
        m_contentArea->SetPosition({ 0, EditorTabHeader::TAB_HEIGHT + 1.0f });
        m_contentArea->SetSize({ GetSize().x, GetSize().y - EditorTabHeader::TAB_HEIGHT - 1.0f });
        m_contentArea->SetHorizontalAlignment(UIHorizontalAlignment::Stretch);
        m_contentArea->SetVerticalAlignment(UIVerticalAlignment::Stretch);
        m_contentArea->GetStyle().backgroundColor = TColor(35, 35, 35, 255);
        m_contentArea->GetStyle().borderWidth = 0.0f;
        AddChild(m_contentArea);

        __super::OnInit();

        m_tabBarWidget->OnInit();
        m_contentArea->OnInit();
    }

    int EditorTabContainer::AddTab(const std::string& name, TRef<UIElement> content, bool canClose)
    {
        TabItem tab;
        tab.name = name;
        tab.content = content;
        tab.canClose = canClose;

        auto header = CreateRef<EditorTabHeader>(name);
        int index = (int)m_tabs.size();
        tab.header = header;
        header->SetTabIndex(index);
        header->SetSize({ ComputeTabWidth(name), EditorTabHeader::TAB_HEIGHT });
        header->OnInit();

        header->OnTabSelect().Subscribe([this](int tabIndex)
        {
            SelectTab(tabIndex);
        });
        header->OnTabClose().Subscribe([this](int tabIndex)
        {
            RemoveTab(tabIndex);
        });

        m_tabs.push_back(tab);

        RebuildTabBar();

        if (m_tabs.size() == 1)
        {
            SelectTab(0);
        }

        return index;
    }

    void EditorTabContainer::RemoveTab(int index)
    {
        if (index < 0 || index >= (int)m_tabs.size()) return;

        if (m_activeTabIndex == index)
        {
            if (m_tabs[index].content && m_tabs[index].content->GetParent() == m_contentArea)
            {
                m_contentArea->RemoveChild(m_tabs[index].content);
            }
        }

        m_tabs.erase(m_tabs.begin() + index);

        for (int i = 0; i < (int)m_tabs.size(); i++)
        {
            m_tabs[i].header->SetTabIndex(i);
        }

        RebuildTabBar();

        if (m_tabs.empty())
        {
            m_activeTabIndex = -1;
            return;
        }

        if (m_activeTabIndex >= (int)m_tabs.size())
            SelectTab((int)m_tabs.size() - 1);
        else if (index < m_activeTabIndex)
            SelectTab(m_activeTabIndex - 1);
        else
            SelectTab(m_activeTabIndex);
    }

    void EditorTabContainer::SelectTab(int index)
    {
        if (index < 0 || index >= (int)m_tabs.size()) return;

        if (m_activeTabIndex >= 0 && m_activeTabIndex < (int)m_tabs.size())
        {
            auto& prevTab = m_tabs[m_activeTabIndex];
            prevTab.header->SetActive(false);
            if (prevTab.content && prevTab.content->GetParent() == m_contentArea)
            {
                m_contentArea->RemoveChild(prevTab.content);
            }
        }

        m_activeTabIndex = index;

        auto& newTab = m_tabs[index];
        newTab.header->SetActive(true);
        if (newTab.content)
        {
            m_contentArea->AddChild(newTab.content);
        }
    }

    void EditorTabContainer::RebuildTabBar()
    {
        auto childrenCopy = m_tabBarWidget->GetChildren();
        for (auto& child : childrenCopy)
        {
            m_tabBarWidget->RemoveChild(child);
        }

        for (auto& tab : m_tabs)
        {
            tab.header->SetPosition({ 0, 0 });
            m_tabBarWidget->AddChild(tab.header);
        }
    }

    void EditorTabContainer::OnRender()
    {
        if (!IsVisible()) return;

        Vector2 pos = GetComputedPosition();
        Vector2 size = GetComputedSize();

        Renderer2D::DrawQuad(pos + size * 0.5f, size, TColor(35, 35, 35, 255));
        Renderer2D::DrawQuad(pos + Vector2(size.x * 0.5f, EditorTabHeader::TAB_HEIGHT * 0.5f),
            Vector2(size.x, EditorTabHeader::TAB_HEIGHT + 1.0f),
            TColor(50, 50, 50, 255));

        //Renderer2D::DrawRect(
        //    Vector3(pos.x + size.x * 0.5f, pos.y + EditorTabHeader::TAB_HEIGHT, 0.0f),
        //    Vector2(size.x, 1.0f),
        //    TColor(40, 40, 40, 255));

        UIElement::OnRender();
    }

    void EditorTabContainer::OnUpdate(float deltaTime)
    {
        if (!IsVisible()) return;

        Vector2 containerSize = GetComputedSize();
        float barWidth = containerSize.x;

        float x = 0;
        for (auto& tab : m_tabs)
        {
            tab.header->SetPosition({ x, 0 });
            x += tab.header->GetSize().x;
        }

        m_tabBarWidget->SetSize({ barWidth, EditorTabHeader::TAB_HEIGHT });

        m_contentArea->SetPosition({ 0, EditorTabHeader::TAB_HEIGHT + 1.0f });
        m_contentArea->SetSize({ containerSize.x, containerSize.y - EditorTabHeader::TAB_HEIGHT - 1.0f });

        Vector2 containerPos = GetComputedPosition();

        bool mousePressed = InputSystem::IsMouseButtonPressed(InputKey::MouseLeft);
        bool mouseJustPressed = mousePressed && !m_prevMousePressed;
        bool mouseJustReleased = !mousePressed && m_prevMousePressed;
        m_prevMousePressed = mousePressed;

        if (mouseJustPressed)
        {
            Vector2 mousePos = InputSystem::GetMousePosition();

            if (mousePos.x >= containerPos.x && mousePos.x <= containerPos.x + barWidth &&
                mousePos.y >= containerPos.y && mousePos.y <= containerPos.y + EditorTabHeader::TAB_HEIGHT)
            {
                float relX = mousePos.x - containerPos.x;
                float accumulated = 0;
                for (int i = 0; i < (int)m_tabs.size(); i++)
                {
                    float tabW = m_tabs[i].header->GetSize().x;
                    if (relX >= accumulated && relX < accumulated + tabW)
                    {
                        m_dragTabIndex = i;
                        m_isDragging = false;
                        break;
                    }
                    accumulated += tabW;
                }
            }
        }

        if (mouseJustReleased)
        {
            if (m_isDragging && m_dragTabIndex >= 0)
            {
                Vector2 mousePos = InputSystem::GetMousePosition();
                float relX = mousePos.x - containerPos.x;

                float accumulated = 0;
                int targetIndex = (int)m_tabs.size() - 1;
                for (int i = 0; i < (int)m_tabs.size(); i++)
                {
                    float tabW = m_tabs[i].header->GetSize().x;
                    if (relX < accumulated + tabW * 0.5f)
                    {
                        targetIndex = i;
                        break;
                    }
                    accumulated += tabW;
                }

                if (targetIndex != m_dragTabIndex)
                {
                    auto item = m_tabs[m_dragTabIndex];
                    m_tabs.erase(m_tabs.begin() + m_dragTabIndex);

                    if (targetIndex > m_dragTabIndex)
                        targetIndex--;

                    targetIndex = std::min(targetIndex, (int)m_tabs.size());
                    m_tabs.insert(m_tabs.begin() + targetIndex, item);

                    for (int i = 0; i < (int)m_tabs.size(); i++)
                    {
                        m_tabs[i].header->SetTabIndex(i);
                    }

                    RebuildTabBar();

                    if (m_activeTabIndex == m_dragTabIndex)
                        m_activeTabIndex = targetIndex;
                }
            }
            m_dragTabIndex = -1;
            m_isDragging = false;
        }

        if (m_dragTabIndex >= 0 && mousePressed)
        {
            m_isDragging = true;
        }

        UIWidget::OnUpdate(deltaTime);
    }
}
