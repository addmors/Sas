#pragma once

#ifdef SS_PLATFORM_WINDOWS
	extern Sas::Application* Sas::CreateApplication();


	int main(int argc, char** argv) {

		std::cout << "Whaaat??";
		auto  app = Sas::CreateApplication();

		app->Run();

		delete app;
	}

#endif