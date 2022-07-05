#include "ssph.h"
#include "WindowsWindow.h"


#include "Sas\Events\ApplicationEvent.h"
#include "Sas\Events\MouseEvent.h"
#include "Sas\Events\KeyEvent.h"
#include "Sas\Renderer\GraphicsContext.h"
//#include "Platform/Vulkan/VulkanContext.h"

#include "Sas\Renderer\RendererAPI.h"


namespace Sas {
	static bool is_GLFWInitialized = false;
	static void GLFWErrorCallback(int error, const char* description) {
		SS_CORE_ERROR("GLFW Error {0}, {1}",error, description);
	}

	Scope<Window> Window::Create(const WindowProps& props)
	{
		return CreateScope<WindowsWindow>(props);
	}


	WindowsWindow::WindowsWindow(const WindowProps& props) {
		SS_PROFILE_FUNCTION();
		Init(props);
	}

	WindowsWindow::~WindowsWindow() {
		SS_PROFILE_FUNCTION();
		ShutDown();
	};

	void WindowsWindow::Init(const WindowProps& props) {
		SS_PROFILE_FUNCTION();
		m_Data.Height = props.Height;
		m_Data.Width = props.Width;
		m_Data.PosX = props.PosX;
		m_Data.PosY = props.PosY;
		m_Data.Title = props.Title;

		SS_CORE_INFO("Create Window {0}, Size :({1}, {2},), Pos:({3}, {4}) ", props.Title, 
			props.Width, props.Height,
			props.PosX, props.PosY);
		
		if (!is_GLFWInitialized) {
			SS_PROFILE_SCOPE("glfwCreateInit");
			int success = glfwInit();
			SS_CORE_ASSERT(success, "Could not  Init GLFW");
			glfwSetErrorCallback(GLFWErrorCallback);
			is_GLFWInitialized = true;
		}
		{
			SS_PROFILE_SCOPE("glfwCreateWindow");
			if (RendererAPI::GetAPI() == RendererAPI::API::Vulcak)
				glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
			m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, props.Title.c_str(), nullptr, nullptr);
		}
		glfwSetWindowPos(m_Window, (int)props.PosX, (int)props.PosY);
		m_Context = GraphicsContext::Create(m_Window);
		m_Context->Init();

		glfwSetWindowUserPointer(m_Window, &m_Data);
		SetVSync(true);

		//Set GLFW CALLBACK
		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			data.Height = height;
			data.Width = width;
			WindowResizeEvent event(width, height);
			data.EventCallback(event);
			
			});
		glfwSetWindowPosCallback(m_Window, [](GLFWwindow* window, int posx, int posy) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			data.PosX = posx;
			data.PosY = posy;
			WindowMovedEvent event(posx, posy);
			data.EventCallback(event);
			});

		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			WindowCloseEvent event;
			data.EventCallback(event);

			});
		glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int character) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			KeyTypedEvent event(character, 0);
			data.EventCallback(event);
		});
		glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mode) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				switch (action)
				{
					case GLFW_PRESS:
					{
						KeyPressedEvent event(key, 0, mode);
						data.EventCallback(event);
						break;
					}
					case GLFW_RELEASE:
					{
						KeyReleasedEvent event(key, mode);
						data.EventCallback(event);
						break;
					}
					case GLFW_REPEAT:
					{
						KeyPressedEvent event(key, 1, mode);
						data.EventCallback(event);
						break; 
					}
				};
			});
		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mode) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				switch (action)
				{
					case GLFW_PRESS:
					{
						MouseButtonPressedEvent event(button);
						data.EventCallback(event);
						break;
					}
					case GLFW_RELEASE:
					{
						MouseButtonReleasedEvent event(button);
						data.EventCallback(event);
						break;
					}
				};
			});
		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			MouseScrolledEvent event((float)xOffset, (float)yOffset);
			data.EventCallback(event); 
			});
		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			MouseMovedEvent event((float)xPos, (float)yPos);
			data.EventCallback(event);
			});
	};

	void WindowsWindow::ShutDown() {
		SS_PROFILE_FUNCTION();

		glfwDestroyWindow(m_Window);
	};

	void WindowsWindow::OnUpdate() {
		SS_PROFILE_FUNCTION();

		glfwPollEvents();
		m_Context->SwapBuffers();
	}

	void WindowsWindow::SetVSync(bool enable) {
		if (enable) glfwSwapInterval(1); else glfwSwapInterval(0);
		m_Data.vSync = enable;
	};
	bool WindowsWindow::IsVSync() const { return m_Data.vSync; };	


}