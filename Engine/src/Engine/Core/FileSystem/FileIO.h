// (c) Nikita Rogalev. All rights reserved.

#pragma once    // Защита от множественного включения

#include <string>           //
#include <vector>           //
#include <optional>         //
#include <filesystem>       //

namespace Engine 
{
    /**
     * @brief Утилиты для работы с файловой системой
     * 
     * Отвечает ТОЛЬКО за чтение/запись данных.
     * Все методы статические — состояние не требуется.
     */
    class FileIO
    {
    public:
        // Текстовые данные
        static std::optional<std::string> ReadTextFile(const std::string& InPath);                                              // Прочитать текстовый файл
        static bool WriteTextFile(const std::string& InPath, const std::string& InContent, bool InCreateDirs = true);           // Записать текстовый файл
        // Бинарные данные
        static std::optional<std::vector<uint8_t>> ReadBinaryFile(const std::string& InPath);                                   // Прочитать бинарный файл
        static bool WriteBinaryFile(const std::string& InPath, const std::vector<uint8_t>& InData, bool InCreateDirs = true);   // Записать бинарный файл
        // Работа с путями
        static bool IsFileExist(const std::string& InPath);                                                         // Проверить, что файл существует
        static bool IsProjectFileExist(const std::string& InFile);                                                  // Проверить, что файл проекта существует
        static std::string GetExtensionFile(const std::string& InPath);                                             // Получить расширение файла
        static std::string GetFileName(const std::string& InPath);                                                  // Получить имя файла без пути и расширения
        static std::string GetDirectoryFile(const std::string& InPath);                                             // Получить директорию файла
        static bool CreateDirectories(const std::string& InPath);                                                   // Создать директорию
        static std::string Join(const std::string& InBase, const std::string& InRelative);                          // Объединить пути платформо-независимо
        static std::string GetProjectDirectory();                                                                   // Путь к рабочей директории приложения
    private:
        FileIO() = delete;                                                                                          // Запрещено создавать экземпляры класса
        ~FileIO() = delete;                                                                                         // Запрещено удалять экземпляры класса
    };
}