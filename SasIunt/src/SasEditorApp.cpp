#include <SasPre.h>
#include <Sas/Core/EntryPoint.h>

#include "EditorLayer.h"

namespace Sas {
	class SasEditor : public Sas::Application
	{
	public:
		SasEditor()
			:Application("Sas Editor")
		{
			PushLayer(new EditorLayer());
		}

		~SasEditor()
		{
		}
	};

	Sas::Application* CreateApplication()
	{
		return new SasEditor();
	}
}