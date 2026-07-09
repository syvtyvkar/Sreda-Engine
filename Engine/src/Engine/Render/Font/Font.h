// (c) Nikita Rogalev. All rights reserved.

#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include <glm/glm.hpp>

#include "BaseEngine.h"
#include "Engine/Render/Texture.h"
#include "Engine/Core/FileSystem/AssetManager.h"

#define DEFAULT_FONT_NAME "Inter_18pt-Regular" // "Inter_18pt-Regular" "Cuprum"

namespace Engine::Render
{
    struct MSDFData;
    struct GlyphMetrics
    {
        Vector2 uvMin;
        Vector2 uvMax;
        glm::ivec2 size;
        glm::ivec2 bearing;
        uint32_t advance;
    };

    struct TextParams
	{
		TColor Color = TColor::White;
		float Kerning = 0.0f;
		float LineSpacing = 0.0f;
	};

    class Font : public Engine::IAsset
    {
        friend class AssetFontFileLoader;
    public:
        Font(const std::string& filepath);
        Font();
        virtual ~Font() override;

        TRef<Texture2D> GetAtlasTexture() const { return m_AtlasTexture; }
        const std::unordered_map<uint32_t, GlyphMetrics>& GetGlyphs() const { return m_Glyphs; }
        int GetFontSize() const { return m_FontSize; }

        static Vector2 CalculateTextSize(const std::wstring& InText, const Font* InFont, int InFontSize);

    protected:
        void GenerateAtlas(const std::string& filepath, const std::vector<uint32_t>& codepoints);

        TRef<Texture2D> m_AtlasTexture;
        std::unordered_map<uint32_t, GlyphMetrics> m_Glyphs;
        int m_FontSize;
    };

    /**
     * @brief IAssetLoader Font
     * 
     * 
     */
    class AssetFontFileLoader : public IAssetLoader
    {
    public:
        AssetFontFileLoader()
        {
        };
        virtual ~AssetFontFileLoader() = default;


        /**
        *   Load Asset */
        virtual TRef<IAsset> LoadStatic(const std::string& InPath, TUniquePtr<IFile> File) override;
    };

    std::vector<uint32_t> GetDefaultCodepoints();

    /*
    * @brief Font Manager
    *
    */
    class FontManager
    {
    public:
        FontManager();
        ~FontManager();

        static FontManager& GetFontManager();                               // Get reference to the single instance (singleton)
        bool IsFontContain(const std::string& InNameFont);                  // Check Font
        TRef<Font> GetFont(const std::string& InNameFont);                  // Get Font from name
        TRef<Font> GetFontDefault();                                        // Get Font from name
        TRef<Font> AddFont(const std::string& InNameFont);                  // Add Font from name
        bool RemoveFont(const std::string& InNameFont);                     // Remove Font from name
        void ClearAllFont();                                                // Clear all font (pre delete)

    private:
        std::unordered_map<std::string, TRef<Font>> m_FontDataBase;         // All load fints
    };
}
