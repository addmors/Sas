#include "ssph.h"
#include "Renderer.h"

namespace Sas {
	void Renderer::BeginScene()
	{
	}
	void Renderer::EndScene()
	{
	}
	void Renderer::Submit(const std::shared_ptr<VertexArray>& vertexArray)
	{
		vertexArray->Bind();
		RendererComand::DrawIndexed(vertexArray);
	}
}