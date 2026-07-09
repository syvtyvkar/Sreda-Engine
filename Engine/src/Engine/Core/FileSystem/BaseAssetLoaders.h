// (c) Nikita Rogalev. All rights reserved.

#pragma once    // Multiple inclusion guard

#include "AssetManager.h"

#include <fstream>
#include <map>
#include <mutex>
#include <string>

namespace Engine 
{
    /**
     * @brief IAssetLoader
     * 
     * 
     */
    class IAssetTextFileLoader : public IAssetLoader
    {
    public:
        IAssetTextFileLoader()
        {

        };
        virtual ~IAssetTextFileLoader() = default;


        /**
        *   Load Asset */
        virtual TRef<IAsset> LoadStatic(const std::string& InPath, TUniquePtr<IFile>) override
        {

        };
    };
}