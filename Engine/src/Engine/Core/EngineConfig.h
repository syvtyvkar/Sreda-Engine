// (c) Nikita Rogalev. All rights reserved.

#pragma once                            // Защита от множественного включения заголовочного файла
#include <iostream>                     //

//nlohmann-json
#include <nlohmann/json.hpp>            // Библиотека для работы с JSON
#include <string>                       // 
#include <unordered_map>                // 
#include <functional>                   // 
#include <mutex>                        // 

#ifdef ENGINE_CONFIG_EMBEDDED
#include "config_embed.h"               // Сгенерированный header
#endif

#include <fstream>                      //
#include <filesystem>                   //
#include <algorithm>                    //
#include <unordered_set>                //
///////////////

namespace EngineConfig 
{
    using json = nlohmann::json;
    using namespace Engine;
    using ChangeCallback = std::function<void(const std::string& key, const json& newValue)>;

    /**
     * Система конфигов
     * 
     * Обертка над библиотекой nlohmann для работы с конфигурацией. Настройки движка, игры и так далее.
     * 
     * Написано так, чтобы работать с этим было максимально удобно. Система умеет хранить в сгенерированном 
     * файле базовые настройки "из коробки", которые затем пользователь переопределяет в конфигурационных файлах.
     * 
     * Это низкоуровневый класс. Предполагается, что с ним могут работать только высокоуровневые обертки и базовое 
     * приложение (для первичной загрузки)
     */
    class ConfigSystem 
    {
    public:
        static ConfigSystem& Get();                                                     // Синглтон
        bool Init(std::string InBasePath);                                              // Инициализация, и первичная загрузка файлов.
        bool DeInit();                                                                  // Вызывать при завершении програмы или перед повторной инициализацией. Чистит данные
    private: 
        ConfigSystem()= default;                                                        // Конструктор
        ~ConfigSystem() = default;                                                      // Деструктор стандартный
        ConfigSystem(const ConfigSystem&) = delete;                                     // Не может существовать одновременно две системы, поэтому запрещаем копирование
        ConfigSystem& operator=(const ConfigSystem&) = delete;                          // Не может существовать одновременно две системы, поэтому запрещаем копирование

        static ConfigSystem* s_instance;                                                // Синглтон
        bool m_embeddedLoaded = false;                                                  // Были ли загружены сгенерированные файлы
        json m_data;
        std::unordered_set<std::string> m_overriddenKeys;
        std::string m_configPath;
        mutable std::mutex m_mutex;
        std::unordered_map<std::string, std::vector<ChangeCallback>> m_subscribers;
    };

}