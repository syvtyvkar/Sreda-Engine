#include "EngineConfig.h"
#include "Engine/Core/Log.h"
#include "Engine/Core/FileSystem/ResourceManager.h"
#include "Engine/Core/FileSystem/FileIO.h"
//#include <fstream>
//#include <filesystem>

#ifdef ENGINE_CONFIG_EMBEDDED
    #include "config_embed.h"  // Сгенерированный header
#endif

#include <fstream>
#include <algorithm>

namespace EngineConfig 
{
    ConfigSystem* ConfigSystem::s_instance = nullptr;
    using namespace Engine;

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

    bool ConfigSystem::Init(std::string InTypeConfig)  // Инициализация наших конфигураций
    {
        ENGINE_LOG_INFO("Init configurate file: {}", Engine::FileIO::GetProjectDirectory() + InTypeConfig); 
        if (!m_embeddedLoaded)   // Пытались ли мы уже грузить сгенерированный файл конфигурации?
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
            ENGINE_LOG_ERROR("Embedded config not available (ENGINE_CONFIG_EMBEDDED not defined)");  // Если мы пришли сюда - проблема где-то в генерации файла. Возможно, CMake?
            return false;
#endif
        }
        if (Engine::FileIO::IsProjectFileExist(InTypeConfig))                                        // Файл существует?
        {
            try 
            {
                //std::ifstream File;
                auto lFileContent = Engine::FileIO::ReadTextFile(Engine::FileIO::GetProjectDirectory() + InTypeConfig);                                 
                if (!lFileContent->empty()) 
                {
                    json userConfig = json::parse(lFileContent.value());              // Парсим данные из пользовательских конфигов
                    m_data.merge_patch(userConfig);                                   // Мерджим данные из пользовательских конфигов
                    MarkOverriddenKeys(userConfig,"");                                // Отмечаем изменённые ключи (упрощённо: все ключи из userConfig)
                    
                    ENGINE_LOG_INFO("User config merged: {} overrides applied", CountKeys(userConfig));
                    return true;
                }
            } 
            catch (const std::exception& e) 
            {
                ENGINE_LOG_WARN("Failed to load user config {}: {}", Engine::FileIO::GetProjectDirectory() + InTypeConfig, e.what());
            }
        }
        else
        {
            ENGINE_LOG_WARN("No found config file: {}", InTypeConfig);
        }
        
        ENGINE_LOG_INFO("Using embedded config defaults (no user override)");
        return false;
    }

    bool ConfigSystem::DeInit()
    {
        m_overriddenKeys.clear();
        m_data = nullptr;
        s_instance = nullptr;
        ENGINE_LOG_INFO("Stop configurate system"); 
        return true;
    }

    void ConfigSystem::MarkOverriddenKeys(const json &InConfig, const std::string &InPrefix)    // Рекурсивно отмечаем все ключи как "изменённые"
    {
        for (auto& [key, value] : InConfig.items()) 
        {
            std::string fullPath = InPrefix.empty() ? key : InPrefix + "." + key;                   // Путь к цели
            if (value.is_object()) 
            {
                MarkOverriddenKeys(value, fullPath);        // Рекурсия для вложенных объектов
            } 
            else 
            {
                m_overriddenKeys.insert(fullPath);          // Помечаем как измененный
            }
        }
    }

    void ConfigSystem::MarkOverridden(const std::string &key)           // Пометить ключи как измененные
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_overriddenKeys.insert(key);
    }

    bool ConfigSystem::SaveConfig(const std::string &InTypeConfig)        // Сохраняем конфиги определенного типа
    {
        json delta;
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            
            for (const auto& key : m_overriddenKeys) 
            {
                const json* value = GetNestedValue(key);
                if (value) 
                {
                    auto parts = SplitPath(key);
                    if (parts.empty()) continue;;
        
                    json* current = &delta;
        
                    for (size_t i = 0; i < parts.size() - 1; ++i) 
                    {
                        const std::string& key = parts[i];
                        if (!current->is_object() || !current->contains(key)) 
                        {
                            (*current)[key] = json::object();
                        }
                        current = &current->at(key);
                    }
        
                    (*current)[parts.back()] = *value;
                }
            }
        }
        
        // Если нет изменений — не создаём файл
        if (delta.empty()) 
        {
            ENGINE_LOG_INFO("No config changes to save");
            return true;
        }

        std::string content = delta.dump(4);   // Готовим данные к отправке

        if (FileIO::WriteTextFile(Engine::FileIO::GetProjectDirectory() + InTypeConfig, content)) 
        {
            ENGINE_LOG_INFO("Config saved: {} ({} overrides)", Engine::FileIO::GetProjectDirectory() + InTypeConfig, m_overriddenKeys.size());
            return true;
        }

        ENGINE_LOG_ERROR("Error saving user config: {}", Engine::FileIO::GetProjectDirectory() + InTypeConfig);
        return false;
    }

    size_t ConfigSystem::CountKeys(const json &j, size_t depth)         // Считает количество листовых ключей
    {
        if (!j.is_object()) return 1;
        
        size_t count = 0;
        for (auto& [key, value] : j.items()) 
        {
            count += CountKeys(value, depth + 1);
        }
        return count;
    }

    const json *ConfigSystem::GetNestedValue(const std::string &path) const
    {
        auto parts = SplitPath(path);
        if (parts.empty()) return nullptr;
    
        const json* current = &m_data;
    
        for (size_t i = 0; i < parts.size(); ++i) 
        {
            const std::string& key = parts[i];
        
            if (!current->is_object() || !current->contains(key)) 
            {
                return nullptr;  // Ключ не найден или не объект
            }
        
            current = &current->at(key);
        
            // Если это не последний элемент, но значение не объект — ошибка навигации
            if (i < parts.size() - 1 && !current->is_object()) 
            {
                return nullptr;
            }
        }
    
        return current;
    }

    std::vector<std::string> ConfigSystem::SplitPath(const std::string &path)
    {
        std::vector<std::string> parts;
        std::string normalized = path;
    
        size_t start = 0;
        while (start < normalized.size()) 
        {
            size_t end = normalized.find('.', start);
            if (end == std::string::npos) 
            {
                parts.push_back(normalized.substr(start));
                break;
            }
            if (end > start)                                            // Избегаем пустых частей при ".."
            {
                parts.push_back(normalized.substr(start, end - start));
            }
            start = end + 1;
        }
    
        return parts;
    }
}