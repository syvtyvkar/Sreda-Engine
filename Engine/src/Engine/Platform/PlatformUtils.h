// (c) Nikita Rogalev. All rights reserved.

#pragma once                // Защита от множественного включения заголовочного файла

#include <string>           // Для использования типа std::string

namespace Engine 
{
    /**
     * @class PlatformUtils
     * @brief Утилиты для получения информации о платформе и файловой системе.
     *
     * Предоставляет статические методы для получения пути к исполняемому файлу
     * и текущей рабочей директории. Реализация методов зависит от операционной системы
     * (Windows, Linux, macOS) и скрыта в соответствующем .cpp файле.
     */
    class PlatformUtils 
    {
    public:
        /**
         * @brief Возвращает полный абсолютный путь к исполняемому файлу текущего процесса.
         * @return Строка, содержащая путь к исполняемому файлу.
         *
         * На Windows использует GetModuleFileNameA, на Linux — readlink("/proc/self/exe"),
         * на macOS — _NSGetExecutablePath.
         */
        static std::string getExecutablePath();

        /**
         * @brief Возвращает текущую рабочую директорию процесса.
         * @return Строка, содержащая абсолютный путь к рабочей директории.
         *
         * На Windows использует GetCurrentDirectory, на Unix-подобных — getcwd.
         */
        static std::string getCurrentWorkingDirectory();
    };
}