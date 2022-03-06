#pragma once
#include "Sas/Core/Input.h"

namespace Sas {
	class WindowsInput : public Input {
	protected:
		virtual bool IsKeyPressedIMPL(KeyCode keycode) override;
		virtual bool IsMouseButtonPressedIMPL(MouseCode button) override;
		virtual float GetMouseXIMPL() override;
		virtual float GetMouseYIMPL() override;
		virtual std::pair<float, float> GetMousePositionIMPL() override;
	};
}