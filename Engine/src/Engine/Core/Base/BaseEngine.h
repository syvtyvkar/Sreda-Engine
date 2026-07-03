// (c) Nikita Rogalev. All rights reserved.

#pragma once // Preprocessor directive for multiple inclusion guard

#include "Engine/Core/Utilities/Types.h"

namespace Engine
{
    class EngineCore
    {
        EngineCore() = default;
        ~EngineCore() = default;

    protected:

        static void PreInit(int argc, char **argv)
        {

        };
        static void Init()
        {

        };
        static void Shotdown()
        {

        };
    };


    struct EngineContext
    {
        TUniquePtr<Vector2> m_LogSystem;
    };
}