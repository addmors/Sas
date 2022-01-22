#pragma once


#include "Core.h"
namespace Sas {

	class SAS_API Application
	{
	public:
		Application();	
		virtual ~Application();
	
		void Run();
		

	};


	//To be difene in CLIENT
	Application* CreateApplication();
}