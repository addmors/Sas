#include "ssph.h"
#include "OpenGLContext.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>


namespace Sas {

	OpenGLContext::OpenGLContext(GLFWwindow* window)
		:m_WindowHandle(window)
	{
		SS_CORE_ASSERT(window, "windowHandle is Null");
	}

	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		SS_CORE_ASSERT(status, "Failed to initialize Glad!");

		SS_CORE_INFO("OpenGL Info:");
		SS_CORE_INFO("  Vendor: {0}", glGetString(GL_VENDOR));
		SS_CORE_INFO("  Renderer: {0}", glGetString(GL_RENDERER));
		SS_CORE_INFO("  Version: {0}", glGetString(GL_VERSION));

		SS_CORE_ASSERT(GLVersion.major > 4 || (GLVersion.major == 4 && GLVersion.minor >= 5), "Hazel requires at least OpenGL version 4.5!");

	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
	}

}