
// (c) Nikita Rogalev. All rights reserved.

#pragma once

#include "Engine/UI/Framework/UIWidget.h"
#include "Engine/Render/Font/Font.h"

#include "Engine/Render/Vertex.h"
#include "Engine/Render/Shader.h"
#include "Engine/Render/Texture.h"

using namespace Engine::Render;

namespace Engine::UI 
{
    class UIComboBox : public UIWidget
    {
    public:
        UIComboBox();
        void OnInit() override;
        void OnUpdate(float deltaTime) override;
        
        virtual void OnSelfUICollectCommand(UICommandList& InCmd) override;

        std::vector<std::string> GetOptions() const {return Options;}
        void SetOptions(std::vector<std::string> InVal) {Options = InVal;}

        void AddOption(std::string InVal);
        void RemoveOption(std::string InVal);

        void CallOnClick();

    private:
        std::vector<std::string> Options;
    };
}