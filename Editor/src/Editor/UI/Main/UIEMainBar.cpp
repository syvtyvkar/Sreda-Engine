#include "UIEMainBar.h"
#include "imgui.h"

#include "Windows/UIEAbout.h"


void UIEMainBar::OnInit()
{
}

void UIEMainBar::OnRender()
{
    //if (IsVisible()) return;

    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            ImGui::MenuItem("New verse", NULL, &m_NewVerse);
            ImGui::MenuItem("Open verse", NULL, &m_OpenVerse);
            ImGui::Separator();
            ImGui::MenuItem("New project", NULL, &m_NewProject);
            ImGui::MenuItem("Open project", NULL, &m_OpenProject);
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Help"))
        {
            ImGui::MenuItem("About", NULL, &m_About);
            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }

    UIElement::OnRender();
}

void UIEMainBar::OnUpdate(float deltaTime)
{
    //if (IsVisible()) return;

    if (m_About)
    {
        m_About = false;

        AddChild(CreateRef<UIEAbout>());
    }

    UIElement::OnUpdate(deltaTime);
}
