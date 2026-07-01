// (c) Nikita Rogalev. All rights reserved.

#include "ResourceManager.h"
#include <iostream>
#include "BaseEngine.h"

#ifdef _WIN32                                   // Platform-specific headers for path information
    #include <windows.h>                        // For Windows API (may be needed for path operations)
#elif defined(__linux__) || defined(__APPLE__)
    #include <unistd.h>                         // For POSIX (readlink, etc.)
    #include <limits.h>                         // For PATH_MAX
#endif

#include <fstream>

namespace Engine {

    ResourceManager& ResourceManager::getInstance()     // Singleton implementation: static local object created on first call
    {
        static ResourceManager instance;
        return instance;
    }

    void ResourceManager::init(const std::string& executablePath)   // Initialize resource manager: saves base path determined from executable path
    {
        m_basePath = determineBasePath(executablePath);
        ENGINE_LOG_INFO("Resource Manager initialized. Base path: ");
    }

    std::string ResourceManager::getResourcePath(const std::string& relativePath) // Returns full resource path by appending relative path to base directory
    {
        auto& lInst = ResourceManager::getInstance();
        return (fs::path(lInst.m_basePath) / relativePath).string();
    }

    std::string ResourceManager::getBasePath()
    {
        auto& lInst = ResourceManager::getInstance();
        return lInst.m_basePath;
    }

    /*Determines the base resource directory by analyzing the executable path
        Looks for a "Resources" folder next to the executable or one level up (for debug builds)*/
    std::string ResourceManager::determineBasePath(const std::string& executablePath) 
    {
        auto& lInst = ResourceManager::getInstance();
        fs::path exePath(executablePath);           // Convert executable path to filesystem::path object
        fs::path baseDir = exePath.parent_path();   // Directory where the exe is located

        if (fs::exists(baseDir / "Resources/")) // Check if "Resources" subdirectory exists in the same folder
        {   
            return baseDir.string();            // If yes, base path is the current directory
        }

        fs::path parentDir = baseDir.parent_path(); // If not, try going one level up (e.g., from Debug/Release folder)
        if (fs::exists(parentDir / "Resources/")) 
        {
            return parentDir.string();              // Base path is the parent directory
        }

        return baseDir.string();                    // If nothing found, return executable directory as fallback
    }
}
