#pragma once
#include "Sas/Core/Window.h"
#include "Sas/Renderer/GraphicsContext.h"
//#include "Platform\Vulkan\VulkanSwapChain.h"
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

		inline uint16_t GetPosX() const override { return m_Data.PosX; };
		inline uint16_t GetPosY() const override { return m_Data.PosY; };


		inline void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }
		void SetVSync(bool enable) override;
		bool IsVSync() const override;
		inline virtual void* GetNativeWindow() override { return m_Window; };
		virtual Ref<GraphicsContext> GetContext() override { return m_Context; }
	private:
		virtual void Init(const WindowProps& props);
		virtual void ShutDown();

		GLFWwindow* m_Window;

		Ref<GraphicsContext> m_Context;

		struct WindowData
		{
			std::string Title;
			uint16_t Height, Width;
			uint16_t PosX, PosY;
			EventCallbackFn EventCallback;
			bool vSync;
		};

		WindowData m_Data;
		//VulkanSwapChain m_SwapChain;
	};

}

