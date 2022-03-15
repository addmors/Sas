#include <SasPre.h>
#include <Sas/Core/EntryPoint.h>

#include "GameLayer.h"

class Sandbox : public Sas::Application
{
public:
	Sandbox()
	{
		PushLayer(new GameLayer());
	}

	~Sandbox()
	{
	}
};

Sas::Application* Sas::CreateApplication()
{
	return new Sandbox();
}