// (c) Nikita Rogalev. All rights reserved.

#include "FileIO.h"
#include "Engine/Core/Log.h"
#include <fstream>
#include <sstream>

namespace Engine 
{
 
    std::optional<std::string> FileIO::ReadTextFile(const std::string& InPath) 
    {
        std::ifstream file(InPath, std::ios::binary | std::ios::ate);
        if (!file.is_open()) 
        {
            ENGINE_LOG_ERROR("Failed to open file for reading: {}", InPath);
            return std::nullopt;
        }
        
        // Получаем размер и читаем всё сразу
        std::streamsize size = file.tellg();
        file.seekg(0, std::ios::beg);
        
        std::string content(static_cast<size_t>(size), '\0');
        if (!file.read(content.data(), size)) 
        {
            ENGINE_LOG_ERROR("Failed to read file: {}", InPath);
            return std::nullopt;
        }
        
        return content;
    }

    bool FileIO::WriteTextFile(const std::string& InPath, const std::string& InContent, bool InCreateDirs) 
    {
        if (InCreateDirs) 
        {
            CreateDirectories(GetDirectoryFile(InPath));
        }
        
        std::ofstream file(InPath, std::ios::binary);
        if (!file.is_open()) 
        {
            ENGINE_LOG_ERROR("Failed to open file for writing: {}", InPath);
            return false;
        }
        
        file.write(InContent.c_str(), static_cast<std::streamsize>(InContent.size()));
        return file.good();
    }

    std::optional<std::vector<uint8_t>> FileIO::ReadBinaryFile(const std::string& InPath) 
    {
        std::ifstream file(InPath, std::ios::binary | std::ios::ate);
        if (!file.is_open()) return std::nullopt;
        
        std::streamsize size = file.tellg();
        file.seekg(0, std::ios::beg);
        
        std::vector<uint8_t> buffer(static_cast<size_t>(size));
        if (!file.read(reinterpret_cast<char*>(buffer.data()), size)) {
            return std::nullopt;
        }
        
        return buffer;
    }

    bool FileIO::WriteBinaryFile(const std::string& InPath, const std::vector<uint8_t>& InData, bool InCreateDirs) 
    {
        if (InCreateDirs) 
        {
            CreateDirectories(GetDirectoryFile(InPath));
        }
        
        std::ofstream file(InPath, std::ios::binary);
        if (!file.is_open()) return false;
        
        file.write(reinterpret_cast<const char*>(InData.data()), static_cast<std::streamsize>(InData.size()));
        return file.good();
    }

    bool FileIO::IsFileExist(const std::string& InPath) 
    {
        return std::filesystem::exists(InPath);
    }

    bool FileIO::IsProjectFileExist(const std::string& InFile) 
    {
        return std::filesystem::exists(GetProjectDirectory() + InFile);
    }

    std::string FileIO::GetExtensionFile(const std::string& InPath) 
    {
        return std::filesystem::path(InPath).extension().string();
    }

    std::string FileIO::GetFileName(const std::string& InPath) 
    {
        auto fsPath = std::filesystem::path(InPath);
        return fsPath.stem().string();
    }

    std::string FileIO::GetDirectoryFile(const std::string& InPath) 
    {
        auto parent = std::filesystem::path(InPath).parent_path();
        return parent.empty() ? "" : parent.string();
    }

    bool FileIO::CreateDirectories(const std::string& InPath) 
    {
        if (InPath.empty()) return true;
        
        std::error_code ec;
        bool created = std::filesystem::create_directories(InPath, ec);
        
        if (ec) 
        {
            ENGINE_LOG_WARN("Failed to create directories '{}': {}", InPath, ec.message());
            return false;
        }
        return created;
    }

    std::string FileIO::Join(const std::string& InBase, const std::string& InRelative) 
    {
        return (std::filesystem::path(InBase) / InRelative).string();
    }

    std::string FileIO::GetProjectDirectory() 
    {
        return std::filesystem::current_path().string();
    }
}