// (c) Nikita Rogalev. All rights reserved.

#pragma once    // Защита от множественного включения

#include <string>               
#include <filesystem>           // Для работы с путями файловой системы (C++17)
#include <unordered_map>        // (пока не используется, возможно для будущего кэширования)
#include <memory>               // (пока не используется)

/**
 *  Класс ResourceManager управляет путями к ресурсам приложения (шейдеры, текстуры, модели и т.д.).
 *  Любое взаимодействие с внешними ресурсами должно протекать через этот класс,
 *  либо через связанные с ним оболочки.
 *  
 *  Реализован как синглтон (гарантирует единственный экземпляр).
 */

namespace Engine 
{

    namespace fs = std::filesystem;         // Для удобства использования
    class ResourceManager 
    {
    public:
        static ResourceManager& getInstance();                                  // Получить единственный экземпляр менеджера (синглтон)
        void setBasePath(const std::string& path) {m_basePath = path;};         // Установить базовый путь к ресурсам вручную (если нужно переопределить)
        std::string getResourcePath(const std::string& relativePath) const;     // Получить полный путь к ресурсу, объединяя базовый путь с относительным
        const std::string& getBasePath() const { return m_basePath; }           // Получить текущий базовый путь
        void init(const std::string& executablePath);                           // Инициализация менеджера (вызывается один раз при старте приложения).
    private:
        ResourceManager() = default;                                            // Приватный конструктор (синглтон)
        ~ResourceManager() = default;                                           // Приватный деструктор (синглтон)
        ResourceManager(const ResourceManager&) = delete;                       // Запрет копирования
        ResourceManager& operator=(const ResourceManager&) = delete;
        std::string m_basePath;                                                 // Хранит базовый путь, относительно которого ищутся ресурсы
        std::string determineBasePath(const std::string& executablePath);       // Вспомогательный метод для автоматического определения базового пути на основе местоположения исполняемого файла.
    };

}