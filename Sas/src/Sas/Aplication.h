#pragma 

#include "Core.h"
#include "Events/Event.h"
#include "Window.h"

namespace Sas {

	class WindowCloseEvent;

	class SAS_API Application
	{
	public:
		Application();	
		virtual ~Application();
	
		void Run();
		void onEvent(Event& e);

	private:
		bool OnWindowClosed(WindowCloseEvent& e );
		std::unique_ptr<Window> m_Window;
		bool m_Running = true;

	};


	//To be difene in CLIENT
	Application* CreateApplication();
}