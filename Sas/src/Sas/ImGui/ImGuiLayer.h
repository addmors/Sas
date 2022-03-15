#pragma once
#include "Sas\Core\Layer.h"
#include "Sas\Events\KeyEvent.h"
#include "Sas\Events\MouseEvent.h"
#include "Sas\Events\ApplicationEvent.h"
namespace Sas {
	class SAS_API ImGuiLayer : public Layer {
	public:
		ImGuiLayer();
		~ImGuiLayer();
		virtual void OnAttach() override;
		virtual void OnDetach() override;
		

		void Begin();
		void End();

	private:
		float m_Time;
	};
}