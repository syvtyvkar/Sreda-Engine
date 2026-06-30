// (c) Nikita Rogalev. All rights reserved.

#include "Font.h"

#include <cstring>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "Engine/Platform/PlatformUtils.h"

namespace Engine::Render
{
    struct RawGlyph
    {
        int width;
        int height;
        int bearingX;
        int bearingY;
        unsigned int advance;
        std::vector<unsigned char> buffer;
        int texX = 0;
        int texY = 0;
    };

    struct Shelf
    {
        int y;
        int height;
        int remainingX;
    };

    static void PackGlyphs(std::vector<std::pair<uint32_t, RawGlyph>>& glyphs,std::vector<unsigned char>& outPixels,int& outWidth,int& outHeight,std::unordered_map<uint32_t, GlyphMetrics>& outMetrics,int padding)
    {
        std::vector<std::pair<uint32_t, RawGlyph>> toPack;
        for (auto& pair : glyphs)
        {
            if (pair.second.width > 0 && pair.second.height > 0)
                toPack.push_back(pair);

            GlyphMetrics gm;
            gm.uvMin = Vector2(0.0f);
            gm.uvMax = Vector2(0.0f);
            gm.size = glm::ivec2(pair.second.width, pair.second.height);
            gm.bearing = glm::ivec2(pair.second.bearingX, pair.second.bearingY);
            gm.advance = pair.second.advance;
            outMetrics[pair.first] = gm;
        }

        if (toPack.empty())
        {
            outWidth = 1;
            outHeight = 1;
            outPixels.resize(1, 0);
            return;
        }

        std::sort(toPack.begin(), toPack.end(),[](const auto& a, const auto& b){return a.second.height > b.second.height;});

        int totalArea = 0;
        int maxGlyphWidth = 0;
        for (const auto& [cp, rg] : toPack)
        {
            totalArea += (rg.width + padding) * (rg.height + padding);
            if (rg.width > maxGlyphWidth) maxGlyphWidth = rg.width;
        }

        int atlasW = 256;
        while (atlasW * atlasW < totalArea && atlasW < 4096)
        {
            atlasW *= 2;
        }
        atlasW = std::max(atlasW, maxGlyphWidth + padding);

        std::vector<Shelf> shelves;
        int atlasH = 0;

        for (auto& [cp, rg] : toPack)
        {
            int gw = rg.width + padding;
            int gh = rg.height + padding;
            bool placed = false;

            for (auto& shelf : shelves)
            {
                if (shelf.remainingX >= gw && shelf.height >= gh)
                {
                    rg.texX = atlasW - shelf.remainingX;
                    rg.texY = shelf.y;
                    shelf.remainingX -= gw;
                    placed = true;
                    break;
                }
            }

            if (!placed)
            {
                rg.texX = 0;
                rg.texY = atlasH;
                shelves.push_back({ atlasH, gh, atlasW - gw });
                atlasH += gh;
            }
        }

        outWidth = atlasW;
        outHeight = std::max(atlasH, 1);
        outPixels.resize(outWidth * outHeight, 0);

        for (const auto& [cp, rg] : toPack)
        {
            if (rg.buffer.empty()) continue;

            for (int row = 0; row < rg.height; ++row)
            {
                int destOffset = (rg.texY + row) * outWidth + rg.texX;
                int srcOffset = row * rg.width;
                memcpy(&outPixels[destOffset], &rg.buffer[srcOffset], rg.width);
            }

            outMetrics[cp].uvMin = Vector2(float(rg.texX) / outWidth, float(rg.texY) / outHeight);
            outMetrics[cp].uvMax = Vector2(float(rg.texX + rg.width) / outWidth, float(rg.texY + rg.height) / outHeight);
        }
    }

    Font::Font(const std::string& filepath) : m_FontSize(48)
    {
        auto codepoints = GetDefaultCodepoints();
        GenerateAtlas(filepath, codepoints);
    }

    Vector2 Font::CalculateTextSize(const std::wstring &InText, const Font* InFont, int InFontSize)
    {
        if (InFont==nullptr)
        {
            ENGINE_LOG_WARN("Font::CalculateTextSize NO VALID FONT!");
            return Vector2(50.f);
        }
        float scale = (float)InFontSize / 48.0f;
        float width = 0.0f;
        float height = 0.0f;
        for (wchar_t ch : InText) 
        {
            auto it = InFont->GetGlyphs().find((uint32_t)ch);
            if (it == InFont->GetGlyphs().end()) continue;
            const GlyphMetrics& gm = it->second;
            width += (gm.advance >> 6) * scale; // advance in 1/64 pixel
            float glyphHeight = (gm.size.y + gm.bearing.y) * scale;
            if (glyphHeight > height) height = glyphHeight;
        }
        return { width, height };
    }

    void Font::GenerateAtlas(const std::string &filepath, const std::vector<uint32_t> &codepoints)
    {
        FT_Library ft;
        if (FT_Init_FreeType(&ft))
        {
            ENGINE_LOG_ERROR("FreeType: failed to initialize");
            return;
        }

        std::string fullPath = PlatformUtils::GetProjectDirectory() + "/" + filepath;
        FT_Face face;
        if (FT_New_Face(ft, fullPath.c_str(), 0, &face))
        {
            ENGINE_LOG_ERROR("FreeType: failed to load font '{}'", fullPath);
            FT_Done_FreeType(ft);
            return;
        }
        FT_Set_Pixel_Sizes(face, 0, m_FontSize);

        std::unordered_map<uint32_t, RawGlyph> rawMap;
        for (uint32_t cp : codepoints)
        {
            if (FT_Load_Char(face, cp, FT_LOAD_RENDER))
            {
                ENGINE_LOG_WARN("FreeType: failed to load glyph for codepoint {}", cp);
                continue;
            }

            FT_GlyphSlot g = face->glyph;
            RawGlyph rg;
            rg.width = g->bitmap.width;
            rg.height = g->bitmap.rows;
            rg.bearingX = g->bitmap_left;
            rg.bearingY = g->bitmap_top;
            rg.advance = g->advance.x;

            if (rg.width > 0 && rg.height > 0)
            {
                rg.buffer.resize(rg.width * rg.height);
                memcpy(rg.buffer.data(), g->bitmap.buffer, rg.buffer.size());
            }
            rawMap[cp] = rg;
        }

        std::vector<std::pair<uint32_t, RawGlyph>> glyphList(rawMap.begin(), rawMap.end());

        std::vector<unsigned char> atlasPixels;
        int atlasW, atlasH;
        PackGlyphs(glyphList, atlasPixels, atlasW, atlasH, m_Glyphs, 1);

        if (atlasPixels.empty())
        {
            ENGINE_LOG_ERROR("Font atlas generation failed: no glyphs loaded");
            FT_Done_Face(face);
            FT_Done_FreeType(ft);
            return;
        }

		TextureSpecification spec;
		spec.Width = atlasW;
		spec.Height = atlasH;
		spec.Format = ImageFormat::R8;
		spec.GenerateMips = false;
		spec.WrapS = TextureWrap::ClampToEdge;
		spec.WrapT = TextureWrap::ClampToEdge;

        m_AtlasTexture = Texture2D::Create(spec);
        m_AtlasTexture->SetData(atlasPixels.data(), atlasW * atlasH);

        ENGINE_LOG_INFO("Font atlas created: {}x{} ({} glyphs)", atlasW, atlasH, m_Glyphs.size());

        FT_Done_Face(face);
        FT_Done_FreeType(ft);
    }

    std::vector<uint32_t> GetDefaultCodepoints()
    {
        std::vector<uint32_t> codepoints;

        for (uint32_t c = 32; c <= 126; ++c)
            codepoints.push_back(c);

        for (uint32_t c = 0x0410; c <= 0x042F; ++c)
            codepoints.push_back(c);

        for (uint32_t c = 0x0430; c <= 0x044F; ++c)
            codepoints.push_back(c);

        codepoints.push_back(0x0401);
        codepoints.push_back(0x0451);

        return codepoints;
    }

    TUniquePtr<FontManager> FontManager::s_Instance=nullptr;                           // Static pointer to the single (singleton)

    Render::FontManager::FontManager()
    {
        ENGINE_LOG_TRACE("Start Font Manager...");
    }

    Render::FontManager::~FontManager()
    {
        ENGINE_LOG_TRACE("Stop Font Manager...");
        m_FontDataBase.clear();
    }

    FontManager &Render::FontManager::GetFontManager()
    {
        if (s_Instance)
        {
            return *s_Instance;
        }

        s_Instance = CreateUniquePtr<FontManager>();

        return *s_Instance;
        // TODO: insert return statement here
    }

    bool Render::FontManager::IsFontContain(const std::string &InNameFont)
    {
        std::string LFontName = InNameFont;
        auto LInt = m_FontDataBase.find(LFontName);
        if (LInt != m_FontDataBase.end())
        {
            return true;
        }
        return false;
    }

    TRef<Font> Render::FontManager::GetFont(const std::string &InNameFont)
    {
        std::string LFontName = InNameFont;
        auto LInt = m_FontDataBase.find(LFontName);
        if (LInt != m_FontDataBase.end())
        {
            return LInt->second;
        }
        return AddFont(InNameFont);
    }

    TRef<Font> Render::FontManager::GetFontDefault()
    {
        return GetFont(DEFAULT_FONT_NAME);
    }

    TRef<Font> Render::FontManager::AddFont(const std::string &InNameFont)
    {
        std::string LFontName = InNameFont;
        TRef<Font> LNewFont = CreateRef<Font>("Resources/Fonts/" + InNameFont + ".ttf");
        if (!LNewFont)
        {
            return nullptr;
        }
        m_FontDataBase.insert({LFontName,LNewFont});

        ENGINE_LOG_TRACE("Load new font: {1}", LFontName);

        return LNewFont;
    }

    bool Render::FontManager::RemoveFont(const std::string& InNameFont)
    {
        std::string LFontName = InNameFont;
        auto LInt = m_FontDataBase.find(LFontName);
        if (LInt != m_FontDataBase.end())
        {
            m_FontDataBase.erase(LFontName);

            ENGINE_LOG_TRACE("Remove font: {1}", LFontName);
            return true;
        }
        return false;
    }
}
