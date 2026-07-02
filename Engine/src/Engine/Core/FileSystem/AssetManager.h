// (c) Nikita Rogalev. All rights reserved.

#pragma once    // Multiple inclusion guard

#include "IVirtualFileSystem.h"
#include "Engine/Core/Utilities/Event.h"

#include <fstream>
#include <map>
#include <mutex>
#include <string>

namespace Engine 
{
    /**
     * @brief IAsset
     * 
     * 
     */
    class IAsset
    {
        friend class AssetManager;
    public:
        virtual ~IAsset() = default;
        /**
        *   Get a virtual file path */
        std::string GetPath() const {return m_path;};
    protected:
        std::string m_path; // Virtual Path
    };

    ADD_DELEGATE_TWO_PARAMS(DOnFinishSyncLoad,TRef<IAsset>,std::string)

    /**
     * @brief IAssetLoader
     * 
     * 
     */
    class IAssetLoader
    {
    public:
        virtual ~IAssetLoader() = default;


        /**
        *   Load Asset */
        virtual TRef<IAsset> LoadStatic(const std::string& InPath, TUniquePtr<IFile> File) = 0;

        /**
        *   Start Sync Load Asset */
        //virtual bool LoadSync(std::string& InPath) = 0;

        /**
        *   Get type loader */
        std::vector<std::string> GetLoaderType() {return m_typeLoader; }

    protected:

        /**
        *   Finish Sync load */
        //virtual TRef<IAsset> FinishLoadSync() = 0;

        DOnFinishSyncLoad& OnFinishSyncLoad() {return m_DOnFinishSyncLoad;}

        std::vector<std::string> m_typeLoader= {};

    private:
        DOnFinishSyncLoad m_DOnFinishSyncLoad;
    };

    /**
     * @brief IFile
     * 
     * 
     */
    class AssetManager
    {
    public:
        AssetManager(IVirtualFileSystem* InVFS) : m_vfs(*InVFS) {};
        ~AssetManager() = default;

        void RegisterLoader(const std::string& InExtension, TUniquePtr<IAssetLoader> InLoader);

        template<typename T>
        TRef<T> Get(const std::string& InPath)
        {
            static_assert(std::is_base_of<IAsset, T>, "T must derive from Asset");
            
            std::string LNormalPath = Normalize(InPath);

            auto it = m_cashe.find(LNormalPath);
            if (it != m_cashe.end())
            {
                if (auto ptr = it->second.lock())
                {
                    return std::static_pointer_cast<T>(ptr);
                }else
                {
                    m_cashe.erase(it);
                }
            }

            TRef<IAsset> LAsset = CallLoadAssetStatic(LNormalPath);
            if (LAsset)
            {
                m_cashe[LNormalPath] = LAsset;
                return std::static_pointer_cast<T>(LAsset);
            }

            return nullptr;
        }

        void Unload(const std::string InPath);

        void GarbageCollect();
        
    private:
        IVirtualFileSystem& m_vfs;
        std::unordered_map<std::string, TUniquePtr<IAssetLoader>> m_loaders;
        std::unordered_map<std::string, TWeak<IAsset>> m_cashe;
        std::mutex m_mutex;

        TRef<IAsset> CallLoadAssetStatic(const std::string InNormalPath);
        //bool CallLoadAssetSync(const std::string InNormalPath, DOnFinishSyncLoad& InDelegate);
        static std::string Normalize(const std::string InPath)
        {
            //return NativeFileSystem::NormalizeVirtualPath(InPath);
            std::string LResult = InPath;
            std::replace(LResult.begin(), LResult.end(), '\\', '/');
            #ifdef _WIN32
                // Set lowercase
                std::transform(LResult.begin(), LResult.end(),LResult.begin(), ::tolower);
            #endif
                auto new_end = std::unique(LResult.begin(), LResult.end(),[](char a, char b) { return a== '/' && b == '/';});
                LResult.erase(new_end, LResult.end());
                if (LResult.size() > 1 && LResult.back() == '/')
                    LResult.pop_back();
            return LResult;
        }
        static std::string GetExtension(const std::string& InPath);
    };
}