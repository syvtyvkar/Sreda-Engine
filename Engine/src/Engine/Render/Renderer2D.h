// (c) Nikita Rogalev. All rights reserved.

#pragma once // Multiple inclusion guard

#include "../../Engine/Core/Log.h"
#include "../../Engine/Core/Utilities/Types.h"

#include "OrthographicCamera.h"

#include "Texture.h"

#include "Font/Font.h"

#include "Camera.h"

namespace Engine::Render
{
    class Renderer2D
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const Camera& camera, const glm::mat4& transform);
		static void BeginScene(const OrthographicCamera& camera); // TODO: Remove
		static void EndScene();
		static void Flush();

		// Primitives
		static void DrawQuad(const Vector2& position, const Vector2& size, const TColor& color);
		static void DrawQuad(const Vector3& position, const Vector2& size, const TColor& color);
		static void DrawQuad(const Vector2& position, const Vector2& size, const TRef<Texture2D>& texture, float tilingFactor = 1.0f, const TColor& tintColor = TColor::White);
		static void DrawQuad(const Vector3& position, const Vector2& size, const TRef<Texture2D>& texture, float tilingFactor = 1.0f, const TColor& tintColor = TColor::White);

		static void DrawQuad(const glm::mat4& transform, const TColor& color, int entityID = -1);
		static void DrawQuad(const glm::mat4& transform, const TRef<Texture2D>& texture, float tilingFactor = 1.0f, const TColor& tintColor = TColor::White, int entityID = -1);

		static void DrawRotatedQuad(const Vector2& position, const Vector2& size, float rotation, const TColor& color);
		static void DrawRotatedQuad(const Vector3& position, const Vector2& size, float rotation, const TColor& color);
		static void DrawRotatedQuad(const Vector2& position, const Vector2& size, float rotation, const TRef<Texture2D>& texture, float tilingFactor = 1.0f, const TColor& tintColor = TColor::White);
		static void DrawRotatedQuad(const Vector3& position, const Vector2& size, float rotation, const TRef<Texture2D>& texture, float tilingFactor = 1.0f, const TColor& tintColor = TColor::White);

		static void DrawCircle(const glm::mat4& transform, const TColor& color, float thickness = 1.0f, float fade = 0.005f, int entityID = -1);
		
		static void DrawLine(const Vector3& p0, Vector3& p1, const TColor& color, int entityID = -1);

		static void DrawRect(const Vector3& position, const Vector2& size, const TColor& color, int entityID = -1);
		static void DrawRect(const glm::mat4& transform, const TColor& color, int entityID = -1);

		static void RenderDrawText(const std::wstring& text, const TRef<Texture2D>& fontAtlas,const std::unordered_map<uint32_t, GlyphMetrics>& glyphs,float x, float y, int FontSize, const TColor& color,int entityID = -1);

		static float GetLineWidth();
		static void SetLineWidth(float width);

		// Stats
		struct Statistics
		{
			uint32_t DrawCalls = 0;
			uint32_t QuadCount = 0;

			uint32_t GetTotalVertexCount() const { return QuadCount * 4; }
			uint32_t GetTotalIndexCount() const { return QuadCount * 6; }
		};
		static void ResetStats();
		static Statistics GetStats();

	private:
		static void StartBatch();
		static void NextBatch();
	};
}
