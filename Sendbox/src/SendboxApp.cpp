#include <SasPre.h>
#include <Sas/Core/EntryPoint.h>

#include "SandBox2D.h"


class Sandbox : public Sas::Application
{
public:
	Sandbox()
	{
		PushLayer(new SandBox2D());
	}

	~Sandbox()
	{
	}
};

Sas::Application* Sas::CreateApplication()
{
	return new Sandbox();
}