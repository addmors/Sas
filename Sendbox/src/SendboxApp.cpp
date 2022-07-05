#include <SasPre.h>
#include <Sas/Core/EntryPoint.h>

#include "SandBox2D.h"

namespace Sas {
	class Sandbox : public Sas::Application
	{
	public:
		Sandbox(ApplicationCommandLineArgs args)
			: Application("Hazelnut", args)
		{
			PushLayer(new SandBox2D());
		}

		~Sandbox()
		{
		}
	};

	Application* CreateApplication(ApplicationCommandLineArgs args)
	{
		return new Sandbox(args);
	}
}