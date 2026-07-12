
//#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include "OpenGLContext.h"

#include <GLFW/glfw3.h>

#include "Engine/Core/Base/EngineCore.h"
#include <type_traits>

namespace Engine::Render
{
    OpenGLContext::OpenGLContext(TWeak<IWindow> InWindowOwner)
		: m_windowOwner(InWindowOwner)
	{
		ENGINE_ASSERT(m_windowOwner.lock(), "Window handle is null!");
	}

	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(static_cast<GLFWwindow*>(m_windowOwner.lock().get()->GetWindowAPIHandle()));
		
		static bool InitGLLoader = false;
		if (!InitGLLoader)
		{
			int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
			ENGINE_ASSERT(status, "Failed to initialize Glad!");

			ENGINE_LOG_INFO("OpenGL Info:");
			ENGINE_LOG_INFO("---- Vendor: {0}", *glGetString(GL_VENDOR));
			ENGINE_LOG_INFO("---- Renderer: {0}", *glGetString(GL_RENDERER));
			ENGINE_LOG_INFO("---- Version: {0}", *glGetString(GL_VERSION));

			ENGINE_ASSERT(GLVersion.major > 4 || (GLVersion.major == 4 && GLVersion.minor >= 5), "Engine requires at least OpenGL version 4.5!");
			InitGLLoader = true;
		}
	}

    void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(static_cast<GLFWwindow*>(m_windowOwner.lock().get()->GetWindowAPIHandle()));
	}

	void OpenGLContext::MakeCurrent()
	{
		glfwMakeContextCurrent(static_cast<GLFWwindow*>(m_windowOwner.lock().get()->GetWindowAPIHandle()));
	}
}