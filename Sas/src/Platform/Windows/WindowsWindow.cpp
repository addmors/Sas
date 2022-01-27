#include "ssph.h"
#include "WindowsWindow.h"

namespace Sas {
	static bool is_GLFWInitialized = false;

	Window* Window::Create(const WindowProps& props) {
		return new WindowsWindow(props);
	};

	WindowsWindow::WindowsWindow(const WindowProps& props) {
		Init(props);
	}

	WindowsWindow::~WindowsWindow() {
		ShutDown();
	};

	void WindowsWindow::Init(const WindowProps& props) {
		m_Data.Height = props.Height;
		m_Data.Width = props.Width;
		m_Data.Title = props.Title;

		SS_CORE_INFO("Create Window {0}, ({1}, {2})", props.Title, props.Width, props.Height);
		
		if (!is_GLFWInitialized) {
			int success = glfwInit();
			SS_CORE_ASSERT(success, "Could not  Init GLFW");
			is_GLFWInitialized = true;
		}
		m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, props.Title.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(m_Window);

		glfwSetWindowUserPointer(m_Window, &m_Data);
		SetVSync(true);
	};

	void WindowsWindow::ShutDown() {
		glfwDestroyWindow(m_Window);
	};

	void WindowsWindow::OnUpdate() {
		glfwPollEvents();
		glfwSwapBuffers(m_Window);
	}

	void WindowsWindow::SetVSync(bool enable) {
		if (enable) glfwSwapInterval(1); else glfwSwapInterval(0);
		m_Data.vSync = enable;
	};
	bool WindowsWindow::IsVSync() const { return m_Data.vSync; };	
}