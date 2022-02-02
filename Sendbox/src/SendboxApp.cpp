#include <SasPre.h>

class ExampleLayer : public Sas::Layer {
public:
	ExampleLayer() : Layer("Example") {};
	
	void OnUpdate() override { 
		SS_INFO("EXAMPLE::Update"); 
	}

	void OnEvent(Sas::Event& e) override {
		SS_TRACE("{0}", e);
	};
};

class Sendbox : public Sas::Application {
public:

	Sendbox() {
		PushLayer(new ExampleLayer());
		PushOverlay(new Sas::ImGuiLayer());
	}
	
	~Sendbox() {
	}

};

Sas::Application* Sas::CreateApplication() {
	return new Sendbox();
}