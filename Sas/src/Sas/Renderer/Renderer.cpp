#include "ssph.h"
#include "Renderer.h"

namespace Sas {

	Renderer::SceneData* Renderer::m_SceneData = new Renderer::SceneData;

	void Renderer::Init()
	{
		RendererComand::Init();
	}

	void Renderer::BeginScene(OrthographicCamera& camera)
	{
		m_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
	}
	void Renderer::EndScene()
	{
	}
	void Renderer::Submit( const Ref<Shader> shader, const Ref<VertexArray>& vertexArray, const glm::mat4& u_Transform)
	{
		shader->Bind();
		shader->SetMat4("u_ViewProjectionMatrix", m_SceneData->ViewProjectionMatrix);	
		shader->SetMat4("u_Transform", u_Transform);
		
		vertexArray->Bind();
		RendererComand::DrawIndexed(vertexArray);
	}
}