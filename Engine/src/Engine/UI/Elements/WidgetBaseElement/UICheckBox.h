
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
    class UICheckBox : public UIWidget
    {
    public:
        UICheckBox();
        void OnInit() override;
        void OnUpdate(float deltaTime) override;
        
        virtual void OnSelfUICollectCommand(UICommandList& InCmd) override;

        bool GetCheckOut() const {return m_CheckOut;}
        void SetCheckOut(bool InVal) {m_CheckOut = InVal;}
        
        void CallOnClick();

    private:
        bool m_CheckOut = false;
    };
}