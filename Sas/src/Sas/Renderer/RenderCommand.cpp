#include "ssph.h"
#include "Sas/Renderer/RenderCommand.h"

namespace Sas {

	Scope<RendererAPI> RenderCommand::s_RendererAPI = RendererAPI::Create();

}