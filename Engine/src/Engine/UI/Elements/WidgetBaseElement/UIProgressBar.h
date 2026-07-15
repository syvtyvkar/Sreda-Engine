
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
    class UIProgressBar : public UIWidget
    {
    public:
        UIProgressBar();
        void OnInit() override;
        void OnUpdate(float deltaTime) override;
        
        virtual void OnSelfUICollectCommand(UICommandList& InCmd) override;

        float GetPercent() const {return m_Percent;}
        void SetPercent(float InVal) {m_Percent = InVal;}

        float GetPercentMax() const {return m_PercentMax;}
        void SetPercentMax(float InVal) {m_PercentMax = InVal;}

    private:
        float m_Percent = 0.5f;
        float m_PercentMax = 1.f;
    };
}