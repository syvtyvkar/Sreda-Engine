#include "UIEAbout.h"
#include "imgui.h"

#include "Engine/Render/RenderAPIFactory.h"

void UIEAbout::OnInit()
{
    m_LogoImage = RenderAPIFactory::CreateTexture2DFromPath("/Resources/Textures/SredaLogo.png");
}

void UIEAbout::OnRender()
{
    //if (IsVisible()) return;

    ImGui::Begin("About",&m_openWindow);

    ImGui::Text("Sreda Engine Editor\n\n\n");
    if (m_LogoImage.get())
    {
        auto data = m_LogoImage.get()->GetRendererID();

        ImGui::Image((ImTextureID)(intptr_t)m_LogoImage.get()->GetRendererID(), ImVec2(100, 100));
    }
    ImGui::Separator();
    ImGui::TextWrapped("MIT License\n Copyright (c) 2020 - 2026 Viktor Chlumsky"
        "Permission is hereby granted, free of charge, to any person obtaining a copy "
        " of this software and associated documentation files (the Software), to deal "
        " in the Software without restriction, including without limitation the rights "
        " to use, copy, modify, merge, publish, distribute, sublicense, and/or sell "
        " copies of the Software, and to permit persons to whom the Software is "
        " furnished to do so, subject to the following conditions: "
        " The above copyright notice and this permission notice shall be included in all "
        " copies or substantial portions of the Software. "
        " THE SOFTWARE IS PROVIDED AS IS, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR "
        " IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, "
        " FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE "
        " AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER "
        " LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, "
        " OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE "
        " SOFTWARE.");
    ImGui::Separator();
    ImGui::End();

    UIElement::OnRender();
}

void UIEAbout::OnUpdate(float deltaTime)
{
    //if (IsVisible()) return;

    if (!m_openWindow)
    {
        RemoveThisWidget();
    }

    UIElement::OnUpdate(deltaTime);
}
