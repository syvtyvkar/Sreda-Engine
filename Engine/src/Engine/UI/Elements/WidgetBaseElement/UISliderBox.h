
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
    class UISliderBox : public UIWidget
    {
    public:
        UISliderBox();
        void OnInit() override;
        void OnUpdate(float deltaTime) override;
        
        virtual void OnSelfUICollectCommand(UICommandList& InCmd) override;

        float GetValue() const {return m_val;}
        void SetValue(float InVal) {m_val = std::clamp(InVal,m_MinVal,m_MaxVal);}

        float GetMaxValue() const {return m_MaxVal;}
        void SetMaxValue(float InVal) {m_MaxVal = InVal;}

        float GetMinValue() const {return m_MinVal;}
        void SetMinValue(float InVal) {m_MinVal = InVal;}

        void CallOnHoverBegin();
        void CallOnHoverEnd();
        void CallOnPressBegin();
        void CallOnPressEnd();
        void CallOnMoveMouse(float x, float y);
        void CallOnClick();

    private:
        float m_val = 0.5f;
        float m_MaxVal = 1.f;
        float m_MinVal = 0.f;
        bool m_bIsChangeMode = false;
        Vector2 m_LastMousePos;
    };
}