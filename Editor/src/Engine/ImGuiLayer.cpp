#include "ImGuiLayer.h"

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#include "Engine/Core/App/Application.h"
#include "Engine/Core/Base/EngineCore.h"
#include "Engine/Platform/WindowManager.h"
#include "Engine/Platform/IWindow.h"

#include <GLFW/glfw3.h>

ImGuiLayer::ImGuiLayer()
    : Layer()
{
}

void ImGuiLayer::OnInit(Engine::Application* InApp)
{
    if (m_Initialized) return;

    Engine::IWindow* LWin = Engine::EngineCore::GetEngineContext().GetWindowManager()->GetEngineWindowActiv();
    if (!LWin)
    {
        ENGINE_LOG_ERROR("ImGuiLayer: No active window found!");
        return;
    }

    m_WindowHandle = LWin->GetWindowAPIHandle();
    if (!m_WindowHandle)
    {
        ENGINE_LOG_ERROR("ImGuiLayer: Invalid window handle!");
        return;
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    ///////////////////////////////////////////////////
    ///////////////////////////////////////////////////
    ///////////////////////////////////////////////////

    SetupImGuiStyle();

    //////////////////////////////////////////////////
    ///////////////////////////////////////////////////
    ///////////////////////////////////////////////////

    GLFWwindow* window = static_cast<GLFWwindow*>(m_WindowHandle);
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 450");

    m_Initialized = true;
    ENGINE_LOG_INFO("ImGuiLayer initialized successfully");
}

void ImGuiLayer::OnDeInit()
{
    if (!m_Initialized) return;

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    m_Initialized = false;
    m_WindowHandle = nullptr;
    ENGINE_LOG_INFO("ImGuiLayer shut down");
}

void ImGuiLayer::BeginFrame()
{
    if (!m_Initialized) return;

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void ImGuiLayer::EndFrame()
{
    if (!m_Initialized) return;

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        GLFWwindow* backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
    }
}

void ImGuiLayer::SetupImGuiStyle()
{
    ImGuiIO& io = ImGui::GetIO();

    io.Fonts->Clear();
    io.Fonts->AddFontFromFileTTF("../Resources/Fonts/Inter_18pt-Regular.ttf", 16);
    io.Fonts->AddFontFromFileTTF("../Resources/Fonts/Inter_18pt-Regular.ttf", 32);
    io.Fonts->AddFontFromFileTTF("../Resources/Fonts/Inter_18pt-Regular.ttf", 11);
    io.Fonts->Build();

    ImGuiStyle& style = ImGui::GetStyle();
	
	style.Alpha = 1.0f;
	style.DisabledAlpha = 0.6f;
	style.WindowPadding = ImVec2(6.0f, 3.0f);
	style.WindowRounding = 6.0f;
	style.WindowBorderSize = 1.0f;
	style.WindowMinSize = ImVec2(32.0f, 32.0f);
	style.WindowTitleAlign = ImVec2(0.5f, 0.5f);
	style.WindowMenuButtonPosition = ImGuiDir_Left;
	style.ChildRounding = 0.0f;
	style.ChildBorderSize = 1.0f;
	style.PopupRounding = 0.0f;
	style.PopupBorderSize = 1.0f;
	style.FramePadding = ImVec2(5.0f, 1.0f);
	style.FrameRounding = 3.0f;
	style.FrameBorderSize = 1.0f;
	style.ItemSpacing = ImVec2(8.0f, 4.0f);
	style.ItemInnerSpacing = ImVec2(4.0f, 4.0f);
	style.CellPadding = ImVec2(4.0f, 2.0f);
	style.IndentSpacing = 21.0f;
	style.ColumnsMinSpacing = 6.0f;
	style.ScrollbarSize = 13.0f;
	style.ScrollbarRounding = 16.0f;
	style.GrabMinSize = 20.0f;
	style.GrabRounding = 2.0f;
	style.TabRounding = 4.0f;
	style.TabBorderSize = 1.0f;
	style.TabMinWidthForCloseButton = 0.0f;
	style.ColorButtonPosition = ImGuiDir_Right;
	style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
	style.SelectableTextAlign = ImVec2(0.0f, 0.0f);
	
	style.Colors[ImGuiCol_Text] = ImVec4(0.8745098f, 0.87058824f, 0.8392157f, 1.0f);
	style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.58431375f, 0.57254905f, 0.52156866f, 1.0f);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.23529412f, 0.21960784f, 0.21176471f, 1.0f);
	style.Colors[ImGuiCol_ChildBg] = ImVec4(0.23529412f, 0.21960784f, 0.21176471f, 1.0f);
	style.Colors[ImGuiCol_PopupBg] = ImVec4(0.23529412f, 0.21960784f, 0.21176471f, 1.0f);
	style.Colors[ImGuiCol_Border] = ImVec4(0.3137255f, 0.28627452f, 0.27058825f, 1.0f);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.23529412f, 0.21960784f, 0.21176471f, 0.0f);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(0.3137255f, 0.28627452f, 0.27058825f, 1.0f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.4f, 0.36078432f, 0.32941177f, 1.0f);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.4862745f, 0.43529412f, 0.39215687f, 1.0f);
	style.Colors[ImGuiCol_TitleBg] = ImVec4(0.23529412f, 0.21960784f, 0.21176471f, 1.0f);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.3137255f, 0.28627452f, 0.27058825f, 1.0f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.23529412f, 0.21960784f, 0.21176471f, 1.0f);
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.3137255f, 0.28627452f, 0.27058825f, 1.0f);
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.23529412f, 0.21960784f, 0.21176471f, 1.0f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.4862745f, 0.43529412f, 0.39215687f, 1.0f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.4f, 0.36078432f, 0.32941177f, 1.0f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.3137255f, 0.28627452f, 0.27058825f, 1.0f);
	style.Colors[ImGuiCol_CheckMark] = ImVec4(0.59607846f, 0.5921569f, 0.101960786f, 1.0f);
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.59607846f, 0.5921569f, 0.101960786f, 1.0f);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.7411765f, 0.7176471f, 0.41960785f, 1.0f);
	style.Colors[ImGuiCol_Button] = ImVec4(0.4f, 0.36078432f, 0.32941177f, 1.0f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.4862745f, 0.43529412f, 0.39215687f, 1.0f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.7411765f, 0.7176471f, 0.41960785f, 1.0f);
	style.Colors[ImGuiCol_Header] = ImVec4(0.4f, 0.36078432f, 0.32941177f, 1.0f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.4862745f, 0.43529412f, 0.39215687f, 1.0f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.7411765f, 0.7176471f, 0.41960785f, 1.0f);
	style.Colors[ImGuiCol_Separator] = ImVec4(0.2f, 0.2f, 0.2f, 1.0f);
	style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.4862745f, 0.43529412f, 0.39215687f, 1.0f);
	style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.7411765f, 0.7176471f, 0.41960785f, 1.0f);
	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.4f, 0.36078432f, 0.32941177f, 1.0f);
	style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.4862745f, 0.43529412f, 0.39215687f, 1.0f);
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.7411765f, 0.7176471f, 0.41960785f, 1.0f);
	style.Colors[ImGuiCol_Tab] = ImVec4(0.3137255f, 0.28627452f, 0.27058825f, 1.0f);
	style.Colors[ImGuiCol_TabHovered] = ImVec4(0.4f, 0.36078432f, 0.32941177f, 1.0f);
	style.Colors[ImGuiCol_TabActive] = ImVec4(0.4862745f, 0.43529412f, 0.39215687f, 1.0f);
	style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.23529412f, 0.21960784f, 0.21176471f, 0.972549f);
	style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.3137255f, 0.28627452f, 0.27058825f, 1.0f);
	style.Colors[ImGuiCol_PlotLines] = ImVec4(0.7411765f, 0.7176471f, 0.41960785f, 1.0f);
	style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.8392157f, 0.7490196f, 0.4f, 1.0f);
	style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.7411765f, 0.7176471f, 0.41960785f, 1.0f);
	style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.8392157f, 0.7490196f, 0.4f, 1.0f);
	style.Colors[ImGuiCol_TableHeaderBg] = ImVec4(0.8392157f, 0.7490196f, 0.4f, 0.60944206f);
	style.Colors[ImGuiCol_TableBorderStrong] = ImVec4(0.30980393f, 0.30980393f, 0.34901962f, 1.0f);
	style.Colors[ImGuiCol_TableBorderLight] = ImVec4(0.22745098f, 0.22745098f, 0.24705882f, 1.0f);
	style.Colors[ImGuiCol_TableRowBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
	style.Colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.0f, 1.0f, 1.0f, 0.06f);
	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.8392157f, 0.7490196f, 0.4f, 0.43137255f);
	style.Colors[ImGuiCol_DragDropTarget] = ImVec4(0.8392157f, 0.7490196f, 0.4f, 0.9019608f);
	style.Colors[ImGuiCol_NavHighlight] = ImVec4(0.23529412f, 0.21960784f, 0.21176471f, 1.0f);
	style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.0f, 1.0f, 1.0f, 0.7f);
	style.Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.8f, 0.8f, 0.8f, 0.2f);
	style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.8f, 0.8f, 0.8f, 0.35f);
}
