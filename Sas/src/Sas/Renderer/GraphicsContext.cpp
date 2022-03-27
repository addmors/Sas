#include "ssph.h"
#include "GraphicsContext.h"
#include "Platform\OpenGL\OpenGLContext.h"
#include "Renderer.h"


namespace Sas {
	Scope<GraphicsContext> GraphicsContext::Create(GLFWwindow* window)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None: SS_CORE_ASSERT(false, "RendererAPI::None is currently not supported "); return nullptr;
		case RendererAPI::API::Vulcak: SS_CORE_ASSERT(false, "RendererAPI::Vulcak is currently not supported "); return nullptr;
		case RendererAPI::API::DirectX: SS_CORE_ASSERT(false, "RendererAPI::DirectX is currently not supported "); return nullptr;
		case RendererAPI::API::OpenGL: return CreateScope<OpenGLContext>(window);
		}
		SS_CORE_ASSERT(false, "Unknown RendererAPI! ");
		return nullptr;
	}
}
