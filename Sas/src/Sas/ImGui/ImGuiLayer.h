#pragma once
#include  "Sas\Layer.h"
#include "Sas\Events\KeyEvent.h"
#include "Sas\Events\MouseEvent.h"

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
		void OnButtonPresedEvent(MouseButtonPressedEvent& event);
		void OnButtonReleasedEvent(MouseButtonReleasedEvent& event);
		void OnMouseMovedEvent(MouseMovedEvent& event);
		void OnMouseScrolledEvent(MouseScrolledEvent& event);
	private:
		float m_Time;
	};
}