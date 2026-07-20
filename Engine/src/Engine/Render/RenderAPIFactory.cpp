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
/*#include "Platform/RenderAPI/Vulkan/VulkanAPI.h"
#include "Platform/RenderAPI/Vulkan/VulkanBuffer.h"
#include "Platform/RenderAPI/Vulkan/VulkanContext.h"
#include "Platform/RenderAPI/Vulkan/VulkanFramebuffer.h"
#include "Platform/RenderAPI/Vulkan/VulkanRendererAPI.h"
#include "Platform/RenderAPI/Vulkan/VulkanShader.h"
#include "Platform/RenderAPI/Vulkan/VulkanTexture.h"
#include "Platform/RenderAPI/Vulkan/VulkanUniformBuffer.h"
#include "Platform/RenderAPI/Vulkan/VulkanVertex.h"*/

// Engine core
#include "Engine/Core/Base/EngineCore.h"

namespace Engine::Render
{
    RenderAPIFactory::RHI_API RenderAPIFactory::s_API = RenderAPIFactory::RHI_API::OpenGL;

    RenderAPIFactory::VertexArrayCreator RenderAPIFactory::s_VertexArrayCreator = nullptr;
    RenderAPIFactory::UniformBufferCreator RenderAPIFactory::s_UniformBufferCreator = nullptr;
    RenderAPIFactory::Texture2DCreator RenderAPIFactory::s_Texture2DCreator = nullptr;
    RenderAPIFactory::Texture2DCreatorPath RenderAPIFactory::s_Texture2DCreatorPath = nullptr;
    RenderAPIFactory::ShaderCreator RenderAPIFactory::s_ShaderCreator = nullptr;
    RenderAPIFactory::ShaderCreatorPath RenderAPIFactory::s_ShaderCreatorPath = nullptr;
    //RenderAPIFactory::GraphicsContextCreator RenderAPIFactory::s_GraphicsContextCreator = nullptr;
    RenderAPIFactory::FramebufferCreator RenderAPIFactory::s_FramebufferCreator= nullptr; 
    RenderAPIFactory::VertexBufferCreator RenderAPIFactory::s_VertexBufferCreator= nullptr; 
    RenderAPIFactory::VertexBufferCreatorVertx RenderAPIFactory::s_VertexBufferCreatorVertx= nullptr; 
    RenderAPIFactory::IndexBufferCreator RenderAPIFactory::s_IndexBufferCreator = nullptr;

    void RenderAPIFactory::Init()
    {
        ENGINE_ASSERT(Engine::EngineCore::GetEngineContext().GetConfigSystem(), "Config System no valid!");
        std::string renderAPIStr = Engine::EngineCore::GetEngineContext().GetConfigSystem()->Get<std::string>("Engine.RenderAPI", "OpenGL");
        if (renderAPIStr == "Vulkan")
		{
            s_API = RHI_API::Vulkan;
		}

        switch (RenderAPIFactory::GetRenderAPI())
		{
			case RHI_API::None:
                ENGINE_ASSERT(false, "RendererAPI::None is currently not supported!"); 
                return;
			case RHI_API::OpenGL:  
                // Init OpenGL Factory
                s_VertexArrayCreator = []() -> TRef<VertexArray> { return CreateRef<OpenGLVertexArray>(); };
                s_UniformBufferCreator = [](uint32_t size, uint32_t binding) -> TRef<UniformBuffer> { return CreateRef<OpenGLUniformBuffer>(size, binding); };
                s_Texture2DCreator = [](const TextureSpecification& specification) -> TRef<Texture2D> { return CreateRef<OpenGLTexture2D>(specification); };
                s_Texture2DCreatorPath = [](const std::string& InPath) -> TRef<Texture2D> { return CreateRef<OpenGLTexture2D>(InPath); };
                s_ShaderCreator = [](const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc) -> TRef<Shader> { return CreateRef<OpenGLShader>(name, vertexSrc, fragmentSrc); };
                s_ShaderCreatorPath = [](const std::string& InPath) -> TRef<Shader> { return CreateRef<OpenGLShader>(InPath); };
                s_FramebufferCreator = [](const struct FramebufferSpecification& spec) -> TRef<Framebuffer> { return CreateRef<OpenGLFramebuffer>(spec); };
                s_VertexBufferCreator = [](uint32_t size) -> TRef<VertexBuffer> { return CreateRef<OpenGLVertexBuffer>(size); };
                s_VertexBufferCreatorVertx = [](float* vertices, uint32_t size) -> TRef<VertexBuffer> { return CreateRef<OpenGLVertexBuffer>(vertices, size); };
                s_IndexBufferCreator = [](uint32_t* indices, uint32_t count) -> TRef<IndexBuffer> { return CreateRef<OpenGLIndexBuffer>(indices,count); };
                return;
			case RHI_API::Vulkan:  
                // Init Vulkan Factory
                /*s_VertexArrayCreator = []() -> TRef<VertexArray> { return CreateRef<VulkanVertexArray>(); };
                s_UniformBufferCreator = [](uint32_t size, uint32_t binding) -> TRef<UniformBuffer> { return CreateRef<VulkanUniformBuffer>(size, binding); };
                s_Texture2DCreator = [](const TextureSpecification& specification) -> TRef<Texture2D> { return CreateRef<VulkanTexture2D>(specification); };
                s_Texture2DCreatorPath = [](const std::string& InPath) -> TRef<Texture2D> { return CreateRef<VulkanTexture2D>(InPath); };
                s_ShaderCreator = [](const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc) -> TRef<Shader> { return CreateRef<VulkanShader>(name, vertexSrc, fragmentSrc); };
                s_ShaderCreatorPath = [](const std::string& InPath) -> TRef<Shader> { return CreateRef<VulkanShader>(InPath); };
                s_FramebufferCreator = [](const struct FramebufferSpecification& spec) -> TRef<Framebuffer> { return CreateRef<VulkanFramebuffer>(spec); };
                s_VertexBufferCreator = [](uint32_t size) -> TRef<VertexBuffer> { return CreateRef<VulkanVertexBuffer>(size); };
                s_VertexBufferCreatorVertx = [](float* vertices, uint32_t size) -> TRef<VertexBuffer> { return CreateRef<VulkanVertexBuffer>(vertices, size); };
                s_IndexBufferCreator = [](uint32_t* indices, uint32_t count) -> TRef<IndexBuffer> { return CreateRef<VulkanIndexBuffer>(indices,count); };*/
                return;
		}
    }

    TUniquePtr<class RendererAPI> RenderAPIFactory::CreateRendererAPI()
    {
        switch (RenderAPIFactory::GetRenderAPI())
		{
			case RHI_API::None:    ENGINE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RHI_API::OpenGL:  return CreateUniquePtr<OpenGLRendererAPI>();
			case RHI_API::Vulkan:  return nullptr; //CreateUniquePtr<VulkanRendererAPI>();
		}
        ENGINE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
        return nullptr;
    }

    TRef<class GraphicsContext> RenderAPIFactory::CreateGraphicsContext(TRef<Engine::IWindow> window)
    {
        switch (RenderAPIFactory::GetRenderAPI())
		{
			case RHI_API::None:    ENGINE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RHI_API::OpenGL:  return CreateRef<OpenGLContext>(window);
			case RHI_API::Vulkan:  return nullptr; //CreateRef<VulkanContext>(window);
		}
        ENGINE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
        return nullptr;
    }
}