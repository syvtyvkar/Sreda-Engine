// (c) Nikita Rogalev. All rights reserved.

#include "Log.h"
#include <spdlog/fmt/ostr.h>  // For custom type support in logs

// Headers for time operations
#include <chrono>
#include <ctime>
#include <sstream>
#include <iomanip>

#ifdef WIN32
    #include <windows.h>
#endif

namespace Engine::Log
{
    std::shared_ptr<spdlog::logger> LogSystem::s_MainLogger;                      // Static pointer to the main logger (initialized in Init)

    void LogSystem::Init()                                                        // Initialize logging system
    {          

        auto now = std::chrono::system_clock::now();                        // Get current time for log file name
        auto time_t_now = std::chrono::system_clock::to_time_t(now);
        std::tm tm_now;                                                     // Convert time to local (tm struct) platform-dependently

        #ifdef WIN32
            localtime_s(&tm_now, &time_t_now);                              // Windows: use safe localtime_s
        #else
            localtime_r(&time_t_now, &tm_now);                              // Unix-like: localtime_r (thread-safe)
        #endif
        std::ostringstream oss;                                             // Format date-time string as "YYYY-MM-DD_HH-MM-SS"
        oss << std::put_time(&tm_now, "%Y-%m-%d_%H-%M-%S");
        //std::string NameLogFile = "../logs/Log_" + oss.str() + ".log";       // Was planned to have dynamic file name with timestamp, but fixed name is used for now
        std::string NameLogFile = "../Logs/Log.log";

        spdlog::sinks_init_list logSinks =                                          // List of log sinks:
        {                    
            std::make_shared<spdlog::sinks::stdout_color_sink_mt>(),                // - stdout_color_sink_mt — colored console output (multi-threaded)
            std::make_shared<spdlog::sinks::basic_file_sink_mt>(NameLogFile, true)  // - basic_file_sink_mt — file output (append, true)
        };
        
        s_MainLogger = std::make_shared<spdlog::logger>("LOG", logSinks.begin(), logSinks.end());   // Create logger with name "LOG" and specified sinks
        spdlog::register_logger(s_MainLogger);                                                      // Register logger in spdlog global registry (so it can be retrieved by name)
        s_MainLogger->set_pattern("%^[%T] %n: %v%$");                                               // Set output format
        s_MainLogger->set_level(spdlog::level::trace);                                              // Set logging level to trace (most detailed)
    }

    void LogSystem::ShowMessageBox(uint8_t Type, const char* Tittle, const char* Message)
    {
        #ifdef WIN32
        MessageBox(GetActiveWindow(), Message, Tittle, MB_ICONERROR);
        #endif
    }

    void LogSystem::SetLevel(spdlog::level::level_enum level)                      // Method to change logging level at runtime
    {
        s_MainLogger->set_level(level);
    }
}
