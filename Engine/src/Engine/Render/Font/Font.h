// (c) Nikita Rogalev. All rights reserved.

#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include <glm/glm.hpp>

#include "BaseEngine.h"
#include "Engine/Render/Texture.h"

namespace Engine::Render
{
    struct GlyphMetrics
    {
        Vector2 uvMin;
        Vector2 uvMax;
        glm::ivec2 size;
        glm::ivec2 bearing;
        uint32_t advance;
    };

    class Font
    {
    public:
        Font(const std::string& filepath, int fontSize = 48);
        ~Font() = default;

        TRef<Texture2D> GetAtlasTexture() const { return m_AtlasTexture; }
        const std::unordered_map<uint32_t, GlyphMetrics>& GetGlyphs() const { return m_Glyphs; }
        int GetFontSize() const { return m_FontSize; }

        static TRef<Font> GetDefault();

    private:
        void GenerateAtlas(const std::string& filepath, const std::vector<uint32_t>& codepoints);

        TRef<Texture2D> m_AtlasTexture;
        std::unordered_map<uint32_t, GlyphMetrics> m_Glyphs;
        int m_FontSize;
    };

    std::vector<uint32_t> GetDefaultCodepoints();
}
