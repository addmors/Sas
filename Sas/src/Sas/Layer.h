#pragma once
#include "Sas\Core.h"
#include "Sas\Events\Event.h"

namespace Sas {

	class SAS_API Layer
	{
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer();

		virtual void OnAttach() {};
		virtual void OnDetach() {};
		virtual void OnUpdate() {};
		virtual void OnImGuiRender() {};

		virtual void OnEvent(Event& event) {};

		inline const std::string& GetNam() { return m_DebugName; };
	protected:
		std::string m_DebugName;
	};
}

