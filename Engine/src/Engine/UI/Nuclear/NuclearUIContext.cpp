#include <glad/glad.h>


#define NK_IMPLEMENTATION
#define NK_GLFW_GL3_IMPLEMENTATION  

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_FONT
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_STANDARD_BOOL

#include "nuklear.h"
#include "nuklear_glfw_gl3.h"

#include "NuclearUIContext.h"

#include "Platform/WindowAPI/WindowGLFW/WindowGLFW.h"
#include "Engine/Core/FileSystem/FileIO.h"

namespace Engine::UI
{

    void NuclearUIContext::BeginFrame()
    {
        if (!Initialized) return;

        nk_glfw3_new_frame(GlfwCtx); 
        
    }

    void NuclearUIContext::Update()
    {
        DrawSimpleUI();
    }

    void NuclearUIContext::EndFrame()
    {
    }

    bool NuclearUIContext::DrawSimpleUI()
    {
        if (!Initialized) return false;

        bool pressed = false;

        if (nk_begin(Ctx, "MenuBar", nk_rect(0, 0, GlfwCtx->width, 30),
        NK_WINDOW_NO_SCROLLBAR /*| NK_WINDOW_NO_INPUT*/ | NK_WINDOW_BORDER))
        {
            nk_menubar_begin(Ctx);
            
            
            // File
            nk_layout_row_begin(Ctx, NK_STATIC, 25, 3);
            
            nk_layout_row_push(Ctx, 45);
            if (nk_menu_begin_label(Ctx, "File", NK_TEXT_LEFT, nk_vec2(120, 200))) {
                nk_layout_row_dynamic(Ctx, 25, 1);
                if (nk_menu_item_label(Ctx, "New", NK_TEXT_LEFT)) {  }
                if (nk_menu_item_label(Ctx, "Open", NK_TEXT_LEFT)) {  }
                nk_spacer(Ctx);
                //nk_separator(Ctx);
                if (nk_menu_item_label(Ctx, "Save", NK_TEXT_LEFT)) { }
                if (nk_menu_item_label(Ctx, "Exit", NK_TEXT_LEFT)) { 
                    // Можно отправить сигнал на закрытие
                }
                nk_menu_end(Ctx);
            }
            
            // Edit
            nk_layout_row_push(Ctx, 45);
            if (nk_menu_begin_label(Ctx, "Edit", NK_TEXT_LEFT, nk_vec2(120, 150))) {
                nk_layout_row_dynamic(Ctx, 25, 1);
                if (nk_menu_item_label(Ctx, "Undo", NK_TEXT_LEFT)) { }
                if (nk_menu_item_label(Ctx, "Redo", NK_TEXT_LEFT)) {  }
                nk_spacer(Ctx);
                //nk_separator(Ctx);
                if (nk_menu_item_label(Ctx, "Copy", NK_TEXT_LEFT)) {  }
                if (nk_menu_item_label(Ctx, "Paste", NK_TEXT_LEFT)) {  }
                nk_menu_end(Ctx);
            }
            
            // View
            nk_layout_row_push(Ctx, 45);
            if (nk_menu_begin_label(Ctx, "View", NK_TEXT_LEFT, nk_vec2(120, 120))) {
                nk_layout_row_dynamic(Ctx, 25, 1);
                static bool showSidebar = true;
                if (nk_menu_item_label(Ctx, "Toggle Sidebar", NK_TEXT_LEFT)) {
                    showSidebar = !showSidebar;
                }
                nk_menu_end(Ctx);
            }
            nk_layout_row_end(Ctx);
            
            nk_menubar_end(Ctx);
        }
        nk_end(Ctx);
        

        if (nk_begin(Ctx, "Demo", nk_rect(10, 10, 250, 150),
            NK_WINDOW_BORDER | NK_WINDOW_MOVABLE | NK_WINDOW_SCALABLE |
            NK_WINDOW_MINIMIZABLE | NK_WINDOW_TITLE))
        {
            nk_layout_row_dynamic(Ctx, 30, 1);
            nk_label(Ctx, "Hello from Nuklear!", NK_TEXT_LEFT);

            nk_layout_row_dynamic(Ctx, 30, 1);
            if (nk_button_label(Ctx, "Press me"))
                    pressed = true;
        }
        nk_end(Ctx);

        if (nk_begin(Ctx, "Debug Engine", nk_rect(50, 50, 250, 150),
            NK_WINDOW_MOVABLE | NK_WINDOW_SCALABLE |
            NK_WINDOW_MINIMIZABLE))
        {
            nk_layout_row_dynamic(Ctx, 30, 1);
            nk_label(Ctx, "This is debug panel!", NK_TEXT_LEFT);
        }
        nk_end(Ctx);


            const float sidebarWidth = 220.f;
            const float menuBarHeight = 30.f;
        
        if (nk_begin(Ctx, "Sidebar", 
            nk_rect(0, menuBarHeight, sidebarWidth, GlfwCtx->height - menuBarHeight),
            NK_WINDOW_BORDER | NK_WINDOW_NO_SCROLLBAR | NK_WINDOW_TITLE))
        {
            // Заголовок секции
            nk_layout_row_dynamic(Ctx, 30, 1);
            nk_label(Ctx, "Engine Tools", NK_TEXT_CENTERED);
            nk_spacer(Ctx);
            
            // Кнопки инструментов
            nk_layout_row_dynamic(Ctx, 30, 1);
            if (nk_button_label(Ctx, "> Play")) {}
            if (nk_button_label(Ctx, "> Pause")) {}
            if (nk_button_label(Ctx, "> Stop")) {}
            
            nk_spacer(Ctx);
            
            // Настройки отображения
            nk_layout_row_dynamic(Ctx, 20, 1);
            nk_label(Ctx, "Display Options:", NK_TEXT_LEFT);
            
            static bool showGrid = true;
            static bool showAxes = true;
            static bool showFPS = true;
            
            nk_layout_row_dynamic(Ctx, 20, 2);
            nk_checkbox_label(Ctx, "Grid", &showGrid);
            nk_checkbox_label(Ctx, "Axes", &showAxes);
            nk_checkbox_label(Ctx, "Show FPS", &showFPS);
        }
        nk_end(Ctx);

        return pressed;
    }

    void NuclearUIContext::Render()
    {
        if (!Initialized) return;

            // Сохранение GL состояния
            GLint last_program;
            glGetIntegerv(GL_CURRENT_PROGRAM, &last_program);

            GLint last_vao;
            glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &last_vao);

            GLint last_array_buffer;
            glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &last_array_buffer);

            GLint last_element_array_buffer;
            glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &last_element_array_buffer);

            GLboolean last_blend = glIsEnabled(GL_BLEND);
            GLboolean last_cull = glIsEnabled(GL_CULL_FACE);
            GLboolean last_depth = glIsEnabled(GL_DEPTH_TEST);
            GLboolean last_scissor = glIsEnabled(GL_SCISSOR_TEST);

            GLint last_viewport[4];
            glGetIntegerv(GL_VIEWPORT, last_viewport);

            // Рендер Nuklear
            nk_glfw3_render(GlfwCtx, NK_ANTI_ALIASING_ON, 512 * 1024, 128 * 1024);

            glUseProgram(last_program);
            glBindVertexArray(last_vao);
            glBindBuffer(GL_ARRAY_BUFFER, last_array_buffer);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, last_element_array_buffer);
            glViewport(last_viewport[0], last_viewport[1], last_viewport[2], last_viewport[3]);

            // Восстановление GL состояния
            if (last_blend) glEnable(GL_BLEND); else glDisable(GL_BLEND);
            if (last_cull) glEnable(GL_CULL_FACE); else glDisable(GL_CULL_FACE);
            if (last_depth) glEnable(GL_DEPTH_TEST); else glDisable(GL_DEPTH_TEST);
            if (last_scissor) glEnable(GL_SCISSOR_TEST); else glDisable(GL_SCISSOR_TEST);
    }

    void NuclearUIContext::Shutdown()
    {
        if (!Initialized) return;

        nk_glfw3_shutdown(GlfwCtx);

        if (GlfwCtx) 
        {
            free(GlfwCtx);
            GlfwCtx = nullptr;
        }

        if (Atlas) 
        {
            nk_font_atlas_clear(Atlas);
        }
        Ctx = nullptr;
        Atlas = nullptr;
        Window = nullptr;
        Initialized = false;
    }
    void NuclearUIContext::UpdateResizeCallback(GLFWwindow* inWindow, int width, int height)
    {
        if (GlfwCtx) 
        {
            int fbWidth, fbHeight;
            glfwGetFramebufferSize(Window, &fbWidth, &fbHeight);
            GlfwCtx->width = fbWidth;  
            GlfwCtx->height = fbHeight;
        }
    }
    void NuclearUIContext::InitContext(Engine::Window* window)
    {
        WindowGLFW* GLFWWin = static_cast<WindowGLFW*>(window);
        Window = GLFWWin->GetHandle();
        if (!Window) return;

        GlfwCtx = static_cast<nk_glfw*>(calloc(1, sizeof(nk_glfw)));
        if (!GlfwCtx) return;

        Ctx = nk_glfw3_init(GlfwCtx, Window, NK_GLFW3_DEFAULT);

        if (!Ctx) return;

        nk_glfw3_font_stash_begin(GlfwCtx, &Atlas);

        std::string LFilePathFont = FileIO::GetProjectDirectory() + "/Resources/Fonts/Cuprum.ttf";
        nk_font *font = nk_font_atlas_add_from_file(Atlas,LFilePathFont.c_str(),16,0);
        if (!font)
        {
            font = nk_font_atlas_add_default(Atlas, 14, nullptr);
        }

        nk_glfw3_font_stash_end(GlfwCtx);
        if (font) nk_style_set_font(Ctx, &font->handle);
        Initialized = true;
    }
    bool NuclearUIContext::LoadDocument(const std::string &path)
    {
        return false;
    }
    void NuclearUIContext::UnloadDocument()
    {
    }
    void NuclearUIContext::Show()
    {
    }
    void NuclearUIContext::Hide()
    {
    }
    bool NuclearUIContext::IsVisible() const
    {
        return false;
    }
}