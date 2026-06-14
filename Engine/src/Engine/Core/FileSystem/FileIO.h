// (c) Nikita Rogalev. All rights reserved.

#pragma once    // Multiple inclusion guard

#include <string>           //
#include <vector>           //
#include <optional>         //
#include <filesystem>       //
#include "Engine/Platform/PlatformUtils.h"

namespace Engine 
{
    /**
     * @brief File system utilities
     * 
     * Handles ONLY reading/writing data.
     * All methods are static — no state required.
     */
    class FileIO
    {
    public:
        // Text data
        static std::string ReadTextDataToString(const std::string& InPath);                                                     // Read text file
        static std::optional<std::string> ReadTextFile(const std::string& InPath);                                              // Read text file
        static bool WriteTextFile(const std::string& InPath, const std::string& InContent, bool InCreateDirs = true);           // Write text file
        // Binary data
        static std::optional<std::vector<uint8_t>> ReadBinaryFile(const std::string& InPath);                                   // Read binary file
        static bool WriteBinaryFile(const std::string& InPath, const std::vector<uint8_t>& InData, bool InCreateDirs = true);   // Write binary file
        // Path operations
        static bool IsFileExist(const std::string& InPath);                                                         // Check if file exists
        static bool IsProjectFileExist(const std::string& InFile);                                                  // Check if project file exists
        static std::string GetExtensionFile(const std::string& InPath);                                             // Get file extension
        static std::string GetFileName(const std::string& InPath);                                                  // Get file name without path and extension
        static std::string GetDirectoryFile(const std::string& InPath);                                             // Get file directory
        static bool CreateDirectories(const std::string& InPath);                                                   // Create directory
        static std::string Join(const std::string& InBase, const std::string& InRelative);                          // Join paths platform-independently
        //static std::string GetBinaryDirectory();                                                                    // Path to executable
        //static std::string GetProjectDirectory();                                                                   // Path to application working directory
    private:
        FileIO() = delete;                                                                                          // Forbidden to create instances
        ~FileIO() = delete;                                                                                         // Forbidden to destroy instances
    };
}
