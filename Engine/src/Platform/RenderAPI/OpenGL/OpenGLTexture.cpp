#include <glad/glad.h>
#include "OpenGLTexture.h"

#include "Engine/Core/FileSystem/FileIO.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <GLFW/glfw3.h>

namespace Engine::Render
{
    namespace Utils {

		static GLenum ImageFormatToGLDataFormat(ImageFormat format)
		{
			switch (format)
			{
				case ImageFormat::RGB8:  return GL_RGB;
				case ImageFormat::RGBA8: return GL_RGBA;
			}

			ENGINE_ASSERT(false,"");
			return 0;
		}
		
		static GLenum ImageFormatToGLInternalFormat(ImageFormat format)
		{
			switch (format)
			{
			case ImageFormat::RGB8:  return GL_RGB8;
			case ImageFormat::RGBA8: return GL_RGBA8;
			}

			ENGINE_ASSERT(false,"");
			return 0;
		}

	}

	OpenGLTexture2D::OpenGLTexture2D(const TextureSpecification& specification)
		: m_Specification(specification), m_Width(m_Specification.Width), m_Height(m_Specification.Height)
	{

		m_InternalFormat = Utils::ImageFormatToGLInternalFormat(m_Specification.Format);
		m_DataFormat = Utils::ImageFormatToGLDataFormat(m_Specification.Format);

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, m_InternalFormat, m_Width, m_Height);

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}

	OpenGLTexture2D::OpenGLTexture2D(const std::string& path): m_Path(path)
	{

		if (glfwGetCurrentContext() == nullptr) 
		{
        	ENGINE_LOG_CRITICAL("OpenGL context is NOT current in this thread!");
        	return;
    	}


		std::string LPath = FileIO::GetProjectDirectory() + path;
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

	void OpenGLTexture2D::SetData(void* data, uint32_t size)
	{
		uint32_t bpp = m_DataFormat == GL_RGBA ? 4 : 3;
		ENGINE_ASSERT(size == m_Width * m_Height * bpp, "Data must be entire texture!");
		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);
	}

	void OpenGLTexture2D::Bind(uint32_t slot) const
	{
		glBindTextureUnit(slot, m_RendererID);
	}
}