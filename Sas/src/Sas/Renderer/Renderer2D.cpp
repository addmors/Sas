#include "ssph.h"
#include "Renderer2D.h"
#include "VertexArray.h"
#include "RendererComand.h"
#include "Shader.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Sas {
	struct Renderer2DStorage
	{
		Ref<VertexArray> QuadVertexArray;
		Ref<Shader> TextureShader;
		Ref<Texture2D> WhiteTexture;
	};

	static Renderer2DStorage* s_Data;

	void Renderer2D::Init()
	{
		s_Data = new Renderer2DStorage();
		float sqvertices[5 * 4] = {
		-0.5f, -0.5f, 0.0f, 0.0f,0.0f,
		 0.5f, -0.5f, 0.0f, 1.0f,0.0f,
		 0.5f,  0.5f, 0.0f, 1.0f,1.0f,
		-0.5f,  0.5f, 0.0f, 0.0f,1.0f
		};


		s_Data->QuadVertexArray = VertexArray::Create();

		Ref<VertexBuffer> sqareVB;
		sqareVB = VertexBuffer::Create(sqvertices, sizeof(sqvertices));
		sqareVB->SetLayout({
			{Sas::ShaderDataType::Float3, "a_Position"},
			{Sas::ShaderDataType::Float2, "a_TexCoord"},

			});

		s_Data->QuadVertexArray->AddVertexBufer(sqareVB);



		uint32_t sqindices[6] = { 0,1,2,2, 3,0 };

		Ref<Sas::IndexBuffer> sqareIB;
		sqareIB.reset(IndexBuffer::Create(sqindices, sizeof(sqindices) / sizeof(uint32_t)));

		s_Data->QuadVertexArray->SetIndexBufer(sqareIB);

		s_Data->WhiteTexture = Texture2D::Create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		s_Data->WhiteTexture->SetData(&whiteTextureData,sizeof(uint32_t));
		s_Data->TextureShader = Shader::Create("assets/shader/Texture.glsl");
		s_Data->TextureShader->Bind();
		s_Data->TextureShader->SetInt("u_TexCoord", 0);
	}

	void Renderer2D::ShotDown()
	{
		delete s_Data;
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		s_Data->TextureShader->Bind();
		s_Data->TextureShader->SetMat4("u_ViewProjectionMatrix", camera.GetViewProjectionMatrix());
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
		s_Data->TextureShader->SetFloat4("u_Color", color);
		s_Data->WhiteTexture->Bind();

		glm::mat4 trans = glm::translate(glm::mat4(1.0f), pos) * glm::scale(glm::mat4(1.0f), {size.x,size.y, 0});
		s_Data->TextureShader->SetMat4("u_Transform", trans);

		s_Data->QuadVertexArray->Bind();
		RendererComand::DrawIndexed(s_Data->QuadVertexArray);
	}

	void Renderer2D::DrawQuad(const glm::vec2& pos, const glm::vec2& size, const Ref<Texture>& texture)
	{
		DrawQuad({ pos.x, pos.y, 0.0f }, size, texture);

	}

	void Renderer2D::DrawQuad(const glm::vec3& pos, const glm::vec2& size, const Ref<Texture>& texture)
	{

		s_Data->TextureShader->SetFloat4("u_Color", glm::vec4(1.0f));
		texture->Bind();

		glm::mat4 trans = glm::translate(glm::mat4(1.0f), pos) * glm::scale(glm::mat4(1.0f), { size.x,size.y, 0 });
		s_Data->TextureShader->SetMat4("u_Transform", trans);

		s_Data->QuadVertexArray->Bind();
		RendererComand::DrawIndexed(s_Data->QuadVertexArray);
	}

}