#include <glad/glad.h>

#include "Element.h"

#include <iostream>
#include <vector>
#include <functional>


#include <iostream>
#include <fstream>
#include <sstream>
#include "Engine/Core/Log.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Engine/Render/Render.h"
#include "Engine/Core/Utilities/Types.h"


namespace Engine::UI 
{
    UIButton::UIButton(const std::string &text)
    {
        m_text=text;
    }

    void UIButton::OnRender()
    {
        if (!IsVisible()) return;
        
        UIElement::OnRender();
    }

    void UIButton::OnUpdate(float deltaTime)
    {
        if (!IsVisible()) return;
        UIElement::OnUpdate(deltaTime);
    }

    UITextBlock::UITextBlock(const std::string &text)
    {
         m_text=text;
    }

    void UITextBlock::OnRender()
    {
        if (!IsVisible()) return;
        
        UIElement::OnRender();
    }
    
    void UITextBlock::OnUpdate(float deltaTime)
    {
        if (!IsVisible()) return;
        UIElement::OnUpdate(deltaTime);
    }
}