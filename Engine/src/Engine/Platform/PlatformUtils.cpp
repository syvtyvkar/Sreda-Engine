// (c) Nikita Rogalev. All rights reserved.

#include "PlatformUtils.h"                          // Заголовок
#include <filesystem>                               // Для std::filesystem (C++17) — используется в getCurrentWorkingDirectory
#include <iostream>

// Платформозависимые заголовки
#ifdef _WIN32
    #include <windows.h>
#elif defined(__linux__) || defined(__APPLE__)
    #include <unistd.h>
    #include <limits.h>
#endif

namespace Engine 
{
    /**
     * @brief Возвращает полный путь к исполняемому файлу текущего процесса.
     * 
     * Реализация зависит от операционной системы:
     * - Windows: используется GetModuleFileNameA.
     * - Linux: читается символьная ссылка /proc/self/exe.
     * - macOS: используется _NSGetExecutablePath.
     * 
     * @return std::string Абсолютный путь к исполняемому файлу.
     */
    std::string PlatformUtils::getExecutablePath() 
    {
        char path[4096];                                                    // Буфер для пути (достаточно большой для большинства систем)

        #ifdef _WIN32                                           
            GetModuleFileNameA(NULL, path, sizeof(path));                   // Windows: GetModuleFileNameA(NULL, ...) возвращает путь к текущему модулю (exe). 
        #elif defined(__linux__)
            ssize_t count = readlink("/proc/self/exe", path, sizeof(path)); // Linux: читаем символьную ссылку /proc/self/exe, которая указывает на исполняемый файл.
            if (count != -1) {
                path[count] = '\0';                                         // Завершаем строку нулём
            }
        #elif defined(__APPLE__)
            #include <mach-o/dyld.h>                                        // macOS: _NSGetExecutablePath помещает путь в буфер, размер передаётся по ссылке.
            uint32_t size = sizeof(path);
            _NSGetExecutablePath(path, &size);
        #endif

        return std::string(path);                                           // Преобразуем C-строку в std::string
    }

    /**
     * @brief Возвращает текущую рабочую директорию процесса.
     * 
     * Использует std::filesystem::current_path() (C++17), что кроссплатформенно.
     * 
     * @return std::string Абсолютный путь к текущей рабочей директории.
     */
    std::string PlatformUtils::getCurrentWorkingDirectory() 
    {
        return std::filesystem::current_path().string();
    }

}