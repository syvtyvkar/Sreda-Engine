// (c) Nikita Rogalev. All rights reserved.

#include "RenderAPIFactory.h"

// OpenGL
#include "Platform/RenderAPI/OpenGL/OpenGLBuffer.h"
#include "Platform/RenderAPI/OpenGL/OpenGLContext.h"
#include "Platform/RenderAPI/OpenGL/OpenGLFramebuffer.h"
#include "Platform/RenderAPI/OpenGL/OpenGLRendererAPI.h"
#include "Platform/RenderAPI/OpenGL/OpenGLShader.h"
#include "Platform/RenderAPI/OpenGL/OpenGLTexture.h"
#include "Platform/RenderAPI/OpenGL/OpenGLUniformBuffer.h"
#include "Platform/RenderAPI/OpenGL/OpenGLVertex.h"

// Vulkan
#include "Platform/RenderAPI/Vulkan/VulkanAPI.h"
#include "Platform/RenderAPI/Vulkan/VulkanBuffer.h"
#include "Platform/RenderAPI/Vulkan/VulkanContext.h"
#include "Platform/RenderAPI/Vulkan/VulkanFramebuffer.h"
#include "Platform/RenderAPI/Vulkan/VulkanRendererAPI.h"
#include "Platform/RenderAPI/Vulkan/VulkanShader.h"
#include "Platform/RenderAPI/Vulkan/VulkanTexture.h"
#include "Platform/RenderAPI/Vulkan/VulkanUniformBuffer.h"
#include "Platform/RenderAPI/Vulkan/VulkanVertex.h"

namespace Engine::Render
{
    RenderAPIFactory::RHI_API RenderAPIFactory::s_API = RenderAPIFactory::RHI_API::None;

    RenderAPIFactory::VertexArrayCreator RenderAPIFactory::s_VertexArrayCreator = nullptr;
    RenderAPIFactory::UniformBufferCreator RenderAPIFactory::s_UniformBufferCreator = nullptr;
    RenderAPIFactory::Texture2DCreator RenderAPIFactory::s_Texture2DCreator = nullptr;
    RenderAPIFactory::ShaderCreator RenderAPIFactory::s_ShaderCreator = nullptr;
    //RenderAPIFactory::GraphicsContextCreator RenderAPIFactory::s_GraphicsContextCreator = nullptr;
    RenderAPIFactory::FramebufferCreator RenderAPIFactory::s_FramebufferCreator= nullptr; 
    RenderAPIFactory::VertexBufferCreator RenderAPIFactory::s_VertexBufferCreator= nullptr; 
    RenderAPIFactory::IndexBufferCreator RenderAPIFactory::s_IndexBufferCreator = nullptr;

    void RenderAPIFactory::Init()
    {
        std::string renderAPIStr = EngineConfig::ConfigSystem::Get()->Get<std::string>("Engine.RenderAPI", "OpenGL");
        if (renderAPIStr == "Vulkan")
		{
			RendererAPI::SetAPI(RendererAPI::API::Vulkan);
		}

        switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::None:
                ENGINE_ASSERT(false, "RendererAPI::None is currently not supported!"); 
                return;
			case RendererAPI::API::OpenGL:  
                // Init OpenGL Factory
                s_VertexArrayCreator = []() -> TRef<VertexArray> { return CreateRef<OpenGLVertexArray>(); };
                s_UniformBufferCreator = [](uint32_t size, uint32_t binding) -> TRef<UniformBuffer> { return CreateRef<OpenGLUniformBuffer>(size, binding); };
                s_Texture2DCreator = [](const TextureSpecification& specification) -> TRef<Texture2D> { return CreateRef<OpenGLTexture2D>(specification); };
                s_ShaderCreator = [](const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc) -> TRef<Shader> { return CreateRef<OpenGLShader>(name, vertexSrc, fragmentSrc); };
                //s_GraphicsContextCreator = [](void* window) -> TRef<GraphicsContext> { return CreateRef<OpenGLContext>(window); };
                s_FramebufferCreator = [](const struct FramebufferSpecification& spec) -> TRef<Framebuffer> { return CreateRef<OpenGLFramebuffer>(spec); };
                s_VertexBufferCreator = [](float* vertices, uint32_t size) -> TRef<VertexBuffer> { return CreateRef<OpenGLVertexBuffer>(vertices,size); };
                s_IndexBufferCreator = [](uint32_t* indices, uint32_t count) -> TRef<IndexBuffer> { return CreateRef<OpenGLIndexBuffer>(indices,count); };
                return;
			case RendererAPI::API::Vulkan:  
                // Init Vulkan Factory
                s_VertexArrayCreator = []() -> TRef<VertexArray> { return CreateRef<VulkanVertexArray>(); };
                s_UniformBufferCreator = [](uint32_t size, uint32_t binding) -> TRef<UniformBuffer> { return CreateRef<VulkanUniformBuffer>(size, binding); };
                s_Texture2DCreator = [](const TextureSpecification& specification) -> TRef<Texture2D> { return CreateRef<VulkanTexture2D>(specification); };
                s_ShaderCreator = [](const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc) -> TRef<Shader> { return CreateRef<VulkanShader>(name, vertexSrc, fragmentSrc); };
                //s_GraphicsContextCreator = [](void* window) -> TRef<GraphicsContext> { return CreateRef<VulkanContext>(window); };
                s_FramebufferCreator = [](const struct FramebufferSpecification& spec) -> TRef<Framebuffer> { return CreateRef<VulkanFramebuffer>(spec); };
                s_VertexBufferCreator = [](float* vertices, uint32_t size) -> TRef<VertexBuffer> { return CreateRef<VulkanVertexBuffer>(vertices,size); };
                s_IndexBufferCreator = [](uint32_t* indices, uint32_t count) -> TRef<IndexBuffer> { return CreateRef<VulkanIndexBuffer>(indices,count); };
                return;
		}
    }

    TUniquePtr<class RendererAPI> RenderAPIFactory::CreateRendererAPI()
    {
        std::string renderAPIStr = EngineConfig::ConfigSystem::Get()->Get<std::string>("Engine.RenderAPI", "OpenGL");
        if (renderAPIStr == "Vulkan")
		{
			RendererAPI::SetAPI(RendererAPI::API::Vulkan);
		}

        switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::None:
                ENGINE_ASSERT(false, "RendererAPI::None is currently not supported!"); 
                return nullptr;
			case RendererAPI::API::OpenGL:  return CreateUniquePtr<OpenGLRendererAPI>();
			case RendererAPI::API::Vulkan:  return CreateUniquePtr<VulkanRendererAPI>();
		}
        ENGINE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
        return nullptr;
    }

    TUniquePtr<class GraphicsContext> RenderAPIFactory::CreateGraphicsContext(void *window)
    {
        switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::None:    ENGINE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return CreateUniquePtr<OpenGLContext>(static_cast<GLFWwindow*>(window));
			case RendererAPI::API::Vulkan:  return CreateUniquePtr<VulkanContext>(static_cast<GLFWwindow*>(window));
		}
        ENGINE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
        return nullptr;
    }

    /*TRef<class VertexArray> RenderAPIFactory::CreateVertexArray()
    {
        switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::None:    ENGINE_ASSERT(false, "VertexArray::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return CreateUniquePtr<OpenGLVertexArray>();
			case RendererAPI::API::Vulkan:  return CreateUniquePtr<VulkanVertexArray>();
		}
    }

    TRef<class UniformBuffer> RenderAPIFactory::CreateUniformBuffer(uint32_t size, uint32_t binding)
    {
        switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::None:    ENGINE_ASSERT(false, "VertexArray::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return CreateUniquePtr<OpenGLUniformBuffer>();
			case RendererAPI::API::Vulkan:  return CreateUniquePtr<VulkanUniformBuffer>();
		}
    }

    TRef<class Texture2D> RenderAPIFactory::CreateTexture2D(const TextureSpecification &specification)
    {
        switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::None:    ENGINE_ASSERT(false, "VertexArray::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return CreateUniquePtr<OpenGLTexture2D>();
			case RendererAPI::API::Vulkan:  return CreateUniquePtr<VulkanTexture2D>();
		}
    }

    TRef<class Shader> RenderAPIFactory::CreateShader(const std::string &name, const std::string &vertexSrc, const std::string &fragmentSrc)
    {
        switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::None:    ENGINE_ASSERT(false, "VertexArray::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return CreateUniquePtr<OpenGLShader>();
			case RendererAPI::API::Vulkan:  return CreateUniquePtr<VulkanShader>();
		}
    }

    TUniquePtr<class GraphicsContext> RenderAPIFactory::CreateGraphicsContext(void *window)
    {
        switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::None:    ENGINE_ASSERT(false, "VertexArray::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return CreateUniquePtr<OpenGLContext>();
			case RendererAPI::API::Vulkan:  return CreateUniquePtr<VulkanContext>();
		}
    }

    TRef<class Framebuffer> RenderAPIFactory::CreateFramebuffer(const FramebufferSpecification &spec)
    {
        switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::None:    ENGINE_ASSERT(false, "VertexArray::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return CreateUniquePtr<OpenGLFramebuffer>();
			case RendererAPI::API::Vulkan:  return CreateUniquePtr<VulkanFramebuffer>();
		}
    }
    TRef<class VertexBuffer> RenderAPIFactory::CreateVertexBuffer(float *vertices, uint32_t size)
    {
        switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::None:    ENGINE_ASSERT(false, "VertexArray::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return CreateUniquePtr<OpenGLVertexBuffer>();
			case RendererAPI::API::Vulkan:  return CreateUniquePtr<VulkanVertexBuffer>();
		}
    }
    TRef<class IndexBuffer> RenderAPIFactory::CreateIndexBuffer(uint32_t *indices, uint32_t count)
    {
        switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::None:    ENGINE_ASSERT(false, "VertexArray::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return CreateUniquePtr<OpenGLIndexBuffer>();
			case RendererAPI::API::Vulkan:  return CreateUniquePtr<VulkanIndexBuffer>();
		}
    }*/
}