#pragma once
#include "Sas\Window.h"
#include "Renderer/GraphicsContext.h"

#include <GLFW/glfw3.h>

namespace Sas {
	class SAS_API WindowsWindow : public  Window
	{
	public:

		WindowsWindow(const  WindowProps& props);
		virtual ~WindowsWindow();

		void OnUpdate() override;


		inline uint16_t GetHeight() const override { return m_Data.Height; };

		inline uint16_t GetWidth() const override { return m_Data.Width; };
		inline void SetEventCallBack(const EventCallBackFn& func) override { m_Data.EventCallBack = func; };
		void SetVSync(bool enable) override;
		bool IsVSync() const override;
		inline virtual void* GetNativeWindow() override { return m_Window; };
	private:
		virtual void Init(const WindowProps& props);
		virtual void ShutDown();

		GLFWwindow* m_Window;

		GraphicsContext* m_Context;

		struct WindowData
		{
			std::string Title;
			uint16_t Height, Width;
			EventCallBackFn EventCallBack;
			bool vSync;
		};

		WindowData m_Data;
	};

}

