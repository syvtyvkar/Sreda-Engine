// (c) Nikita Rogalev. All rights reserved.

#pragma once    // Multiple inclusion guard

/*#include <unordered_map>
#include <functional>
#include <string>*/
#include "Engine/Core/Utilities/Types.h"
#include "Engine/Core/Utilities/Event.h"

namespace Engine::UI 
{
    enum UIVisibleMode
    {
        Visible,
        VisibleNoHit,
        Hidden,
        Collapse
    };

    struct UIMargins
    {
        float left = 0.0f;
        float top = 0.0f;
        float right = 0.0f;
        float bottom = 0.0f;
    };

    struct UIPadding
    {
        UIPadding() : left(0.f), top(0.f), right(0.f), bottom(0.f) {};
        UIPadding(float InLeft, float InTop,float InRight,float InBottom) : left(InLeft), top(InTop), right(InRight), bottom(InBottom) {};
        float left = 0.0f;
        float top = 0.0f;
        float right = 0.0f;
        float bottom = 0.0f;
    };

    enum class UIHorizontalAlignment
    {
        Left,
        Center,
        Right,
        Stretch
    };

    enum class UIVerticalAlignment
    {
        Top,
        Center,
        Bottom,
        Stretch
    };

    struct Border
    {
        float left = 0.0f;
        float top = 0.0f;
        float right = 0.0f;
        float bottom = 0.0f;
    };

    enum class ContextMenuItemType
    {
        Action,
        Submenu,
        Separator
    };

    struct ContextMenuItem
    {
        std::string Label;
        ContextMenuItemType Type = ContextMenuItemType::Action;
        std::function<void()> Callback;
        std::vector<ContextMenuItem> Children;
        bool Enabled = true;
    };

    class ContextMenuItemBuilder
    {
    public:
        using ContextMenuCallbackType = std::function<void()>;

        ContextMenuItemBuilder(std::vector<ContextMenuItem>& InItems) : m_Items(InItems) {}

        ContextMenuItemBuilder& AddItem(const std::string& InLabel, ContextMenuCallbackType InCallback, bool InEnabled = true)
        {
            ContextMenuItem item;
            item.Label = InLabel;
            item.Type = ContextMenuItemType::Action;
            item.Callback = std::move(InCallback);
            item.Enabled = InEnabled;
            m_Items.push_back(std::move(item));
            return *this;
        }

        template<typename T>
        ContextMenuItemBuilder& AddItem(const std::string& InLabel,T* object, void (T::*method)())
        {
            return AddItem(InLabel,[object, method]() { (object->*method)(); });
        }

        template<typename T>
        ContextMenuItemBuilder& AddItem(const std::string& InLabel, const T* object, void (T::*method)() const)
        {
            return AddItem(InLabel,[object, method]() { (object->*method)(); });
        }

        ContextMenuItemBuilder& AddSubmenu(const std::string& InLabel, std::function<void(ContextMenuItemBuilder&)> InBuildFunc, bool InEnabled = true)
        {
            ContextMenuItem item;
            item.Label = InLabel;
            item.Type = ContextMenuItemType::Submenu;
            item.Enabled = InEnabled;
            ContextMenuItemBuilder subBuilder(item.Children);
            InBuildFunc(subBuilder);
            m_Items.push_back(std::move(item));
            return *this;
        }

        ContextMenuItemBuilder& AddSeparator()
        {
            ContextMenuItem item;
            item.Type = ContextMenuItemType::Separator;
            m_Items.push_back(std::move(item));
            return *this;
        }

    private:
        std::vector<ContextMenuItem>& m_Items;
    };
}
