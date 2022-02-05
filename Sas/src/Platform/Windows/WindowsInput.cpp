#include "ssph.h"
#include "WindowsInput.h"
#include "Sas/Aplication.h"
#include "GLFW/glfw3.h"

namespace Sas {

	Input* Input::s_Instance = new WindowsInput();

	bool WindowsInput::IsKeyPressedIMPL(KeyCode keycode)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNatieWindow());
		int state = glfwGetKey(window, keycode);

		return state == GLFW_PRESS || state == GLFW_REPEAT;
	} 

	bool WindowsInput::IsMouseButtonPressedIMPL(MouseCode button)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNatieWindow());
		int state = glfwGetMouseButton(window, button);

		return state == GLFW_PRESS;

	}

	float WindowsInput::GetMouseXIMPL()
	{
		return GetMousePositionIMPL().first;
	}

	float WindowsInput::GetMouseYIMPL()
	{
		return GetMousePositionIMPL().second;
	}

	std::pair<float, float> WindowsInput::GetMousePositionIMPL()
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNatieWindow());
		double x, y;
		glfwGetCursorPos(window, &x, &y);
		return {(float)x, (float)y}; 
	}

}