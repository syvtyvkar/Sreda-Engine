// (c) Nikita Rogalev. All rights reserved.

#include "PlatformUtils.h"                          // Header
#include <filesystem>                               // For std::filesystem (C++17) — used in getCurrentWorkingDirectory
#include <iostream>

// Platform-specific headers
#ifdef _WIN32
    #include <windows.h>
#elif defined(__linux__) || defined(__APPLE__)
    #include <unistd.h>
    #include <limits.h>
#endif

namespace Engine 
{
    /**
     * @brief Returns the full path to the executable file of the current process.
     * 
     * Implementation depends on the operating system:
     * - Windows: uses GetModuleFileNameA.
     * - Linux: reads the symbolic link /proc/self/exe.
     * - macOS: uses _NSGetExecutablePath.
     * 
     * @return std::string Absolute path to the executable.
     */
    std::string PlatformUtils::GetExecutablePath() 
    {
        char path[4096];                                                    // Buffer for the path (large enough for most systems)

        #ifdef _WIN32                                           
            GetModuleFileNameA(NULL, path, sizeof(path));                   // Windows: GetModuleFileNameA(NULL, ...) returns path to the current module (exe). 
        #elif defined(__linux__)
            ssize_t count = readlink("/proc/self/exe", path, sizeof(path)); // Linux: read symbolic link /proc/self/exe, which points to the executable.
            if (count != -1) {
                path[count] = '\0';                                         // Null-terminate the string
            }
        #elif defined(__APPLE__)
            #include <mach-o/dyld.h>                                        // macOS: _NSGetExecutablePath places the path in the buffer, size passed by reference.
            uint32_t size = sizeof(path);
            _NSGetExecutablePath(path, &size);
        #endif

        return std::string(path);                                           // Convert C-string to std::string
    }

    /**
     * @brief Returns the current working directory of the process.
     * 
     * Uses std::filesystem::current_path() (C++17), which is cross-platform.
     * 
     * @return std::string Absolute path to the current working directory.
     */
    std::string PlatformUtils::GetBinaryDirectory()
    {
         return std::filesystem::current_path().string();
    }

    /**
     * @brief Returns the project path, considering the executable is located in /Bin
     */
    std::string PlatformUtils::GetProjectDirectory()
    {
        std::filesystem::path projectPath = std::filesystem::current_path().parent_path();
        return projectPath.string();
    }
}
