 #pragma once
#include "Sas/Core/Core.h"
#include "Sas/Core/KeyCodes.h"
#include "Sas/Core/MouseCodes.h"
namespace Sas {
	class SAS_API Input {
	public:
		 static bool IsKeyPressed(KeyCode kode);
		
		 static bool IsMouseButtonPressed(MouseCode button);
		 static bool IsMouseButtonRepeat(MouseCode button);

		 static float GetMouseX();
		 static float GetMouseY();
		 static std::pair<float, float> GetMousePosition();
	};
}