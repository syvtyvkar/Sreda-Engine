// (c) Nikita Rogalev. All rights reserved.

#pragma once

#include "Engine/Core/Utilities/Types.h"
#include "Engine/UI/Framework/UIRenderType.h"
#include "Engine/Render/Camera/OrthographicCamera.h"
#include <memory>
#include <unordered_map>
#include <variant>

namespace Engine::UI 
{
    using namespace Engine;

    using UICommand = std::variant<UIQuadRenderCommand, UIRectRenderCommand, 
    UITextRenderCommand,UILineRenderCommand>;

    class UICommandList 
    {
    public:

        void PushText(const UITextRenderCommand& cmd) 
        { 
            m_commands.push_back(cmd); 
        }
        void PushLine(const UILineRenderCommand& cmd) 
        { 
            m_commands.push_back(cmd); 
        }
        void PushRect(const UIRectRenderCommand& cmd) 
        { 
            m_commands.push_back(cmd); 
        }
        void PushQuad(const UIQuadRenderCommand& cmd) 
        { 
            m_commands.push_back(cmd); 
        }

        void Clear() { m_commands.clear(); };
        void ExecuteCommands(const OrthographicCamera &InCamera);
        const std::vector<UICommand>& GetCommands() const { return m_commands; }

    private:
        std::vector<UICommand> m_commands;
    };
}