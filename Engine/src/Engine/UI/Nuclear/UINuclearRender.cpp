#include "NuclearUIContext.h"

#include "Platform/WindowAPI/WindowGLFW/WindowGLFW.h"
#include "Engine/Core/FileSystem/FileIO.h"

#include "UINuclearRender.h"

namespace Engine::UI 
{
    void NuclearUIRenderer::BeginUI()
    {
        //nk_glfw3_new_frame(m_context->GlfwCtx);
    }
    void NuclearUIRenderer::EndUI()
    {
    }
    void NuclearUIRenderer::RenderButton(const UIButton &Element)
    {
        //nk_button_label(m_context->Ctx, Element.GetText().c_str());
    }
    void NuclearUIRenderer::RenderTextBlock(const UITextBlock &Element)
    {
        //nk_label(m_context->Ctx, Element.GetText().c_str(), NK_TEXT_LEFT);
    }
}