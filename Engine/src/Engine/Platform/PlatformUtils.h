// (c) Nikita Rogalev. All rights reserved.

#pragma once                // Multiple inclusion guard for header file

#include <string>           // For std::string type

namespace Engine 
{
    /**
     * @class PlatformUtils
     * @brief Utilities for obtaining platform and file system information.
     *
     * Provides static methods for getting the executable path
     * and current working directory. Method implementations depend on the operating system
     * (Windows, Linux, macOS) and are hidden in the corresponding .cpp file.
     */
    class PlatformUtils 
    {
    public:
        /**
         * @brief Returns the full absolute path to the current process executable.
         * @return String containing the path to the executable.
         *
         * On Windows uses GetModuleFileNameA, on Linux — readlink("/proc/self/exe"),
         * on macOS — _NSGetExecutablePath.
         */
        static std::string GetExecutablePath();

        /**
         * @brief Returns the current working directory of the process.
         * @return String containing the absolute path to the working directory.
         *
         * On Windows uses GetCurrentDirectory, on Unix-like — getcwd.
         */
        static std::string GetBinaryDirectory();

        /**
         * @brief Path to the application working directory
         */
        static std::string GetProjectDirectory(); 
    };
}
