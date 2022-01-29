#include "ssph.h"
#include "Aplication.h"

namespace Sas {
	Application::~Application()
	{
	}

	void Application::Run()
	{
		std::cout << 123;
		while (m_Running){
		
			m_Window->OnUpdate();
		}; 
	}

	Application::Application()
	{

		m_Window = std::unique_ptr<Window>(Window::Create());
	}

}