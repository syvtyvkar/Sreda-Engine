#include <glad/glad.h>
#include "OpenGLTexture.h"

#include "Engine/Core/FileSystem/FileIO.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

namespace Engine::Render
{
    namespace Utils {

		static GLenum ImageFormatToGLDataFormat(ImageFormat format)
		{
			switch (format)
			{
				case ImageFormat::R8:  	 	return GL_RED;
				case ImageFormat::RGB8:  	return GL_RGB;
				case ImageFormat::RGBA8: 	return GL_RGBA;
				case ImageFormat::RGBA32F: 	return GL_RGBA; 
				default: ENGINE_ASSERT(false, "Unknown ImageFormat"); return 0;
			}

			//ENGINE_ASSERT(false,"");
			//return 0;
		}
		
		static GLenum ImageFormatToGLInternalFormat(ImageFormat format)
		{
			switch (format)
			{
			case ImageFormat::R8:  			return GL_R8;
			case ImageFormat::RGB8:  		return GL_RGB8;
			case ImageFormat::RGBA8: 		return GL_RGBA8;
			case ImageFormat::RGBA32F: 		return GL_RGBA32F;
			default: ENGINE_ASSERT(false, "Unknown ImageFormat"); return 0;
			}
		}
	}

	OpenGLTexture2D::OpenGLTexture2D(const TextureSpecification& specification)
		: m_Specification(specification), m_Width(m_Specification.Width), m_Height(m_Specification.Height)
	{

		m_InternalFormat = Utils::ImageFormatToGLInternalFormat(m_Specification.Format);
		m_DataFormat = Utils::ImageFormatToGLDataFormat(m_Specification.Format);
		
		//ENGINE_LOG_INFO("Created texture with format={0}, internal={1}, data={2}", (int)m_Specification.Format, m_InternalFormat, m_DataFormat);

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, m_InternalFormat, m_Width, m_Height);

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, m_Specification.GenerateMips ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		GLenum wrapS = GL_REPEAT;
		GLenum wrapT = GL_REPEAT;
		switch (m_Specification.WrapS)
		{
			case TextureWrap::ClampToEdge:  wrapS = GL_CLAMP_TO_EDGE; break;
			case TextureWrap::MirroredRepeat: wrapS = GL_MIRRORED_REPEAT; break;
			default: wrapS = GL_REPEAT; break;
		}
		switch (m_Specification.WrapT)
		{
			case TextureWrap::ClampToEdge:  wrapT = GL_CLAMP_TO_EDGE; break;
			case TextureWrap::MirroredRepeat: wrapT = GL_MIRRORED_REPEAT; break;
			default: wrapT = GL_REPEAT; break;
		}
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, wrapS);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, wrapT);

		if (m_Specification.GenerateMips)
			glGenerateTextureMipmap(m_RendererID);
	}

	OpenGLTexture2D::OpenGLTexture2D(const std::string& path): m_Path(path)
	{

		if (glfwGetCurrentContext() == nullptr) 
		{
        	ENGINE_LOG_CRITICAL("OpenGL context is NOT current in this thread!");
        	return;
    	}


		std::string LPath = PlatformUtils::GetProjectDirectory() + path;
    	stbi_set_flip_vertically_on_load(1);
    
		int width, height, channels;
		stbi_uc* data = stbi_load(LPath.c_str(), &width, &height, &channels, 0);
		
		if (!data) 
		{
			ENGINE_LOG_WARN("Failed to load texture: {}", LPath);
			return;
		}

		if (width <= 0 || height <= 0) 
		{
        	ENGINE_LOG_ERROR("Invalid texture dimensions: {}x{}", width, height);
        	stbi_image_free(data);
        	return;
    	}

		m_IsLoaded = true;
		m_Width = width;
		m_Height = height;

		GLenum internalFormat = 0, dataFormat = 0;
		if (channels == 4) { internalFormat = GL_RGBA8; dataFormat = GL_RGBA; }
		else if (channels == 3) { internalFormat = GL_RGB8; dataFormat = GL_RGB; }
		else if (channels == 1) { internalFormat = GL_R8; dataFormat = GL_RED; }

		if (internalFormat == 0 || dataFormat == 0) 
		{
			ENGINE_LOG_ERROR("Unsupported channels: {} in {}", channels, path);
			stbi_image_free(data);
			return;
		}

		m_InternalFormat = internalFormat;
		m_DataFormat = dataFormat;

		glGenTextures(1, &m_RendererID);
		glBindTexture(GL_TEXTURE_2D, m_RendererID);

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		
		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, m_Width, m_Height, 0, dataFormat, GL_UNSIGNED_BYTE, data);

		glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glBindTexture(GL_TEXTURE_2D, 0);

		if (GLenum err = glGetError(); err != GL_NO_ERROR) 
		{
			ENGINE_LOG_ERROR("OpenGL Error after glTexImage2D: 0x{:x} (File: {})", err, path);
		}

		ENGINE_LOG_TRACE("Loaded texture: {}", path);
		stbi_image_free(data);
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		glDeleteTextures(1, &m_RendererID);
	}

    uint32_t OpenGLTexture2D::GetSize() const
    {
        uint32_t bpp = 0;
    	switch (m_Specification.Format) 
		{
			case ImageFormat::R8:      bpp = 1; break;
        	case ImageFormat::RGB8:    bpp = 3; break;
        	case ImageFormat::RGBA8:   bpp = 4; break;
        	case ImageFormat::RGBA32F: bpp = 16; break;
        	default: ENGINE_ASSERT(false, "Unknown format in Data");
    	}
		return m_Width * m_Height * bpp;
    }

    void OpenGLTexture2D::SetData(void *data, uint32_t size)
    {
		uint32_t bpp = 0;
    	switch (m_Specification.Format) 
		{
			case ImageFormat::R8:      bpp = 1; break;
        	case ImageFormat::RGB8:    bpp = 3; break;
        	case ImageFormat::RGBA8:   bpp = 4; break;
        	case ImageFormat::RGBA32F: bpp = 16; break;
        	default: ENGINE_ASSERT(false, "Unknown format in Data"); return;
    	}
		ENGINE_ASSERT(size == m_Width * m_Height * bpp, "Data size mismatch!");

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	}

    void *OpenGLTexture2D::GetData() const
    {
        uint32_t size = GetSize();
		void* data = malloc(size);
		if (!data) return nullptr;

		GLenum type = (m_Specification.Format == ImageFormat::RGBA32F) ? GL_FLOAT : GL_UNSIGNED_BYTE;

		glGetTextureImage(m_RendererID, 0, m_DataFormat, type, size, data);
		return data;
    }

    std::vector<unsigned char> OpenGLTexture2D::GetDataRGBA8() const
    {
        size_t pixelSize = 4; // RGBA
		size_t rowSize = m_Width * pixelSize;

		std::vector<unsigned char> result(m_Width * m_Height * 4);


		size_t paddedRowSize = (rowSize + 3) & ~3;
		std::vector<unsigned char> tempData(paddedRowSize * m_Height);

		glPixelStorei(GL_PACK_ALIGNMENT, 4);
		glGetTextureImage(m_RendererID, 0, GL_RGBA, GL_UNSIGNED_BYTE, tempData.size(), tempData.data());

		for (int y = 0; y < m_Height; ++y)
		{
			int srcY = m_Height - 1 - y; // инвертируем Y
			memcpy(result.data() + y * rowSize,
				tempData.data() + srcY * paddedRowSize,
				rowSize);
		}

		return result;
    }

    void OpenGLTexture2D::Bind(uint32_t slot) const
	{
		glBindTextureUnit(slot, m_RendererID);
	}

    bool OpenGLTexture2D::Save(const std::string &path, TextureFileType Type) const
    {
        void* data = GetData();
		if (!data) return false;

		int channels = 0;

		// Определяем количество каналов
		switch (m_DataFormat)
		{
			case GL_RED:   channels = 1; break;
			case GL_RGB:   channels = 3; break;
			case GL_RGBA:  channels = 4; break;
			default: free(data); return false;
		}

		bool success = false;

		// Преобразуем float → uchar при необходимости
		unsigned char* byteData = nullptr;
		if (m_Specification.Format == ImageFormat::RGBA32F)
		{
			byteData = new unsigned char[m_Width * m_Height * channels];
			float* floatData = (float*)data;
			for (uint32_t i = 0; i < m_Width * m_Height * channels; ++i)
				byteData[i] = (unsigned char)(floatData[i] * 255.0f);
		}
		else
		{
			byteData = (unsigned char*)data;
		}

		switch (Type)
		{
		case TextureFileType::PNG:
			success = stbi_write_png(path.c_str(), m_Width, m_Height, channels, byteData, m_Width * channels);
			break;
		case TextureFileType::JPG:
			success = stbi_write_jpg(path.c_str(), m_Width, m_Height, channels, byteData, 90);
			break;
		case TextureFileType::JPEG:
			success = stbi_write_jpg(path.c_str(), m_Width, m_Height, channels, byteData, 90);
			break;
		case TextureFileType::BMP:
			success = stbi_write_bmp(path.c_str(), m_Width, m_Height, channels, byteData);
			break;
		default:
			ENGINE_ASSERT(false, "Unsupported image format for saving!");
			break;
		}

		if (m_Specification.Format == ImageFormat::RGBA32F)
			delete[] byteData;

		free(data);
		if (success)
			ENGINE_LOG_INFO("Texture saved to: {}", path);
		else
			ENGINE_LOG_ERROR("Failed to save texture to: {}", path);

		return success;
    }
}