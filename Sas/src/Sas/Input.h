#pragma once
#include "Sas/Core.h"
#include "Sas/Core/KeyCodes.h"
namespace Sas {
	class SAS_API Input {
	public:
		static bool IsKeyPressed(KeyCode kode){return s_Instance->IsKeyPressedIMPL(kode);
	};
	protected:
		virtual bool IsKeyPressedIMPL(KeyCode keykode) = 0; 
	private:
		static Input* s_Instance;
	};
}