#include "ssph.h"
#include "Sas/Core/Application.h"
#include "Sas/Core/Input.h" 
#include "GLFW/glfw3.h"

namespace Sas {

	bool Input::IsKeyPressed(KeyCode keycode)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		int state = glfwGetKey(window, keycode);

		return state == GLFW_PRESS || state == GLFW_REPEAT;
	} 

	bool Input::IsMouseButtonPressed(MouseCode button)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		int state = glfwGetMouseButton(window, button);

		return state == GLFW_PRESS;

	}

	float Input::GetMouseX()
	{
		return GetMousePosition().first;
	}

	float Input::GetMouseY()
	{
		return GetMousePosition().second;
	}

	std::pair<float, float> Input::GetMousePosition()
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		double x, y;
		glfwGetCursorPos(window, &x, &y);
		return {(float)x, (float)y}; 
	}

}