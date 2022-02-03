#pragma once
#include  "Sas\Layer.h"
#include "Sas\Events\KeyEvent.h"
#include "Sas\Events\MouseEvent.h"
#include "Sas\Events\ApplicationEvent.h"
namespace Sas {
	class SAS_API ImGuiLayer : public Layer {
	public:
		ImGuiLayer();
		~ImGuiLayer();
		void OnAttach() override;
		void OnDetach() override;
		void OnUpdate() override;
		void OnEvent(Event& event) override;
	private:
		bool OnButtonPresedEvent(MouseButtonPressedEvent& event);
		bool OnButtonReleasedEvent(MouseButtonReleasedEvent& event);
		bool OnMouseMovedEvent(MouseMovedEvent& event);
		bool OnMouseScrolledEvent(MouseScrolledEvent& event);
		bool OnKeyPressedEvent(KeyPressedEvent& event);
		bool OnKeyReleasedEvent(KeyReleasedEvent& event);
		bool OnKeyTypedEvent(KeyTypedEvent& event);
		bool OnWindowResizeEvent(WindowResizeEvent& event);

	private:
		float m_Time;
	};
}