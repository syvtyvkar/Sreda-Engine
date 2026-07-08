// (c) Nikita Rogalev. All rights reserved.

#pragma once // Multiple inclusion guard

#include "BaseEngine.h"

#include <map>

namespace Engine::Render
{
	class RenderAPIFactory
	{
    public:
        enum class RHI_API
		{
			None = 0, OpenGL = 1, Vulkan = 2
		};

        static void Init();
        static RHI_API GetRenderAPI() {return s_API; }
        static TUniquePtr<class RendererAPI> CreateRendererAPI();
        static std::string GetNameRenderAPI()
		{
			RHI_API LAPI = RenderAPIFactory::GetRenderAPI();
			switch (LAPI)
			{
                case RHI_API::OpenGL:   return "OpenGL";
                case RHI_API::Vulkan:   return "Vulkan";
                default:                return "No Render API";
			}
		    return "No Render API";
		}
        static RHI_API GetRenderAPIFromName(std::string InName)
        {
            if (InName == "OpenGL") return RHI_API::OpenGL;
            if (InName == "Vulkan") return RHI_API::Vulkan;
            return RHI_API::None;
        }
        static TRef<class VertexArray> CreateVertexArray() { return s_VertexArrayCreator(); }
        static TRef<class UniformBuffer> CreateUniformBuffer(uint32_t size, uint32_t binding) { return s_UniformBufferCreator(size,binding); }
        static TRef<class Texture2D> CreateTexture2D(const TextureSpecification& specification) { return s_Texture2DCreator(specification); }
		static TRef<class Shader> CreateShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc) { return s_ShaderCreator(name,vertexSrc,fragmentSrc); }
        static TUniquePtr<class GraphicsContext> CreateGraphicsContext(void* window);
        static TRef<class Framebuffer> CreateFramebuffer(const struct FramebufferSpecification& spec) { return s_FramebufferCreator(spec); }
		static TRef<class VertexBuffer> CreateVertexBuffer(float* vertices, uint32_t size) { return s_VertexBufferCreator(vertices,size); }
        static TRef<class IndexBuffer> CreateIndexBuffer(uint32_t* indices, uint32_t count) { return s_IndexBufferCreator(indices,count); }

    protected:

        using VertexArrayCreator = TRef<VertexArray>(*)();
        using UniformBufferCreator = TRef<UniformBuffer>(*)(uint32_t, uint32_t);
        using Texture2DCreator = TRef<Texture2D>(*)(const TextureSpecification&);
        using ShaderCreator = TRef<Shader>(*)(const std::string&, const std::string&, const std::string&);
        using FramebufferCreator = TRef<Framebuffer>(*)(const FramebufferSpecification&);
        using VertexBufferCreator = TRef<VertexBuffer>(*)(float*, uint32_t);
        using IndexBufferCreator = TRef<IndexBuffer>(*)(uint32_t*, uint32_t);

        static VertexArrayCreator s_VertexArrayCreator;
        static UniformBufferCreator s_UniformBufferCreator;
        static Texture2DCreator s_Texture2DCreator;
        static ShaderCreator s_ShaderCreator;
        static FramebufferCreator s_FramebufferCreator;
        static VertexBufferCreator s_VertexBufferCreator;
        static IndexBufferCreator s_IndexBufferCreator;

        static RHI_API s_API;

	};
}