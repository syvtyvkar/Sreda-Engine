// (c) Nikita Rogalev. All rights reserved.

#include "ResourceManager.h"
#include <iostream>
#include "Engine/Core/Log.h"

#ifdef _WIN32                                   // Платформозависимые заголовки для получения информации о путях
    #include <windows.h>                        // Для Windows API (может понадобиться для работы с путями)
#elif defined(__linux__) || defined(__APPLE__)
    #include <unistd.h>                         // Для POSIX (readlink и т.д.)
    #include <limits.h>                         // Для PATH_MAX
#endif

namespace Engine {

    ResourceManager& ResourceManager::getInstance()     // Реализация синглтона: статический локальный объект создаётся при первом вызове
    {
        static ResourceManager instance;
        return instance;
    }

    void ResourceManager::init(const std::string& executablePath)   // Инициализация менеджера ресурсов: сохраняет базовый путь, определённый на основе пути к исполняемому файлу
    {
        m_basePath = determineBasePath(executablePath);
        ENGINE_LOG_INFO("Resource Manager initialized. Base path: ");
    }

    std::string ResourceManager::getResourcePath(const std::string& relativePath) const // Возвращает полный путь к ресурсу, добавляя относительный путь к базовому каталогу
    {
        return (fs::path(m_basePath) / relativePath).string();
    }

    /*Определяет базовый каталог для ресурсов, анализируя путь к исполняемому файлу
        Ищет папку "Resources" рядом с исполняемым файлом или на уровень выше (для отладочных сборок)*/
    std::string ResourceManager::determineBasePath(const std::string& executablePath) 
    {
        fs::path exePath(executablePath);           // Преобразуем путь к исполняемому файлу в объект filesystem::path
        fs::path baseDir = exePath.parent_path();   // Директория, где лежит exe

        if (fs::exists(baseDir / "Resources/")) // Проверяем, существует ли поддиректория "Resources" в той же папке
        {   
            return baseDir.string();            // Если да, то базовый путь — текущая директория
        }

        fs::path parentDir = baseDir.parent_path(); // Если нет, пробуем подняться на один уровень вверх (например, из папки Debug/Release)
        if (fs::exists(parentDir / "Resources/")) 
        {
            return parentDir.string();              // Базовый путь — родительская директория
        }

        return baseDir.string();                    // Если ничего не найдено, возвращаем директорию исполняемого файла как запасной вариант
    }
}