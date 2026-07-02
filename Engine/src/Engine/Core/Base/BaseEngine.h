// (c) Nikita Rogalev. All rights reserved.

#pragma once // Preprocessor directive for multiple inclusion guard

namespace Engine
{
    class BaseEngine
    {
        BaseEngine() = delete;
        ~BaseEngine() = delete;

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
}