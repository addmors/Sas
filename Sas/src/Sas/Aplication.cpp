#include "ssph.h"
#include "Aplication.h"
#include "Events/ApplicationEvent.h"
#include "Input.h"

#include "ImGui/ImGuiLayer.h"

#include "glad\glad.h"




namespace Sas {

	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::Float:    return GL_FLOAT;
		case ShaderDataType::Float2:   return GL_FLOAT;
		case ShaderDataType::Float3:   return GL_FLOAT;
		case ShaderDataType::Float4:   return GL_FLOAT;
		case ShaderDataType::Mat3:     return GL_FLOAT;
		case ShaderDataType::Mat4:     return GL_FLOAT;
		case ShaderDataType::Int:      return GL_INT;
		case ShaderDataType::Int2:     return GL_INT;
		case ShaderDataType::Int3:     return GL_INT;
		case ShaderDataType::Int4:     return GL_INT;
		case ShaderDataType::Bool:     return GL_BOOL;
		}

		SS_CORE_ASSERT(false, "Unknown ShaderDataType!");
		return 0;
	}


	
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


		float vertices[3 * 7] = {
	   -0.5f, -0.8f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		0.5f, -0.8f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
		0.0f,  0.8f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f
		};


		uint32_t indices[3] = { 0,1,2 };

		m_VertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));

		BufferLayout layout = {
			{ShaderDataType::Float3, "a_Position"},
			{ShaderDataType::Float4, "a_Color"},
		};
		uint32_t index = 0;
		m_VertexBuffer->SetLayout(layout);

			for (const auto& element : m_VertexBuffer->GetLayout()) {
				glEnableVertexAttribArray(index);
				glVertexAttribPointer(index,
					element.Size/4, 
					ShaderDataTypeToOpenGLBaseType(element.Type), 
					element.Normalized ? GL_TRUE : GL_FALSE,
					layout.GetStride(),
					(const void*)element.Offset);
				index++;
			};
		
		m_IndexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices)/sizeof(uint32_t)));

		std::string vertSourse = R"(

			#version 450 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;
			
			out vec3 v_Pos;
			out vec4 v_Color;

			void main(){
				v_Color = a_Color;
				v_Pos = a_Position;
				gl_Position = vec4(a_Position, 1.0);
			}
		)";

		std::string fragSourse = R"(

			#version 450 core
			layout(location = 0) out vec4 color;
			in vec3 v_Pos;
			in vec4 v_Color;
			void main(){
				color = vec4(v_Pos*0.5+0.5, 1.0);
				//color = v_Color;
			}
		)";



		m_Shader.reset(new Shader(vertSourse, fragSourse));
	}

}