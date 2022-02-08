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
		SS_CORE_INFO("OpenGL: "); 
		SS_CORE_INFO("\tVendor:  {0}", glGetString(GL_VENDOR));
		SS_CORE_INFO("\tRederer: {0}", glGetString(GL_RENDERER));
		SS_CORE_INFO("\tVersion: {0}", glGetString(GL_VERSION));

	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
	}

}