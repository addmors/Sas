#include "ssph.h"
#include "VertexArray.h"
#include "Renderer.h"

#include "Platform/OpenGL/OpenGLVertexArray.h"
namespace Sas {
	VertexArray* VertexArray::Create()
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::None: SS_CORE_ASSERT(false, "RendererAPI::None is currently not supported "); return nullptr;
		case RendererAPI::Vulcak: SS_CORE_ASSERT(false, "RendererAPI::Vulcak is currently not supported "); return nullptr;
		case RendererAPI::DirectX: SS_CORE_ASSERT(false, "RendererAPI::DirectX is currently not supported "); return nullptr;
		case RendererAPI::OpenGL: return new OpenGLVertexArray();
		}	
		SS_CORE_ASSERT(false, "Unknown RendererAPI! ");
		return nullptr;

	}
}
