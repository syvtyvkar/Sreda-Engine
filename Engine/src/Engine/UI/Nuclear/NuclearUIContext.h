#pragma once

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_FONT
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_STANDARD_BOOL

//#include "nuklear.h"
//#include "nuklear_glfw_gl3.h"
//#include <GLFW/glfw3.h>

#include "Engine/UI/Framework/IUIContext.h"

namespace Engine::UI
{
    class NuclearUIContext : public UIContext
    {
    public:
        NuclearUIContext() = default;
        virtual ~NuclearUIContext() = default;

        struct nk_glfw* GlfwCtx = nullptr;
        struct GLFWwindow* Window = nullptr;
        struct nk_context* Ctx = nullptr;
        struct nk_font_atlas* Atlas = nullptr;
        bool Initialized = false;

        virtual void BeginFrame() override;
        virtual void EndFrame() override;

        bool DrawSimpleUI();
        virtual void Render() override;
        void Shutdown();
        void UpdateResizeCallback(GLFWwindow* inWindow, int width, int height);


        virtual void InitContext(Engine::Window* window) override;

        // UIContext
        virtual bool LoadDocument(const std::string& path) override;
        virtual void UnloadDocument() override;

        virtual void Show() override;
        virtual void Hide() override;
        virtual bool IsVisible() const override;

        virtual void Update() override;
    };
}