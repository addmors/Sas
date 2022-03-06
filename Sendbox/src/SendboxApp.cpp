#include <SasPre.h>

#include <Sas/Core/EntryPoint.h>
#include "imgui/imgui.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "Sas/Renderer/Shader.h"

#include "SandBox2D.h"
class ExampleLayer : public Sas::Layer {
public:
	ExampleLayer() 
		:Layer("Example"), m_CameraController(1280.0f/ 720.0f, true),m_CameraPos(0.0f)
	{
		float vertices[3 * 7] = {
		   -0.5f, -0.8f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			0.5f, -0.8f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
			0.0f,  0.8f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f
		};


		uint32_t indices[3] = { 0,1,2 };

		m_VertexArray = Sas::VertexArray::Create();
		Sas::Ref<Sas::VertexBuffer> m_VertexBuffer;
		m_VertexBuffer = Sas::VertexBuffer::Create(vertices, sizeof(vertices));

		m_VertexBuffer->SetLayout({
			{Sas::ShaderDataType::Float3, "a_Position"},
			{Sas::ShaderDataType::Float4, "a_Color"},
			});
		m_VertexArray->AddVertexBufer(m_VertexBuffer);

		Sas::Ref<Sas::IndexBuffer> m_IndexBuffer;
		m_IndexBuffer.reset(Sas::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBufer(m_IndexBuffer);



		float sqvertices[5 * 4] = {
	   -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
		0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
	   -0.5f,  0.5f, 0.0f, 0.0f, 1.0f 
		};


		m_SquareVA = Sas::VertexArray::Create();

		Sas::Ref<Sas::VertexBuffer> sqareVB;
		sqareVB = Sas::VertexBuffer::Create(sqvertices, sizeof(sqvertices));
		sqareVB->SetLayout({
			{Sas::ShaderDataType::Float3, "a_Position"},
			{Sas::ShaderDataType::Float2, "a_TexCoord"},
			});

		m_SquareVA->AddVertexBufer(sqareVB);



		uint32_t sqindices[6] = { 0,1,2,2, 3,0 };

		Sas::Ref<Sas::IndexBuffer> sqareIB;
		sqareIB.reset(Sas::IndexBuffer::Create(sqindices, sizeof(sqindices) / sizeof(uint32_t)));

		m_SquareVA->SetIndexBufer(sqareIB);


		std::string vertSourse = R"(

			#version 450 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			uniform mat4 u_ViewProjectionMatrix;
 			uniform mat4 u_Transform;

			out vec3 v_Pos;
			out vec4 v_Color;

			void main(){
				v_Color = a_Color;
				v_Pos = a_Position;

				gl_Position = u_ViewProjectionMatrix * u_Transform * vec4(a_Position, 1.0);
			}
		)";

		std::string fragSourse = R"(

			#version 450 core
			layout(location = 0) out vec4 color;			
			in vec3 v_Pos;
			in vec4 v_Color;

			void main(){
				color = v_Color;
			}
		)";


		m_Shader = Sas::Shader::Create("first shader", vertSourse, fragSourse);


		std::string vertSourse2 = R"(

			#version 450 core
			
			layout(location = 0) in vec3 a_Position;
			
			uniform mat4 u_ViewProjectionMatrix;
			uniform mat4 u_Transform;


			void main(){
				gl_Position = u_ViewProjectionMatrix * u_Transform * vec4(a_Position, 1.0);
			}
		)";

		std::string fragSourse2 = R"(

			#version 450 core
			layout(location = 0) out vec4 color;

			uniform vec3 u_Color;


			void main(){
				color = vec4(u_Color,1.0f);
			}
		)";

		m_Shader2 = Sas::Shader::Create("Seconds Shader", vertSourse2, fragSourse2);


		auto m_TextureShader = m_ShaderLibrary.Load("assets/shader/Texture.glsl");

		m_TextureShader->Bind();
		m_TextureShader->SetInt("u_Texture", 0);


		m_Texture = Sas::Texture2D::Create("assets/textures/Checkerboard.png");
		m_TextureShild = Sas::Texture2D::Create("assets/textures/Logoshild.png");

	};
	
	void OnUpdate(Sas::Timestep ts) override { 

		m_CameraController.OnUpdate(ts);
		Sas::RendererComand::SetClearColor({ 0.2f, 0.2f, 0.2f, 1.0f });
		Sas::RendererComand::Clear();
		
		Sas::Renderer::BeginScene(m_CameraController.GetCamera());

		//Submit Triangle
		//Sas::Renderer::Submit(m_Shader, m_VertexArray, glm::mat4(1.0f));

		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f, 0.1f, 0));

		m_Shader2->Bind();
		m_Shader2->SetFloat3("u_Color", m_SquareColor);
		for (size_t x = 0; x < 20; x++)
		{
			for (size_t y = 0; y < 20; y++)
			{
				glm::vec3 _pos{ x * 0.11, y * 0.11, 0.0f};
				glm::mat4 trans = glm::translate(glm::mat4(1.0f), _pos) * scale;
				Sas::Renderer::Submit(m_Shader2, m_SquareVA, trans);
			}
		}
		
		auto m_TextureShader = m_ShaderLibrary.Get("Texture");
		m_Texture->Bind();
		Sas::Renderer::Submit(m_TextureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

		m_TextureShild->Bind();
		Sas::Renderer::Submit(m_TextureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));


		Sas::Renderer::EndScene();


	}
	virtual void OnImGuiRender() override {
		ImGui::Begin("Test");
		ImGui::ColorEdit3("Squares color", glm::value_ptr(m_SquareColor));
		ImGui::End();

	};

	void OnEvent(Sas::Event& e) override {
	
		m_CameraController.OnEvent(e);
	};

private:
	Sas::ShaderLibrary m_ShaderLibrary;
	Sas::Ref<Sas::Shader> m_Shader;
	Sas::Ref<Sas::VertexArray> m_VertexArray;
	Sas::Ref<Sas::VertexArray> m_SquareVA;

	Sas::Ref<Sas::Texture2D> m_Texture, m_TextureShild;
	Sas::Ref<Sas::Shader> m_Shader2;
	Sas::OrthographicCameraController m_CameraController;
	glm::vec3 m_CameraPos;

	glm::vec3 m_SquareColor = { 0.2f,0.3f,0.8f };
};

class Sendbox : public Sas::Application {
public:

	Sendbox() {
		//PushLayer(new ExampleLayer());
		PushLayer(new SandBox2D());

	}
	
	~Sendbox() {
	}

};

Sas::Application* Sas::CreateApplication() {
	return new Sendbox();
}