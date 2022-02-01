#include "ssph.h"
#include "Aplication.h"
#include "Events/ApplicationEvent.h"



#define BIND_EVENT_FUNC(x) std::bind(&Application::x, this, std::placeholders::_1)

namespace Sas {
	Application::~Application()
	{
	}

	void Application::Run()
	{
		while (m_Running) {
			for (Layer* layer : m_LayerStack){
				layer->OnUpdate();
			}

			m_Window->OnUpdate();
		}; 
	}

	void Application::onEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FUNC(OnWindowClosed));
		SS_TRACE("{0}", e);

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();) {
			(*--it)->OnEvent(e);
			if (e.Handled)
				break;
		}
	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
	}

	void Application::PushOverlay(Layer* layer)
	{
		m_LayerStack.PushOverlay(layer);
	}

	bool Application::OnWindowClosed(WindowCloseEvent& e)
	{
		m_Running = false;

		return true;
	}

	Application::Application()
	{

		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallBack(BIND_EVENT_FUNC(onEvent));
	}

}