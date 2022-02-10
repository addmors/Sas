#include <SasPre.h>
#include "imgui/imgui.h"

class ExampleLayer : public Sas::Layer {
public:
	ExampleLayer() : Layer("Example") 
	{
	};
	
	void OnUpdate() override { 
		if (Sas::Input::IsKeyPressed(Sas::Key::Tab))
			SS_CORE_TRACE("Key Tab Press(pull)");
	}
	virtual void OnImGuiRender() override {
		ImGui::Begin("Test");
		ImGui::Text("Hello World");
		ImGui::ColorEdit4("", new float[4]);
		ImGui::End();


	};

	void OnEvent(Sas::Event& e) override {
		
		if (e.GetEventType() == Sas::EventType::KeyPressed) {
			Sas::KeyPressedEvent& event = (Sas::KeyPressedEvent&)e;
			SS_CORE_TRACE("{0}", (char)event.GetKeyKode());
			

			if (Sas::Input::IsKeyPressed(Sas::Key::RightAlt)) {
				SS_CORE_TRACE("Key Right alt Press(event)");

			}
			else if (Sas::Input::IsKeyPressed(Sas::Key::LeftAlt)) {
				SS_CORE_TRACE("Key Left alt Press(event)");

			}
			else if (Sas::Input::IsKeyPressed(Sas::Key::Tab)) {
				SS_CORE_TRACE("Key Tab Press(event)");
			}
			else SS_CORE_TRACE("{0}", (char)event.GetKeyKode()); 
		}
	};
};

class Sendbox : public Sas::Application {
public:

	Sendbox() {
		PushLayer(new ExampleLayer());
	}
	
	~Sendbox() {
	}

};

Sas::Application* Sas::CreateApplication() {
	return new Sendbox();
}