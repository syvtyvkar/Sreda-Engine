// (c) Nikita Rogalev. All rights reserved.

#pragma once                                    // Защита от множественного включения

#include "Engine/Core/Utilities/Types.h"        // Основные типы движка (Color, Transform и др.)
#include "Engine/Render/Components/Mesh.h"      // Класс меша (геометрия)
#include "Engine/Render/Components/Material.h"  // Класс материала (шейдеры, текстуры, параметры)

namespace Engine
{

    struct RenderCommand 
    {
        enum Type 
        {
            DRAW_MESH,
            SET_VIEWPORT,
            CLEAR,
            SET_BLEND_MODE,
            SET_DEPTH_TEST,
            DRAW_BATCH
        };
        
        Type type;
        union 
        {
            struct 
            {
                Mesh* mesh;
                Material* material;
                Transform transform;
            } 
            draw;
            
            struct 
            {
                int x, y, width, height;
            } 
            viewport;
            
            struct 
            {
                Color color;
            } 
            clear;
            
            struct 
            {
                bool enabled;
            } 
            blend;
        };
    };

    struct RenderState 
    {
        int viewport[4] = {0, 0, 0, 0};
        Color clearColor = Color(0.0f, 0.0f, 0.0f, 1.0f);
        bool depthTestEnabled = true;
        bool blendingEnabled = false;
    };

    struct BatchItem 
    {
        Mesh* mesh;
        Material* material;
        Transform transform;
    };
}