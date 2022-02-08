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
		SS_CORE_ASSERT(status, "Failed to Init GLAD!!");
	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
	}

}