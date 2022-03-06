#pragma once
#include "Sas/Core/Core.h"
#include "Sas/Core/KeyCodes.h"
#include "Sas/Core/MouseCodes.h"
namespace Sas {
	class SAS_API Input {
	public:
		inline static bool IsKeyPressed(KeyCode kode) { return s_Instance->IsKeyPressedIMPL(kode); };
		
		inline static bool IsMouseButtonPressed(MouseCode button) { return s_Instance->IsMouseButtonPressed(button);};
		inline static float GetMouseX(){return s_Instance->GetMouseXIMPL();};
		inline static float GetMouseY(){return s_Instance->GetMouseYIMPL();};
		inline static std::pair<float, float> GetMousePosition() { return s_Instance->GetMousePositionIMPL(); };
	protected:
		virtual bool IsKeyPressedIMPL(KeyCode keykode) = 0; 
		virtual bool IsMouseButtonPressedIMPL(MouseCode button) = 0;
		virtual float GetMouseXIMPL() = 0;
		virtual float GetMouseYIMPL() = 0;
		virtual std::pair<float, float> GetMousePositionIMPL() = 0;

	private:
		static Input* s_Instance;
	};
}