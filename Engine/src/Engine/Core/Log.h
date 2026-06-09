// (c) Nikita Rogalev. All rights reserved.

#pragma once    // Защита от множественного включения

#include <spdlog/spdlog.h>                      // Основной заголовок spdlog
#include <spdlog/sinks/stdout_color_sinks.h>    // Для цветного вывода в консоль
#include <spdlog/sinks/basic_file_sink.h>       // Для записи в файл
#include <memory>                               // Для std::shared_ptr
#include <iostream>                             // Для std::cerr
#include <cstdlib>                              // Для std::abort

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

    /*  Класс Log отвечает за инициализацию и предоставление глобального логгера spdlog.
        Реализован как статический класс (все методы статические).*/
    class LogSystem 
    {
    public:
        static void Init();                                         // Инициализация логгера (вызывать один раз при старте приложения)

        static void ShowMessageBox(uint8_t Type, const char* Tittle, const char* Message);

        static std::shared_ptr<spdlog::logger>& GetClientLogger()   // Получить логгер для клиента (игры)
        { 
            return s_MainLogger;                                    // Возвращает ссылку на shared_ptr, чтобы можно было использовать напрямую
        }

        static void SetLevel(spdlog::level::level_enum level);      // Установить уровень логирования (trace, debug, info, warn, error, critical)

    private:
        /*  Статический указатель на основной логгер (инициализируется в Init)
            Используется для всех сообщений от игрового кода*/
        static std::shared_ptr<spdlog::logger> s_MainLogger;
    };
}

namespace Engine 
{
    using namespace Engine::Log;

} // namespace Engine

// ============================================================================
// УДОБНЫЕ МАКРОСЫ ДЛЯ ИСПОЛЬЗОВАНИЯ
// ============================================================================

// Макросы для быстрого логирования с разными уровнями.
// Они вызывают GetClientLogger() и соответствующий метод spdlog.
// Использование: ENGINE_LOG_INFO("Player health: {}", health);
#define ENGINE_LOG_TRACE(...)         ::Engine::Log::LogSystem::GetClientLogger()->trace(__VA_ARGS__)
#define ENGINE_LOG_INFO(...)          ::Engine::Log::LogSystem::GetClientLogger()->info(__VA_ARGS__)
#define ENGINE_LOG_WARN(...)          ::Engine::Log::LogSystem::GetClientLogger()->warn(__VA_ARGS__)
#define ENGINE_LOG_ERROR(...)         ::Engine::Log::LogSystem::GetClientLogger()->error(__VA_ARGS__)
#define ENGINE_LOG_CRITICAL(...)      ::Engine::Log::LogSystem::GetClientLogger()->critical(__VA_ARGS__)


// ============================================================================
// АССЕРТЫ (проверки условий)
// ============================================================================

#ifdef _DEBUG
    // "CRASH PROGRAMM:\n"
    // Позволяет вызвать краш с выводом окна ошибки. В отличии от ассертов, сам факт вызова этого макроса означает, 
    // что мы принудительно хотим завершить программу с выводом визуального отображения ошибки
    #define ENGINE_CRASH(message) \
        ENGINE_LOG_CRITICAL(std::string(message "\n\n" "The source of the error:\n" "File: ") + __FILE__ + "\n" "Line: " + std::to_string(__LINE__) + "\n""Function: " + __func__ + "\n\n""Please send this to the developer."); \
        throw std::runtime_error(std::string(message "\n\n" "The source of the error:\n" "File: ") + __FILE__ + "\n" "Line: " + std::to_string(__LINE__) + "\n""Function: " + __func__ + "\n\n""Please send this to the developer.") \

#else
    #define ENGINE_CRASH(message) do {} while(false)
#endif 

#ifdef _DEBUG
    // В отладочной сборке ассерт проверяет условие, и если оно ложно,
    // выводит развёрнутую информацию об ошибке (в лог и в cerr) и аварийно завершает программу.
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
     // В релизной сборке ассерты превращаются в пустую операцию (удаляются компилятором)
    #define ENGINE_ASSERT(condition, message) do {} while(false)
#endif

// Упрощённый вариант ассерта для внутренних нужд движка (без детального вывода)
// Всегда активен (даже в релизе), но лучше использовать с умом.
#define ENGINE_CORE_ASSERT(condition, message) \
    do { \
        if (!(condition)) { \
            ENGINE_LOG_ERROR("ENGINE ASSERTION FAILED: {}", message); \
            std::abort(); \
        } \
    } while(false)