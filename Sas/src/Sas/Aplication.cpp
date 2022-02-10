#include "ssph.h"
#include "Aplication.h"
#include "Events/ApplicationEvent.h"
#include "Input.h"
#include "glad\glad.h"

#include "ImGui/ImGuiLayer.h"


namespace Sas {

	
#define BIND_EVENT_FUNC(x) std::bind(&Application::x, this, std::placeholders::_1)
	
	Application* Application::s_Instanse = nullptr;

	Application::~Application()
	{
	}

	void Application::Run()
	{
		while (m_Running) {
			
			glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			m_Shader->Bind();

			glBindVertexArray(m_VertexArray);
			glDrawElements(GL_TRIANGLES, m_IndexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr);

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
	{
		SS_CORE_ASSERT(!s_Instanse, "App Is Already Exist");

		s_Instanse = this;
		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallBack(BIND_EVENT_FUNC(onEvent));

		m_ImGuiLayer = new ImGuiLayer();

		PushOverlay(m_ImGuiLayer);

		glGenVertexArrays(1, &m_VertexArray);
		glBindVertexArray(m_VertexArray);


		float vertices[9] = {
		-.5f, -.5f, 0.0f,
		 .5f, -.5f, 0.0f,
		0.0f,  .5f, 0.0f,
		};


		uint32_t index[3] = { 0,1,2 };

		m_VertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

		m_IndexBuffer.reset(IndexBuffer::Create(index, sizeof(index)/sizeof(uint32_t)));

		std::string vertSourse = R"(

			#version 450 core
			
			layout(location = 0) in vec3 pos;
			
			out vec3 v_Pos;

			void main(){
				v_Pos = pos;
				gl_Position = vec4(pos, 1.0);
			}
		)";

		std::string fragSourse = R"(

			#version 450 core
			layout(location = 0) out vec4 color;
			in vec3 v_Pos;
			
			void main(){
				color = vec4(v_Pos*0.5 + 0.5, 1.0);
			}
		)";



		m_Shader.reset(new Shader(vertSourse, fragSourse));
	}

}