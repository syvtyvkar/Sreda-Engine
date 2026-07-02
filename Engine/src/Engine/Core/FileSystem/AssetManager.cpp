#include "AssetManager.h"

namespace Engine 
{
    void AssetManager::RegisterLoader(const std::string &InExtension, TUniquePtr<IAssetLoader> InLoader)
    {
        m_loaders[InExtension] = std::move(InLoader);
    }
    
    void AssetManager::Unload(const std::string InPath)
    {
        std::string LNormalPath = Normalize(InPath);
        auto it = m_cashe.find(LNormalPath);
        if (it != m_cashe.end() && it->second.expired())
        {
            m_cashe.erase(it);
        }
    }

    void AssetManager::GarbageCollect()
    {
        for (auto it = m_cashe.begin(); it != m_cashe.end();)
        {
            if (it->second.expired())
            {
                it = m_cashe.erase(it);
            }else
                ++it;
        }
    }

    TRef<IAsset> AssetManager::CallLoadAssetStatic(const std::string InNormalPath)
    {
        std::string ext = GetExtension(InNormalPath);
        auto loaderIt = m_loaders.find(ext);
        if (loaderIt == m_loaders.end())
            return nullptr;

        auto file = m_vfs.Open(InNormalPath);

        if (!file)
            return nullptr;

        auto asset = loaderIt->second->LoadStatic(InNormalPath, std::move(file));
        if (asset) asset.get()->m_path = InNormalPath;

        return asset;
    }

    std::string AssetManager::GetExtension(const std::string &InPath)
    {
        auto pos = InPath.rfind('.');
        if (pos == std::string::npos) return "";
        return InPath.substr(pos).c_str();
    }
}