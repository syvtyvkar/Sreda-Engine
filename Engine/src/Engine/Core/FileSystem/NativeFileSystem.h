// (c) Nikita Rogalev. All rights reserved.

#pragma once    // Multiple inclusion guard

#include "IVirtualFileSystem.h"

#include <fstream>
#include <map>
#include <mutex>
#include <string>

namespace Engine 
{
    /**
     * @brief Native File
     * 
     * 
     */
    class NativeFile : public IFile
    {
    public:
        NativeFile(std::string InName, std::ifstream InStream, size_t InSize) : m_name(InName), m_stream(std::move(InStream)), m_size(InSize), m_pos(0) {};
        virtual ~NativeFile() = default;

        virtual size_t Read(void* InBuffer, size_t InSize) override
        {
            m_stream.seekg(m_pos);
            m_stream.read(static_cast<char*>(InBuffer), InSize);
            size_t read = m_stream.gcount();
            m_pos += read;
            return read;
        };

        virtual size_t Size() const override {return m_size;}

        virtual void Seek(size_t InOffset) override 
        {
            m_pos = InOffset;
            if (m_pos > m_size) m_pos = m_size;
        }; 

        virtual size_t Tell() const override {return m_pos;}
        virtual std::string GetName() const override {return m_name;}
    private:
        std::ifstream m_stream; // File streaming
        size_t m_size;          // File Size
        size_t m_pos;           // Cursor position (offset)
        std::string m_name;     // Name asset
    };

    /**
     * @brief Native File System
     * 
     * 
     */
    class NativeFileSystem : public IVirtualFileSystem
    {
    public:
        NativeFileSystem() = default;
        virtual ~NativeFileSystem() = default;

        virtual TUniquePtr<IFile> Open(const std::string& InVirtualPath) override
        {
            std::string LNormalPath = NormalizeVirtualPath(InVirtualPath);
            std::lock_guard<std::mutex> lock(m_mutex);
            for (const auto& entry : m_mountPoints)
            {
                if (LNormalPath.find(entry.MountPoint) == 0)
                {
                    // Calculating the physical path
                    std::string LRelative = LNormalPath.substr(entry.MountPoint.size());
                    if (!LRelative.empty() && LRelative.front() == '/') LRelative.erase(0,1);
                    std::filesystem::path LFullPath = std::filesystem::path(entry.PhysPath) / LRelative;
                    LFullPath = LFullPath.lexically_normal();

                    if (!IsPathInside(entry.PhysPath, LFullPath.string())) continue;

                    if (std::filesystem::exists(LFullPath) && std::filesystem::is_regular_file(LFullPath))
                    {
                        std::ifstream LStream(LFullPath, std::ios::binary | std::ios::ate);
                        if (!LStream.is_open()) continue;
                        size_t LSize = LStream.tellg();
                        LStream.seekg(0);
                        return std::make_unique<NativeFile>(LNormalPath, std::move(LStream), LSize);
                    }
                }
            }
            return nullptr;
        };

        virtual bool Exists(const std::string& InVirtualPath) 
        {
            std::string LNormalPath = NormalizeVirtualPath(InVirtualPath);
            std::lock_guard<std::mutex> lock(m_mutex);
            for (const auto& entry : m_mountPoints)
            {
                if (LNormalPath.find(entry.MountPoint) == 0)
                {
                    std::string LRelative = LNormalPath.substr(entry.MountPoint.size());
                    if (!LRelative.empty() && LRelative.front() == '/') LRelative.erase(0,1);
                    std::filesystem::path LFullPath = std::filesystem::path(entry.PhysPath) / LRelative;
                    LFullPath = LFullPath.lexically_normal();
                    if (!IsPathInside(entry.PhysPath, LFullPath.string())) continue;
                    return std::filesystem::exists(LFullPath);
                }
            }
            return false;
        };   
                                            
        virtual void Mount(const std::string& InPhysPath,const std::string& InMountPoint,int InPriority = 0) 
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            std::filesystem::path LPhys(InPhysPath);
            LPhys = std::filesystem::absolute(LPhys).lexically_normal();
            ENGINE_ASSERT(!std::filesystem::is_directory(LPhys), "Error Native File System: mount not a directory!");
            std::string LNormalMount = NormalizeVirtualPath(InMountPoint);
            // Delete a previous mount with the same priority
            m_mountPoints.push_back({LNormalMount, LPhys.string(),InPriority});
            // LNormalMount
            std::sort(m_mountPoints.begin(),m_mountPoints.end(), [](const MountEntry& a, const MountEntry& b) {return a.Priority > b.Priority;});
        };  
                                          
        virtual void UnMount(std::string& InMountPoint) 
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            std::string LNormal = NormalizeVirtualPath(InMountPoint);
            m_mountPoints.erase(std::remove_if(m_mountPoints.begin(),m_mountPoints.end(), [&](const MountEntry& e) {return e.MountPoint==LNormal;}), m_mountPoints.end());
        };
        
    private:
        struct MountEntry 
        {
            std::string MountPoint;     //  Normalized virtual path
            std::string PhysPath;       //  The physical path
            int Priority;
        };
        std::vector<MountEntry> m_mountPoints;
        std::mutex m_mutex;

        /**
        *   Normalization of paths to the virtual file system format */
        static std::string NormalizeVirtualPath(const std::string& InPath)
        {
            std::string LResult = InPath;
            std::replace(LResult.begin(), LResult.end(), "\\", "/");
        #ifdef _WIN32
            // Set lowercase
            std::transform(LResult.begin(), LResult.end(),LResult.begin(), ::tolower);
        #endif
            auto new_end = std::unique(LResult.begin(), LResult.end(),[](char a, char b) { return a== '/' && b == '/';});
            LResult.erase(new_end, LResult.end());
            if (LResult.size() > 1 && LResult.back() == '/')
                LResult.pop_back();
            return LResult;
        };

        /**
        *   We check that we have not gone beyond the limits of the mount space */
        static bool IsPathInside(const std::string& InBase, const std::string& InTarget)
        {
            std::filesystem::path LBasePath = std::filesystem::absolute(InBase).lexically_normal();
            std::filesystem::path LTargetPath = std::filesystem::absolute(InTarget).lexically_normal();
            auto [BaseBegin, BaseEnd] = std::make_pair(LBasePath.begin(), LBasePath.end());
            auto [TargetBegin, TargetEnd] = std::make_pair(LTargetPath.begin(), LTargetPath.end());
            if (std::distance(BaseBegin, BaseEnd) > std::distance(TargetBegin, TargetEnd)) return false;
            return std::equal(BaseBegin, BaseEnd, TargetBegin);
        };
    };
}