#include "ssph.h"
#include "VertexArray.h"
#include "Renderer.h"

#include "Platform/OpenGL/OpenGLVertexArray.h"
namespace Sas {
	Ref<VertexArray> VertexArray::Create()
	{

		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None: SS_CORE_ASSERT(false, "RendererAPI::None is currently not supported "); return nullptr;
		case RendererAPI::API::Vulcak: SS_CORE_ASSERT(false, "RendererAPI::Vulcak is currently not supported "); return nullptr;
		case RendererAPI::API::DirectX: SS_CORE_ASSERT(false, "RendererAPI::DirectX is currently not supported "); return nullptr;
		case RendererAPI::API::OpenGL: return CreateRef<OpenGLVertexArray>();
		}	
		SS_CORE_ASSERT(false, "Unknown RendererAPI! ");
		return nullptr;

	}
}
