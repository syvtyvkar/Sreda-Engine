// (c) Nikita Rogalev. All rights reserved.

#pragma once    // Multiple inclusion guard

#include "Engine/Core/Utilities/Types.h"
#include "Engine/Core/Utilities/Event.h"
#include "Engine/Render/Font/Font.h"
#include "Engine/Render/Texture.h"

namespace Engine::UI 
{
    struct UITextRenderCommand
    {
        UITextRenderCommand(std::wstring InText,TRef<class Engine::Render::Font> InFont,Vector2 InPosition,int InFontSize,TColor InColor,int InEntityID,float InLayout) 
        : text(InText),font(InFont),position(InPosition),fontSize(InFontSize),color(InColor),entityID(InEntityID),layout(InLayout), bIsScissor(false),SizeScissor(Vector2()),PositionScissor(Vector2()) {}

        UITextRenderCommand(std::wstring InText,TRef<class Engine::Render::Font> InFont,Vector2 InPosition,int InFontSize,TColor InColor,int InEntityID,float InLayout, Vector2 InSizeScissor, Vector2 InPositionScissor) 
        : text(InText),font(InFont),position(InPosition),fontSize(InFontSize),color(InColor),entityID(InEntityID),layout(InLayout), bIsScissor(true),SizeScissor(InSizeScissor),PositionScissor(InPositionScissor) {}

        std::wstring text;
        TRef<class Engine::Render::Font> font;
        Vector2 position;
        int fontSize;
        TColor color;
        int entityID;
        float layout;
        bool bIsScissor;
        Vector2 SizeScissor;
        Vector2 PositionScissor;
    };

    struct UIQuadRenderCommand
    {
        UIQuadRenderCommand(Vector2 InPosition,Vector2 InSize,TColor InColor, float InLayout) 
        : position(InPosition),size(InSize),color(InColor),textureIndex(0.f),tilingFactor(0),entityID(0), 
        texture(nullptr), layout(InLayout), bIsScissor(false),SizeScissor(Vector2()),PositionScissor(Vector2()) {}

        UIQuadRenderCommand(Vector2 InPosition, Vector2 InSize,TRef<class Engine::Render::Texture2D> InTexture, 
            float InTilingFactor, TColor InColor, float InLayout) 
        : position(InPosition),size(InSize),color(InColor),textureIndex(0.f),tilingFactor(InTilingFactor),
        entityID(0),texture(InTexture), layout(InLayout), bIsScissor(false),SizeScissor(Vector2()),PositionScissor(Vector2()) {}

        UIQuadRenderCommand(Vector2 InPosition, Vector2 InSize,TRef<class Engine::Render::Texture2D> InTexture, 
            float InTilingFactor, TColor InColor, float InLayout,Vector2 InPositionScissor, Vector2 InSizeScissor) 
        : position(InPosition),size(InSize),color(InColor),textureIndex(0.f),tilingFactor(InTilingFactor),
        entityID(0),texture(InTexture), layout(InLayout), bIsScissor(true),SizeScissor(InSizeScissor),PositionScissor(InPositionScissor) {}

        UIQuadRenderCommand(Vector2 InPosition,Vector2 InSize,TColor InColor, float InLayout,Vector2 InPositionScissor, Vector2 InSizeScissor) 
        : position(InPosition),size(InSize),color(InColor),textureIndex(0.f),tilingFactor(0),entityID(0), 
        texture(nullptr), layout(InLayout), bIsScissor(true),SizeScissor(InSizeScissor),PositionScissor(InPositionScissor) {}

        Vector2 position;
        Vector2 size;
        TColor color;
        float textureIndex;
        float tilingFactor;
        int entityID;
        TRef<class Engine::Render::Texture2D> texture;
        float layout;
        bool bIsScissor;
        Vector2 SizeScissor;
        Vector2 PositionScissor;
    };

    struct UIRectRenderCommand
    {
        UIRectRenderCommand(Vector2 InPosition,Vector2 InSize,TColor InColor,int InEntityID, float InLayout) 
        : position(InPosition),size(InSize),color(InColor),entityID(InEntityID), layout(InLayout) {}

        UIRectRenderCommand(Vector2 InPosition,Vector2 InSize,TColor InColor, float InLayout) 
        : position(InPosition),size(InSize),color(InColor),entityID(0), layout(InLayout) {}

        Vector2 position;
        Vector2 size;
        TColor color;
        int entityID;
        float layout;
    };

    struct UILineRenderCommand
    {
        UILineRenderCommand(const Vector2 InP0,const Vector2 InP1,TColor InColor,int InEntityID, float InLayout)
        : p0(InP0),p1(InP1),color(InColor),entityID(InEntityID), layout(InLayout) {}

        UILineRenderCommand(const Vector2 InP0,const Vector2 InP1,TColor InColor, float InLayout)
        : p0(InP0),p1(InP1),color(InColor),entityID(0), layout(InLayout) {}
        
        const Vector2 p0;
        const Vector2 p1;
        TColor color;
        int entityID;
        float layout;
    };
}