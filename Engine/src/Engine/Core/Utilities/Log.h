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

#include "LogSystem.h"

namespace Engine::Log
{
    /*enum LogType : int8_t
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
    /*class LogSystem 
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
       /* static std::shared_ptr<spdlog::logger> s_MainLogger;
    };*/
}

namespace Engine 
{
    using namespace Engine::Log;

} // namespace Engine

// ============================================================================
// CONVENIENT MACROS FOR USE
// ============================================================================

// Macros for quick logging with different levels.
// They call GetClientLogger() and the corresponding spdlog method.
// Usage: ENGINE_LOG_INFO("Player health: {}", health);
#define ENGINE_LOG_TRACE(...)         ::Engine::Log::LogSystem::GetClientLogger()->trace(__VA_ARGS__)
#define ENGINE_LOG_INFO(...)          ::Engine::Log::LogSystem::GetClientLogger()->info(__VA_ARGS__)
#define ENGINE_LOG_WARN(...)          ::Engine::Log::LogSystem::GetClientLogger()->warn(__VA_ARGS__)
#define ENGINE_LOG_ERROR(...)         ::Engine::Log::LogSystem::GetClientLogger()->error(__VA_ARGS__)
#define ENGINE_LOG_CRITICAL(...)      ::Engine::Log::LogSystem::GetClientLogger()->critical(__VA_ARGS__)


// ============================================================================
// ASSERTIONS (condition checks)
// ============================================================================

#ifdef _DEBUG
    // "CRASH PROGRAMM:\n"
    // Allows triggering a crash with an error window. Unlike assertions, calling this macro means
    // we forcibly want to terminate the program with a visual error display
    #define ENGINE_CRASH(message) \
        ENGINE_LOG_CRITICAL(std::string(message "\n\n" "The source of the error:\n" "File: ") + __FILE__ + "\n" "Line: " + std::to_string(__LINE__) + "\n""Function: " + __func__ + "\n\n""Please send this to the developer."); \
        throw std::runtime_error(std::string(message "\n\n" "The source of the error:\n" "File: ") + __FILE__ + "\n" "Line: " + std::to_string(__LINE__) + "\n""Function: " + __func__ + "\n\n""Please send this to the developer.") \

#else
    #define ENGINE_CRASH(message) do {} while(false)
#endif 

#ifdef _DEBUG
    // In debug builds, assert checks the condition, and if it's false,
    // outputs detailed error information (to log and cerr) and terminates the program abnormally.
    #define ENGINE_ASSERT(condition, message) \
        do { \
            if (!(condition)) { \
                ENGINE_LOG_ERROR("ASSERTION FAILED: {}", message); \
                ENGINE_LOG_ERROR("  File: {}", __FILE__); \
                ENGINE_LOG_ERROR("  Line: {}", __LINE__); \
                ENGINE_LOG_ERROR("  Function: {}", __FUNCTION__); \
                std::cerr << "\n=== ASSERTION FAILED ===\n"; \
                std::cerr << "Condition: " << #condition << "\n"; \
                std::cerr << "Message: " << message << "\n"; \
                std::cerr << "File: " << __FILE__ << ":" << __LINE__ << "\n"; \
                std::cerr << "=========================\n\n"; \
                ENGINE_CRASH(message); \
                std::abort(); \
            } \
        } while(false)
#else
     // In release builds, assertions become no-ops (removed by compiler)
    #define ENGINE_ASSERT(condition, message) do {} while(false)
#endif

// Simplified assert for internal engine use (without detailed output)
// Always active (even in release), but use wisely.
#define ENGINE_CORE_ASSERT(condition, message) \
    do { \
        if (!(condition)) { \
            ENGINE_LOG_ERROR("ENGINE ASSERTION FAILED: {}", message); \
            std::abort(); \
        } \
    } while(false)
