// (c) Nikita Rogalev. All rights reserved.

#include "Log.h"
#include <spdlog/fmt/ostr.h>  // Для поддержки пользовательских типов в логах

// Заголовки для работы со временем
#include <chrono>
#include <ctime>
#include <sstream>
#include <iomanip>

namespace Engine 
{
    std::shared_ptr<spdlog::logger> Log::s_MainLogger;                      // Статический указатель на основной логгер (инициализируется в Init)

    void Log::Init()                                                        // Инициализация системы логирования
    {          

        auto now = std::chrono::system_clock::now();                        // Получаем текущее время для формирования имени файла лога
        auto time_t_now = std::chrono::system_clock::to_time_t(now);
        std::tm tm_now;                                                     // Преобразуем время в локальное (структура tm) с учётом платформы

        #ifdef WIN32
            localtime_s(&tm_now, &time_t_now);                              // Windows: используем безопасную функцию localtime_s
        #else
            localtime_r(&time_t_now, &tm_now);                              // Unix-подобные: localtime_r (потокобезопасная)
        #endif
        std::ostringstream oss;                                             // Формируем строку с датой и временем вида "ГГГГ-ММ-ДД_ЧЧ-ММ-СС"
        oss << std::put_time(&tm_now, "%Y-%m-%d_%H-%M-%S");
        //std::string NameLogFile = "logs/Log_" + oss.str() + ".log";       // Было задумано динамическое имя файла с временной меткой, но пока используется фиксированное
        std::string NameLogFile = "Logs/Log.log";

        spdlog::sinks_init_list logSinks =                                          // Список приёмников (sinks) лога:
        {                    
            std::make_shared<spdlog::sinks::stdout_color_sink_mt>(),                // - stdout_color_sink_mt — цветной вывод в консоль (поддержка многопоточности)
            std::make_shared<spdlog::sinks::basic_file_sink_mt>(NameLogFile, true)  // - basic_file_sink_mt — запись в файл (с дозаписью, true)
        };
        
        s_MainLogger = std::make_shared<spdlog::logger>("LOG", logSinks.begin(), logSinks.end());   // Создаём логгер с именем "LOG" и заданными приёмниками
        spdlog::register_logger(s_MainLogger);                                                      // Регистрируем логгер в глобальном реестре spdlog (чтобы можно было получать по имени)
        s_MainLogger->set_pattern("%^[%T] %n: %v%$");                                               // Устанавливаем формат вывода
        s_MainLogger->set_level(spdlog::level::trace);                                              // Устанавливаем уровень логирования — trace (самый подробный)
    }

    void Log::SetLevel(spdlog::level::level_enum level)                      // Метод для изменения уровня логирования во время выполнения
    {
        s_MainLogger->set_level(level);
    }
}