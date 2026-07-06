// (c) Nikita Rogalev. All rights reserved.

#pragma once    // Multiple inclusion guard

#include <spdlog/spdlog.h>                      // spdlog main header
#include <spdlog/sinks/stdout_color_sinks.h>    // For colored console output
#include <spdlog/sinks/basic_file_sink.h>       // For file output
#include <memory>                               // For std::shared_ptr
#include <iostream>                             // For std::cerr
#include <cstdlib>                              // For std::abort

#include <string>
#include <stdexcept>

namespace Engine::Log
{
    enum LogType : int8_t
    {
        Temp,
        Trace,
        Info,
        Warning,
        Error,
        Critical
    };

    enum Category : int8_t
    {
        Custom,
        Core,
        Render,
        Input
    };

    /*  Log class handles initialization and provides a global spdlog logger.
        Implemented as a static class (all methods are static).*/
    class LogSystem 
    {
    public:
        static void Init();                                         // Initialize logger (call once at app startup)

        static void ShowMessageBox(uint8_t Type, const char* Tittle, const char* Message);

        static std::shared_ptr<spdlog::logger>& GetClientLogger()   // Get logger for client (game)
        { 
            return s_MainLogger;                                    // Returns reference to shared_ptr for direct use
        }

        static void SetLevel(spdlog::level::level_enum level);      // Set logging level (trace, debug, info, warn, error, critical)

    private:
        /*  Static pointer to the main logger (initialized in Init)
            Used for all messages from game code*/
        static std::shared_ptr<spdlog::logger> s_MainLogger;
    };
}

namespace Engine 
{
    using namespace Engine::Log;

} // namespace Engine