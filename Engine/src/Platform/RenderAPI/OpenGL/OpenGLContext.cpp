
//#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include "OpenGLContext.h"

#include <GLFW/glfw3.h>

namespace Engine::Render
{
    OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		: m_WindowHandle(windowHandle)
	{
		ENGINE_ASSERT(windowHandle, "Window handle is null!");
	}

	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		ENGINE_ASSERT(status, "Failed to initialize Glad!");

		ENGINE_LOG_INFO("OpenGL Info:");
		ENGINE_LOG_INFO("---- Vendor: {0}", *glGetString(GL_VENDOR));
		ENGINE_LOG_INFO("---- Renderer: {0}", *glGetString(GL_RENDERER));
		ENGINE_LOG_INFO("---- Version: {0}", *glGetString(GL_VERSION));

		ENGINE_ASSERT(GLVersion.major > 4 || (GLVersion.major == 4 && GLVersion.minor >= 5), "Engine requires at least OpenGL version 4.5!");

		//#ifdef DEBUG
    	glEnable(GL_DEBUG_OUTPUT);
   	 	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    	glDebugMessageCallback([](GLenum source, GLenum type, GLuint id, GLenum severity,GLsizei length, const GLchar* message, const void* userParam) 
		{
        	if (type == GL_DEBUG_TYPE_ERROR) 
			{
            	ENGINE_LOG_CRITICAL("GL_DEBUG: [{}] {}", id, message);
        	}
    	}, nullptr);
		//#endif
	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
	}
}