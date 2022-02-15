#include "ssph.h"
#include "Aplication.h"
#include "Events/ApplicationEvent.h"
#include "Input.h"

#include "ImGui/ImGuiLayer.h"
#include "Sas/Renderer/Renderer.h"




namespace Sas {


#define BIND_EVENT_FUNC(x) std::bind(&Application::x, this, std::placeholders::_1)
	
	Application* Application::s_Instanse = nullptr;

	Application::~Application()
	{
	}

	void Application::Run()
	{
		while (m_Running) {
			
			


			RendererComand::SetClearColor({ 0.2f, 0.2f, 0.2f, 1.0f });
			RendererComand::Clear();

			m_Camera.SetRotation(45.0f);
			Renderer::BeginScene(m_Camera);

			
			Renderer::Submit(m_Shader2,m_SquareVA);
			Renderer::Submit(m_Shader,m_VertexArray);

			Renderer::EndScene();



			for (Layer* layer : m_LayerStack){
				layer->OnUpdate();
			}
			//auto[x,y] = Input::GetMousePosition();
			//SS_CORE_TRACE("{0},{1}", x,y);

			
			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack) {
				layer->OnImGuiRender();
			}
			m_ImGuiLayer->End();

			m_Window->OnUpdate();
		}; 
	}

	void Application::onEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FUNC(OnWindowClosed));
		//SS_TRACE("{0}", e);

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();) {
			(*--it)->OnEvent(e);
			if (e.Handled)
				break;
		}
	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* layer)
	{
		m_LayerStack.PushOverlay(layer);
		layer->OnAttach();
	}

	bool Application::OnWindowClosed(WindowCloseEvent& e)
	{
		m_Running = false;

		return true;
	}

	Application::Application()
		:m_Camera(-1.6f, 1.6f, -0.9f,0.9f)
	{
		SS_CORE_ASSERT(!s_Instanse, "App Is Already Exist");
		s_Instanse = this;
		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallBack(BIND_EVENT_FUNC(onEvent));

		m_ImGuiLayer = new ImGuiLayer();

		PushOverlay(m_ImGuiLayer);

		
		float vertices[3 * 7] = {
	   -0.5f, -0.8f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		0.5f, -0.8f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
		0.0f,  0.8f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f
		};


		uint32_t indices[3] = { 0,1,2 };

		m_VertexArray.reset(VertexArray::Create());

		m_VertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));

		m_VertexBuffer->SetLayout({
			{ShaderDataType::Float3, "a_Position"},
			{ShaderDataType::Float4, "a_Color"},
			});
		m_VertexArray->AddVertexBufer(m_VertexBuffer);


		m_IndexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices)/sizeof(uint32_t)));
		m_VertexArray->SetIndexBufer(m_IndexBuffer);



		float sqvertices[3 * 4] = {
	   -0.5f, -0.5f, 0.0f, 
		0.5f, -0.5f, 0.0f, 
		0.5f,  0.5f, 0.0f, 
	   -0.5f,  0.5f, 0.0f 
		};


		m_SquareVA.reset(VertexArray::Create());
		
		std::shared_ptr<VertexBuffer> sqareVB;
		sqareVB.reset(VertexBuffer::Create(sqvertices, sizeof(sqvertices)));
		sqareVB->SetLayout({
			{ShaderDataType::Float3, "a_Position"},
			});

		m_SquareVA->AddVertexBufer(sqareVB);



		uint32_t sqindices[6] = { 0,1,2,2, 3,0};

		std::shared_ptr<IndexBuffer> sqareIB; 
		sqareIB.reset(IndexBuffer::Create(sqindices, sizeof(sqindices) / sizeof(uint32_t)));
		
		m_SquareVA->SetIndexBufer(sqareIB);


		std::string vertSourse = R"(

			#version 450 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			uniform mat4 u_ViewProjectionMatrix;
 
			out vec3 v_Pos;
			out vec4 v_Color;

			void main(){
				v_Color = a_Color;
				v_Pos = a_Position;
				gl_Position = u_ViewProjectionMatrix*vec4(a_Position, 1.0);
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


		m_Shader.reset(new Shader(vertSourse, fragSourse));


		std::string vertSourse2 = R"(

			#version 450 core
			
			layout(location = 0) in vec3 a_Position;
			
			uniform mat4 u_ViewProjectionMatrix;

			out vec3 v_Pos;

			void main(){
				v_Pos = a_Position;
				gl_Position = u_ViewProjectionMatrix * vec4(a_Position, 1.0);
			}
		)";

		std::string fragSourse2 = R"(

			#version 450 core
			layout(location = 0) out vec4 color;
			in vec3 v_Pos;



			void main(){
				color = vec4(v_Pos*0.5+0.5, 1.0);
			}
		)";

		m_Shader2.reset(new Shader(vertSourse2, fragSourse2));
	}

}