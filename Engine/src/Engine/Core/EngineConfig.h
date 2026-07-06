// (c) Nikita Rogalev. All rights reserved.

#pragma once                            // Multiple inclusion guard for header file
#include <iostream>                     //

//nlohmann-json
#include <nlohmann/json.hpp>            // JSON library
#include <string>                       // 
#include <unordered_map>                // 
#include <functional>                   // 
#include <mutex>                        // 

#ifdef ENGINE_CONFIG_EMBEDDED
#include "config_embed.h"               // Generated header
#endif

#include <algorithm>                    //
#include <unordered_set>                //
///////////////

namespace EngineConfig 
{
    using json = nlohmann::json;
    using namespace Engine;
    using ChangeCallback = std::function<void(const std::string& key, const json& newValue)>;


    const std::string ENINGE_CONFIG_FILE = "/Config/engine.json";

    /**
     * Config system
     * 
     * Wrapper over nlohmann library for working with configuration. Engine settings, game settings, etc.
     * 
     * Designed for maximum convenience. The system can store default "out-of-the-box" settings 
     * in a generated file, which the user can then override in config files.
     * 
     * This is a low-level class. It is expected that only high-level wrappers and the base 
     * application (for initial loading) will work with it.
     */
    class ConfigSystem 
    {
    public:
        static ConfigSystem* Get();                                                     // Singleton

        // Template functions
        template<typename T>
        T Get(const std::string& key, const T& defaultValue) const;
        
        template<typename T>
        void Set(const std::string& key, const T& value);

        bool Init(std::string InTypeConfig);                                            // Initialization and initial file loading.
        bool DeInit();                                                                  // Call at program exit or before reinitialization. Clears data

        void MarkOverridden(const std::string& key);                                    // Mark key as overridden
        bool SaveConfig(const std::string& InTypeConfig);
        ConfigSystem()= default;                                                        // Constructor
        ~ConfigSystem() = default;                                                      // Default destructor
        ConfigSystem(const ConfigSystem&) = delete;                                     // Two systems cannot coexist, so copy is forbidden
        ConfigSystem& operator=(const ConfigSystem&) = delete;                          // Two systems cannot coexist, so copy is forbidden
    private: 
        void MarkOverriddenKeys(const json& InConfig, const std::string& InPrefix);     // Mark file keys as overridden
        size_t CountKeys(const json& j, size_t depth = 0);                              // How many keys?
        const json* GetNestedValue(const std::string& path) const;                      // Get pointer to value
        static std::vector<std::string> SplitPath(const std::string& path);             // Split path "a/b/c" into parts ["a", "b", "c"]

        //static ConfigSystem* s_instance;                                                // Singleton
        bool m_embeddedLoaded = false;                                                  // Whether generated files have been loaded
        json m_data;                                                                    // Current JSON config with data
        std::unordered_set<std::string> m_overriddenKeys;                               // User keys overriding defaults
        mutable std::mutex m_mutex;                                                     // For thread safety
    };

    template<typename T>
    T ConfigSystem::Get(const std::string& key, const T& defaultValue) const 
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        const json* value = GetNestedValue(key);
        if (!value) 
        {
            return defaultValue;
        }
        try 
        {
            return value->get<T>();
        } 
        catch (...) 
        {
            // If type doesn't match — return default
            return defaultValue;
        }
    }

    template<typename T>
    void ConfigSystem::Set(const std::string& key, const T& value) 
    {
        json jsonValue;
    
        // Convert value to json (with special type handling)
        try 
        {
            jsonValue = json(value);
        }   
        catch (...) 
        {
            // If conversion fails, try manually for common cases
            if constexpr (std::is_same_v<T, glm::vec2>) 
            {
                jsonValue = {value.x, value.y};
            } 
            else if constexpr (std::is_same_v<T, glm::vec3>) 
            {
                jsonValue = {value.x, value.y, value.z};
            } 
            else if constexpr (std::is_same_v<T, glm::vec4>) 
            {
                jsonValue = {value.x, value.y, value.z, value.w};
            } 
            else 
            {
                ENGINE_LOG_ERROR("Failed to convert value to JSON for key: {}", key);
                return;
            }
        }

        std::string normalizedKey;
        {
            std::lock_guard<std::mutex> lock(m_mutex);
        
            normalizedKey = key;  // Use original key for subscribers
            //SetNestedValue(key, jsonValue);

            auto parts = SplitPath(key);
            if (parts.empty()) return;
    
            json* current = &m_data;
    
            for (size_t i = 0; i < parts.size() - 1; ++i) 
            {
                const std::string& key = parts[i];
        
                // If key doesn't exist or isn't an object — create an object
                if (!current->is_object() || !current->contains(key)) 
                {
                    (*current)[key] = json::object();
                }
        
                current = &current->at(key);
            }
    
            // Set final value
            const std::string& lastKey = parts.back();
            (*current)[lastKey] = value;

        }
        ENGINE_LOG_TRACE("Change config key: {}, value: {}", key, value);
        MarkOverridden(key);
    }
}
