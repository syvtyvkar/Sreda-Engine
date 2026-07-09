#pragma once

#include "Engine/Render/GraphicsContext.h"
#include "Engine/Platform/PlatformTypes.h"
#include "Engine/Platform/IWindow.h"
#include <map>

struct GLFWwindow;

namespace Engine::Render
{
    class OpenGLContext : public GraphicsContext
	{
	public:
		OpenGLContext(GLFWwindow* windowHandle);

		virtual void Init() override;
		virtual void SwapBuffers() override;
		virtual void MakeCurrent() override;

	private:
		GLFWwindow* m_WindowHandle;
	};
}