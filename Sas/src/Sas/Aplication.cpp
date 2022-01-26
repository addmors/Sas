#include "Aplication.h"
#include "Sas\Events\ApplicaionEvent.h"
#include "Sas\Events\KeyEvent.h"
#include "Sas\Log.h"
namespace Sas {
	Sas::Application::~Application()
	{
	}

	void Application::Run()
	{
		WindowResizeEvent e(1280, 720);
		SS_TRACE(e);
		KeyPressedEvent k('w', 2);
		SS_TRACE(k);

		while (true){}; 
	}

	Sas::Application::Application()
	{
	}
}