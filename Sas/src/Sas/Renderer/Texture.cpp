#include "ssph.h"
#include "Texture.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLTexture.h"

namespace Sas {
	Ref<Sas::Texture2D> Texture2D::Create(const std::string& path)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None: SS_CORE_ASSERT(false, "RendererAPI::None is currently not supported "); return nullptr;
		case RendererAPI::API::Vulcak: SS_CORE_ASSERT(false, "RendererAPI::Vulcak is currently not supported "); return nullptr;
		case RendererAPI::API::DirectX: SS_CORE_ASSERT(false, "RendererAPI::DirectX is currently not supported "); return nullptr;
		case RendererAPI::API::OpenGL: return CreateRef<OpenGLTexture2D>(path);
		}
		SS_CORE_ASSERT(false, "Unknown RendererAPI! ");
		return nullptr;
	}
}