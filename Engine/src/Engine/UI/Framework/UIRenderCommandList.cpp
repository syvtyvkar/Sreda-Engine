// (c) Nikita Rogalev. All rights reserved.

#include "UIRenderCommandList.h"
#include "Engine/Render/Renderer2D.h"
#include "Engine/Render/RenderCommand.h"
#include <variant>

namespace Engine::UI 
{
    void UICommandList::ExecuteCommands(const OrthographicCamera &InCamera)
    {
        for (const auto& cmd : GetCommands()) 
        {std::visit([InCamera](const auto& c) 
            {
                using T = std::decay_t<decltype(c)>;
                if constexpr (std::is_same_v<T, UIQuadRenderCommand>) 
                {
                    if (c.texture.get())
                    {
                        if (c.bIsScissor)
                        {
                            Renderer2D::EndScene();
                            Renderer2D::BeginScene(InCamera);
                            RenderCommand::EnableScissor(true);
                            RenderCommand::SetScissor(static_cast<int>(c.PositionScissor.x), static_cast<int>(c.PositionScissor.y), static_cast<int>(c.SizeScissor.x), static_cast<int>(c.SizeScissor.y));
                            Renderer2D::DrawQuad(Vector3(c.position.x,c.position.y,-(c.layout)),c.size,c.texture,c.tilingFactor,c.color);
                            Renderer2D::EndScene();
                            RenderCommand::EnableScissor(false);
                            Renderer2D::BeginScene(InCamera);
                        }
                        else
                        {
                            Renderer2D::DrawQuad(Vector3(c.position.x,c.position.y,-(c.layout)),c.size,c.texture,c.tilingFactor,c.color);
                        }
                    }
                    else
                    {
                        if (c.bIsScissor)
                        {
                            Renderer2D::EndScene();
                            Renderer2D::BeginScene(InCamera);
                            RenderCommand::EnableScissor(true);
                            RenderCommand::SetScissor(static_cast<int>(c.PositionScissor.x), static_cast<int>(c.PositionScissor.y), static_cast<int>(c.SizeScissor.x), static_cast<int>(c.SizeScissor.y));
                            Renderer2D::DrawQuad(Vector3(c.position.x,c.position.y,-(c.layout)),c.size,c.color);
                            Renderer2D::EndScene();
                            RenderCommand::EnableScissor(false);
                            Renderer2D::BeginScene(InCamera);
                        }
                        else
                        {
                            Renderer2D::DrawQuad(Vector3(c.position.x,c.position.y,-(c.layout)),c.size,c.color);
                        }
                    }
                } 
                else if constexpr (std::is_same_v<T, UIRectRenderCommand>) 
                {
                    Renderer2D::DrawRect(Vector3(c.position.x,c.position.y,-(c.layout)), c.size, c.color,c.entityID);
                } 
                else if constexpr (std::is_same_v<T, UITextRenderCommand>) 
                {
                    if (c.bIsScissor)
                    {
                        Renderer2D::EndScene();
                        Renderer2D::BeginScene(InCamera);
                        RenderCommand::EnableScissor(true);
                        //RenderCommand::SetScissor(static_cast<int>(c.position.x), static_cast<int>(c.position.y), static_cast<int>(c.SizeScissor.x), static_cast<int>(c.SizeScissor.y));
                        RenderCommand::SetScissor(static_cast<int>(c.PositionScissor.x), 0, static_cast<int>(c.SizeScissor.x), 1500);
                        Renderer2D::RenderDrawText(c.text, c.font->GetAtlasTexture(),c.font->GetGlyphs(),c.position.x,c.position.y,-(c.layout),c.fontSize,c.color,c.entityID);
                        Renderer2D::EndScene();
                        RenderCommand::EnableScissor(false);
                        Renderer2D::BeginScene(InCamera);
                    }
                    else
                    {
                        Renderer2D::RenderDrawText(c.text, c.font->GetAtlasTexture(),c.font->GetGlyphs(),c.position.x,c.position.y,-(c.layout),c.fontSize,c.color,c.entityID);
                    }
                }
                else if constexpr (std::is_same_v<T, UILineRenderCommand>) 
                {
                    Renderer2D::DrawLine(Vector3(c.p0.x,c.p0.y,-(c.layout)), Vector3(c.p1.x,c.p1.y,-(c.layout)), c.color, c.entityID);
                }
            }, cmd);
        }
    }
}