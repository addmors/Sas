#pragma 
#include "Core.h"
#include "Events\Event.h"
#include "Window.h"
namespace Sas {

	class SAS_API Application
	{
	public:
		Application();	
		virtual ~Application();
	
		void Run();
		
	private:

		std::unique_ptr<Window> m_Window;
		bool m_Running = true;

	};


	//To be difene in CLIENT
	Application* CreateApplication();
}