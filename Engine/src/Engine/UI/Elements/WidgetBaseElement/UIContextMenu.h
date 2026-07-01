// (c) Nikita Rogalev. All rights reserved.

#pragma once

#include "Engine/UI/Framework/UIWidget.h"
#include "Engine/UI/Elements/BaseElement/Container.h"
#include "Engine/Render/Font/Font.h"

#include "Engine/Render/Vertex.h"
#include "Engine/Render/Shader.h"
#include "Engine/Render/Texture.h"
#include "UIButton.h"

using namespace Engine::Render;

namespace Engine::UI 
{
    class UIContextMenu : public VerticalBox
    {
    public:
        UIContextMenu();
        void OnRender() override;
        void OnUpdate(float deltaTime) override;

        DOnUIWidgetClick& AddButtonContext(std::string InNameButton);
        void RemoveButtonContext(std::string InNameButton);
        void ClearAllButton();
        
        virtual bool IsVisible() const override;
    private:
        std::unordered_map<std::string,TRef<UIButton>> m_ButtonMap;
    };
}