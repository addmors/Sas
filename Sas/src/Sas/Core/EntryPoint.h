#pragma once

#ifdef SS_PLATFORM_WINDOWS
extern Sas::Application* Sas::CreateApplication(ApplicationCommandLineArgs args);

	
	int main(int argc, char** argv) {
		Sas::Log::Init();
		SS_PROFILE_BEGIN_SESSION("Startup", "SasProfile-Startup.json");
		auto app = Sas::CreateApplication({ argc, argv });
		SS_PROFILE_END_SESSION();

		SS_PROFILE_BEGIN_SESSION("Startapp", "SasProfile-Runtime.json");
		app->Run();
		SS_PROFILE_END_SESSION();

		SS_PROFILE_BEGIN_SESSION("Startapp", "SasProfile-Shutdown.json");
		delete app;
		SS_PROFILE_END_SESSION();
	}

#endif