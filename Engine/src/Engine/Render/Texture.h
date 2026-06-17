// (c) Nikita Rogalev. All rights reserved.

#pragma once // Multiple inclusion guard

#include "../../Engine/Core/Log.h"
#include "../../Engine/Core/Utilities/Types.h"
#include <string>

namespace Engine::Render
{
	enum class TextureFileType
	{
		PNG = 0,
		JPG,
		JPEG,
		BMP
	};

    enum class ImageFormat
	{
		None = 0,
		R8,
		RGB8,
		RGBA8,
		RGBA32F
	};

	enum class TextureWrap
	{
		Repeat = 0,
		ClampToEdge,
		MirroredRepeat
	};

	struct TextureSpecification
	{
		uint32_t Width = 1;
		uint32_t Height = 1;
		ImageFormat Format = ImageFormat::RGBA8;
		bool GenerateMips = true;
		TextureWrap WrapS = TextureWrap::Repeat;
		TextureWrap WrapT = TextureWrap::Repeat;
	};

	class Texture
	{
	public:
		virtual ~Texture() = default;

		virtual const TextureSpecification& GetSpecification() const = 0;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;
		virtual uint32_t GetRendererID() const = 0;
		virtual uint32_t GetSize() const = 0;
		
		virtual const std::string& GetPath() const = 0;

		virtual void SetData(void* data, uint32_t size) = 0;
		virtual void* GetData() const = 0;
		virtual std::vector<unsigned char> GetDataRGBA8() const = 0;

		virtual void Bind(uint32_t slot = 0) const = 0;

		virtual bool IsLoaded() const = 0;

		virtual bool Save(const std::string& path, TextureFileType Type) const = 0;

		virtual bool operator==(const Texture& other) const = 0;
	};

	class Texture2D : public Texture
	{
	public:
		static TRef<Texture2D> Create(const TextureSpecification& specification);
		static TRef<Texture2D> Create(const std::string& path);
	};
}
