#include <SasPre.h>
class ExampleLayer : public Sas::Layer {
public:
	ExampleLayer() : Layer("Example") {};
	
	void OnUpdate() override { 
		if (Sas::Input::IsKeyPressed(Sas::Key::Tab))
			SS_CORE_TRACE("Key Tab Press");
	}

	void OnEvent(Sas::Event& e) override {
		
		if (e.GetEventType() == Sas::EventType::KeyPressed) {
			Sas::KeyPressedEvent& event = (Sas::KeyPressedEvent&)e;
			SS_CORE_TRACE("{0}", (char)event.GetKeyKode());
		}
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