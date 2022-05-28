#include "ssph.h"
#include "Sas/Core/Application.h"

#include "Sas/Core/Log.h"

#include "Sas/Renderer/Renderer.h"

#include "Sas/Core/Input.h"

#include <glfw/glfw3.h>

namespace Sas {

	Application* Application::s_Instance = nullptr;

	Application::Application(const std::string& name)
	{
		SS_PROFILE_FUNCTION();

		SS_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;
		m_Window = Window::Create(WindowProps(name));
		m_Window->SetEventCallback(SS_BIND_EVENT_FN(Application::OnEvent));

		Renderer::Init();

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);
	}

	Application::~Application()
	{

		SS_PROFILE_FUNCTION();
		Renderer::Shutdown();
	}

	void Application::Close()
	{
		m_Running = false;
	}

	void Application::PushLayer(Layer* layer)
	{
		SS_PROFILE_FUNCTION();
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* layer)
	{
		SS_PROFILE_FUNCTION();
		m_LayerStack.PushOverlay(layer);
		layer->OnAttach();
	}

	void Application::OnEvent(Event& e)
	{
		SS_PROFILE_FUNCTION();

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(SS_BIND_EVENT_FN(Application::OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(SS_BIND_EVENT_FN(Application::OnWindowResize));

		for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it)
		{
			(*it)->OnEvent(e);
			if (e.Handled)
				break;
		}
	}

	void Application::Run()
	{

		SS_PROFILE_FUNCTION();

		while (m_Running)
		{

			SS_PROFILE_SCOPE("Run Loop");
			float time = (float)glfwGetTime();
			Timestep timestep = time - m_LastFrameTime;
			m_LastFrameTime = time;

			if (!m_Minimized)
			{
				{
					SS_PROFILE_SCOPE("Layer::OnUpdate");
					for (Layer* layer : m_LayerStack)
						layer->OnUpdate(timestep);
					
				}
				{
					SS_PROFILE_SCOPE("Layer::OnImGUIRender");
					m_ImGuiLayer->Begin();
					for (Layer* layer : m_LayerStack)
						layer->OnImGuiRender();
					m_ImGuiLayer->End();
				}

			}

			m_Window->OnUpdate();
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}

	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
		SS_PROFILE_FUNCTION();
		if (e.GetWidth() == 0 || e.GetHeight() == 0)
		{
			m_Minimized = true;
			return false;
		}

		m_Minimized = false;
		Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());

		return false;
	}

}
