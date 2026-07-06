// (c) Nikita Rogalev. All rights reserved.

#pragma once    // Multiple inclusion guard

#include <string>           //
#include <vector>           //
#include <optional>         //
#include <filesystem>       //
#include "Engine/Platform/PlatformUtils.h"
#include "Engine/Render/Texture.h"

namespace Engine 
{
    /**
     * @brief IFile
     * 
     * 
     */
    class IFile
    {
    public:
        virtual ~IFile() = default;

        /**
        *   Read the file */
        virtual size_t Read(void* InBuffer, size_t InSize) = 0;
        /**
        *   Get the file size */
        virtual size_t Size() const = 0;
        /**
        *   Move the cursor (offset) to the specified value from the beginning */
        virtual void Seek(size_t InOffset) = 0; 
        /**
        *   Find out which position the cursor is at (Offset) */
        virtual size_t Tell() const = 0;

        virtual std::string GetName() const = 0;
    };

    /**
     * @brief Virtual File System
     * 
     * 
     */
    class IVirtualFileSystem
    {
    public:
        virtual ~IVirtualFileSystem() = default;

        /**
        *   Get the file on the way */
        virtual TUniquePtr<IFile> Open(const std::string& InVirtualPath) = 0;
        /**
        *   Verify that the file exists */
        virtual bool Exists(const std::string& InVirtualPath) = 0;   
        /**
        *   Mounts the source (folder/archive/file) to the file system. Depending on the priority, it may override files */                                             
        virtual void Mount(const std::string& InPhysPath,const std::string& InMountPoint,int InPriority = 0) = 0;  
        /**  
        *   Mounts the source (folder/archive/file) to the file system. Depending on the priority, it may override files */                                             
        virtual void UnMount(const std::string& InMountPoint) = 0;        
    };
}