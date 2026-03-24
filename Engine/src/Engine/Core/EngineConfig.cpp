#include "EngineConfig.h"
#include "Engine/Core/Log.h"
#include <fstream>
#include <filesystem>

#ifdef ENGINE_CONFIG_EMBEDDED
    #include "config_embed.h"  // Сгенерированный header
#endif

#include <fstream>
#include <filesystem>
#include <algorithm>

namespace EngineConfig 
{
    ConfigSystem* ConfigSystem::s_instance = nullptr;

    /**
     * @brief Получить синглтон
     */
    ConfigSystem& ConfigSystem::Get() 
    {
        if (!s_instance) 
        {
            s_instance = new ConfigSystem();
        }
        return *s_instance;
    }

    bool ConfigSystem::Init(std::string InBasePath)  // Инициализация наших конфигураций
    {
        if (m_embeddedLoaded)   // Пытались ли мы уже грузить сгенерированный файл конфигурации?
        {
#ifdef ENGINE_CONFIG_EMBEDDED
            std::lock_guard<std::mutex> lock(ConfigSystem::Get().m_mutex);              // Никто не имеет права трогать файл, пока с ним работаем мы!
            try 
            {
                // Парсим встроенную JSON-строку
                ConfigSystem::Get().m_data = json::parse(ConfigEmbed::DefaultConfigJSON);                               // Парсим данные
                ConfigSystem::Get().m_embeddedLoaded = true;                                                            // Сообщаем, что больше считывать данные по умолчанию не требуется
                ENGINE_LOG_INFO("Embedded config loaded ({} bytes)", strlen(ConfigEmbed::DefaultConfigJSON));           // Логируем
            } 
            catch (const json::parse_error& e)                                                                          // Упс, что-то пошло не так!
            {
                ENGINE_LOG_ERROR("Failed to parse embedded config: {}", e.what()); ConfigSystem::Get().m_data = json::object();  // Пустой объект как fallback
            }
#else
            ENGINE_LOG_WARN("Embedded config not available (ENGINE_CONFIG_EMBEDDED not defined)");  // Если мы пришли сюда - проблема где-то в генерации файла. Возможно, CMake?
            return false;
#endif
        }

        if (std::filesystem::exists(InBasePath))                                        // Файл существует?
        {
            try 
            {
                std::ifstream file(InBasePath);                                         // Обращаемся к файлу конфига                                 
                if (file.is_open()) 
                {
                    json userConfig = json::parse(file);                                // Парсим данные из пользовательских конфигов
                    m_data.merge_patch(userConfig);                                     // Мерджим данные из пользовательских конфигов
                    //MarkOverriddenKeys(userConfig,"");                                // Отмечаем изменённые ключи (упрощённо: все ключи из userConfig)
                    
                    //ENGINE_LOG_INFO("User config merged: {} overrides applied", CountKeys(userConfig));
                    return true;
                }
            } 
            catch (const std::exception& e) 
            {
                ENGINE_LOG_WARN("Failed to load user config {}: {}", InBasePath, e.what());
                // Не ошибка — просто используем дефолты
            }
        }
        
        ENGINE_LOG_INFO("Using embedded config defaults (no user override)");
        return false;
    }

    bool ConfigSystem::DeInit()
    {
        return false;
    }
}