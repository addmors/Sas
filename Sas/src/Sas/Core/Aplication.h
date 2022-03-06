#pragma once 

#include "Core.h"

#include "Window.h"
#include "LayerStack.h"
#include "Sas/Events/Event.h"
#include "Sas/Events/ApplicationEvent.h"
#include "Sas/Core/Timestep.h"

namespace Sas {

	class ImGuiLayer;

	class SAS_API Application
	{
	public:
		Application();	
		virtual ~Application();
		
		void Run();
		void onEvent(Event& e);


		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);
		
		inline static Application& Get() { return *s_Instanse; };
		inline Window& GetWindow() {
			return *m_Window;
		};

	private:

		bool OnWindowResize(WindowResizeEvent& e);
		bool OnWindowClosed(WindowCloseEvent& e);

	private:
		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Minimized = false;
		bool m_Running = true;
		LayerStack m_LayerStack;
		float m_LastFrameTime = 0.0f;
	private:
		static Application* s_Instanse;

	};


	//To be difene in CLIENT
	Application* CreateApplication();
}