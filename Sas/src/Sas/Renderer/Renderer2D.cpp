#include "ssph.h"
#include "Renderer2D.h"
#include "VertexArray.h"
#include "RendererComand.h"
#include "Shader.h"
namespace Sas {
	struct Renderer2DStorage
	{
		Ref<VertexArray> QuadVertexArray;
		Ref<Shader> FlatColorShader;

	};

	static Renderer2DStorage* s_Data;

	void Renderer2D::Init()
	{
		s_Data = new Renderer2DStorage();
		float sqvertices[3 * 4] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.5f,  0.5f, 0.0f,
		-0.5f,  0.5f, 0.0f
		};


		s_Data->QuadVertexArray = VertexArray::Create();

		Ref<VertexBuffer> sqareVB;
		sqareVB = VertexBuffer::Create(sqvertices, sizeof(sqvertices));
		sqareVB->SetLayout({
			{Sas::ShaderDataType::Float3, "a_Position"},
			});

		s_Data->QuadVertexArray->AddVertexBufer(sqareVB);



		uint32_t sqindices[6] = { 0,1,2,2, 3,0 };

		Ref<Sas::IndexBuffer> sqareIB;
		sqareIB.reset(IndexBuffer::Create(sqindices, sizeof(sqindices) / sizeof(uint32_t)));

		s_Data->QuadVertexArray->SetIndexBufer(sqareIB);
		s_Data->FlatColorShader = Shader::Create("assets/shader/FlatColor.glsl");
	}

	void Renderer2D::ShotDown()
	{
		delete s_Data;
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		s_Data->FlatColorShader->Bind();
		s_Data->FlatColorShader->SetMat4("u_ViewProjectionMatrix", camera.GetViewProjectionMatrix());
		s_Data->FlatColorShader->SetMat4("u_Transform", glm::mat4(1.0f));

	}

	void Renderer2D::EndScene()
	{

	}

	void Renderer2D::DrawQuad(const glm::vec2& pos, const glm::vec2& size, const glm::vec4& color)
	{
		DrawQuad({ pos.x, pos.y, 0.0f }, size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& pos, const glm::vec2& size, const glm::vec4& color)
	{

		s_Data->FlatColorShader->Bind();
		s_Data->FlatColorShader->SetFloat4("u_Color", color);

		s_Data->QuadVertexArray->Bind();
		RendererComand::DrawIndexed(s_Data->QuadVertexArray);
	}

}