// (c) Nikita Rogalev. All rights reserved.

#include "RenderCommand.h"

namespace Engine::Render
{
    TUniquePtr<RendererAPI> RenderCommand::s_RendererAPI = RendererAPI::Create();
}