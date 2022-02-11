#pragma once

#include "Sas/Renderer/GraphicsContext.h"

struct GLFWwindow;

namespace Sas {
	class OpenGLContext : public GraphicsContext
	{
	public:
		OpenGLContext(GLFWwindow* window);
		virtual void Init() override;
		virtual void SwapBuffers() override;
	private:
		GLFWwindow* m_WindowHandle;
	};

}
