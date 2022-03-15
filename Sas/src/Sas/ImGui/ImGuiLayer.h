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

		void Begin();
		void End();
	private:
		float m_Time = 0.0f;
	};

}