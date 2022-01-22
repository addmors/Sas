#pragma once

#ifdef SS_PLATFORM_WINDOWS
	extern Sas::Application* Sas::CreateApplication();


	int main(int argc, char** argv) {


		Sas::Log::Init();
		
		SS_CORE_WARN("Welcome To Sas Engine");
		int a = 5;
		SS_INFO("App Is Started {0}", a);

		auto  app = Sas::CreateApplication();

		app->Run();

		delete app;
	}

#endif