// (c) Nikita Rogalev. All rights reserved.

#pragma once    // Multiple inclusion guard

#include <string>               
#include <filesystem>           // For filesystem path operations (C++17)
#include <unordered_map>        // (not used yet, possibly for future caching)
#include <memory>               // (not used yet)

/**
 *  ResourceManager manages paths to application resources (shaders, textures, models, etc.).
 *  Any interaction with external resources should go through this class
 *  or through related wrappers.
 *  
 *  Implemented as a singleton (guarantees a single instance).
 */

namespace Engine 
{

    namespace fs = std::filesystem;         // For convenience
    class ResourceManager 
    {
    public:
        static ResourceManager& getInstance();                                  // Get the single manager instance (singleton)
        void setBasePath(const std::string& path) {m_basePath = path;};         // Set base resource path manually (if override needed)
        static std::string getResourcePath(const std::string& relativePath);     // Get full resource path by combining base path with relative path
        static std::string getBasePath();                                        // Get current base path

        void init(const std::string& executablePath);                           // Initialize manager (called once at application startup).
    private:
        ResourceManager() = default;                                            // Private constructor (singleton)
        ~ResourceManager() = default;                                           // Private destructor (singleton)
        ResourceManager(const ResourceManager&) = delete;                       // Copy forbidden
        ResourceManager& operator=(const ResourceManager&) = delete;
        std::string m_basePath;                                                 // Stores base path relative to which resources are searched
        std::string determineBasePath(const std::string& executablePath);       // Helper method to automatically determine base path based on executable location.
    };

}
