#include "ssph.h"
#include "Aplication.h"
#include "Events/ApplicationEvent.h"
#include "Input.h"

#include "ImGui/ImGuiLayer.h"
#include "Sas/Renderer/Renderer.h"


#include "GLFW/glfw3.h"


namespace Sas {


#define BIND_EVENT_FUNC(x) std::bind(&Application::x, this, std::placeholders::_1)
	
	Application* Application::s_Instanse = nullptr;

	Application::~Application()
	{
	}

	void Application::Run()
	{
		while (m_Running) {
	
			float time = (float)glfwGetTime();

			Timestep timestep{time - m_LastFrameTime};

			m_LastFrameTime = time;

			for (Layer* layer : m_LayerStack){
				layer->OnUpdate(timestep);
			}
			
			
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
		m_Window->SetVSync(false);

		m_ImGuiLayer = new ImGuiLayer();

		PushOverlay(m_ImGuiLayer);
	}

}