#include "EngineConfig.h"
#include "BaseEngine.h"
//#include <fstream>
//#include <filesystem>

#ifdef ENGINE_CONFIG_EMBEDDED
    #include "config_embed.h"  // Generated header
#endif

#include <fstream>
#include <algorithm>

#include "Engine/Core/Base/EngineCore.h"

namespace EngineConfig 
{
    //ConfigSystem* ConfigSystem::s_instance = nullptr;
    using namespace Engine;

    /**
     * @brief Get singleton
     */
    ConfigSystem* ConfigSystem::Get() 
    {
        auto& ctx = EngineCore::GetEngineContext();
        return ctx.GetConfigSystem();
    }

    bool ConfigSystem::Init(std::string InTypeConfig)  // Initialize our configs
    {
        ENGINE_LOG_INFO("Init configurate file: {}", PlatformUtils::GetProjectDirectory() + InTypeConfig); 
        if (!m_embeddedLoaded)   // Have we already tried to load the generated config file?
        {
#ifdef ENGINE_CONFIG_EMBEDDED
            std::lock_guard<std::mutex> lock(m_mutex);              // Nobody can touch the file while we're working with it!
            try 
            {
                // Parse embedded JSON string
                m_data = json::parse(ConfigEmbed::DefaultConfigJSON);                               // Parse data
                m_embeddedLoaded = true;                                                            // Signal that no more default data loading is needed
                ENGINE_LOG_INFO("Embedded config loaded ({} bytes)", strlen(ConfigEmbed::DefaultConfigJSON));           // Log
            } 
            catch (const json::parse_error& e)                                                                          // Oops, something went wrong!
            {
                ENGINE_LOG_ERROR("Failed to parse embedded config: {}", e.what()); m_data = json::object();  // Empty object as fallback
            }
#else
            ENGINE_LOG_ERROR("Embedded config not available (ENGINE_CONFIG_EMBEDDED not defined)");  // If we got here - there's a problem with file generation. Maybe CMake?
            return false;
#endif
        }
        if (Engine::FileIO::IsProjectFileExist(InTypeConfig))                                        // Does the file exist?
        {
            try 
            {
                //std::ifstream File;
                auto lFileContent = Engine::FileIO::ReadTextFile(PlatformUtils::GetProjectDirectory() + InTypeConfig);                                 
                if (!lFileContent->empty()) 
                {
                    json userConfig = json::parse(lFileContent.value());              // Parse data from user configs
                    m_data.merge_patch(userConfig);                                   // Merge data from user configs
                    MarkOverriddenKeys(userConfig,"");                                // Mark changed keys (simplified: all keys from userConfig)
                    
                    ENGINE_LOG_INFO("User config merged: {} overrides applied", CountKeys(userConfig));
                    return true;
                }
            } 
            catch (const std::exception& e) 
            {
                ENGINE_LOG_WARN("Failed to load user config {}: {}", PlatformUtils::GetProjectDirectory() + InTypeConfig, e.what());
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
        //s_instance = nullptr;
        ENGINE_LOG_INFO("Stop configurate system"); 
        return true;
    }

    void ConfigSystem::MarkOverriddenKeys(const json &InConfig, const std::string &InPrefix)    // Recursively mark all keys as "overridden"
    {
        for (auto& [key, value] : InConfig.items()) 
        {
            std::string fullPath = InPrefix.empty() ? key : InPrefix + "." + key;                   // Path to target
            if (value.is_object()) 
            {
                MarkOverriddenKeys(value, fullPath);        // Recursion for nested objects
            } 
            else 
            {
                m_overriddenKeys.insert(fullPath);          // Mark as overridden
            }
        }
    }

    void ConfigSystem::MarkOverridden(const std::string &key)           // Mark key as overridden
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_overriddenKeys.insert(key);
    }

    bool ConfigSystem::SaveConfig(const std::string &InTypeConfig)        // Save configs of a specific type
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
        
        // If no changes — don't create a file
        if (delta.empty()) 
        {
            ENGINE_LOG_INFO("No config changes to save");
            return true;
        }

        std::string content = delta.dump(4);   // Prepare data for writing

        if (FileIO::WriteTextFile(PlatformUtils::GetProjectDirectory() + InTypeConfig, content)) 
        {
            ENGINE_LOG_INFO("Config saved: {} ({} overrides)", PlatformUtils::GetProjectDirectory() + InTypeConfig, m_overriddenKeys.size());
            return true;
        }

        ENGINE_LOG_ERROR("Error saving user config: {}", PlatformUtils::GetProjectDirectory() + InTypeConfig);
        return false;
    }

    size_t ConfigSystem::CountKeys(const json &j, size_t depth)         // Count leaf keys
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
                return nullptr;  // Key not found or not an object
            }
        
            current = &current->at(key);
        
            // If this is not the last element but the value is not an object — navigation error
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
            if (end > start)                                            // Avoid empty parts with ".."
            {
                parts.push_back(normalized.substr(start, end - start));
            }
            start = end + 1;
        }
    
        return parts;
    }
}
