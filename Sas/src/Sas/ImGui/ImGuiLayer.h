#pragma once

#include "Sas/Core/Layer.h"

#include "Sas/Events/ApplicationEvent.h"
#include "Sas/Events/KeyEvent.h"
#include "Sas/Events/MouseEvent.h"

namespace Sas {

	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnEvent(Event& e) override;
		void Begin();
		void End();
	
		void BlockEvents(bool block) { m_BlockEvents = block; }

		void SetDarkThemeColors();
	private:
		bool m_BlockEvents = true;
		float m_Time = 0.0f;
	};

}