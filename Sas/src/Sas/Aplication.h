#pragma once 

#include "Core.h"

#include "Window.h"
#include "LayerStack.h"
#include "Sas/Events/Event.h"

namespace Sas {

	class WindowCloseEvent;

	class SAS_API Application
	{
	public:
		Application();	
		virtual ~Application();
		
		void Run();
		void onEvent(Event& e);


		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

	private:
		bool OnWindowClosed(WindowCloseEvent& e );
		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
		LayerStack m_LayerStack;
	};


	//To be difene in CLIENT
	Application* CreateApplication();
}