	#include <SasPre.h>
#include <Sas/Core/EntryPoint.h>

#include "EditorLayer.h"

namespace Sas {
	class SasEditor : public Sas::Application
	{
	public:
		SasEditor(ApplicationCommandLineArgs args)
			: Application("SasEditor", args)
		{
			PushLayer(new EditorLayer());
		}

		~SasEditor()
		{
		}
	};

	Application* CreateApplication(ApplicationCommandLineArgs args)
	{
		return new SasEditor(args);
	}
}