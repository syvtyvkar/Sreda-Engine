#pragma once

#include "Engine/Render/GraphicsContext.h"

struct GLFWwindow;

namespace Engine::Render
{
    class OpenGLContext : public GraphicsContext
	{
	public:
		OpenGLContext(GLFWwindow* windowHandle);

		virtual void Init() override;
		virtual void SwapBuffers() override;
	private:
		GLFWwindow* m_WindowHandle;
	};
}