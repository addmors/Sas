#pragma once
#include "ssph.h"
#include "Sas/Events/Event.h"
#include "Sas/Core/Log.h"
#include "Sas/Renderer/GraphicsContext.h"

namespace Sas {

	struct  WindowProps
	{
		std::string Title;
		uint16_t  Height;
		uint16_t  Width;
		uint16_t PosX;
		uint16_t PosY;

		WindowProps(const std::string& title = "Sas Engine",
			uint16_t height = 900,
			uint16_t width = 1600, uint16_t posx = 100, uint16_t posy = 100)
			:Title(title),Height(height),Width(width),PosX(posx),PosY(posy)
		{};
	};

	class SAS_API Window {

		enum class WINDOW_API
		{
			None = 0, GLFW = 1, WIN_32 = 2, X_11 = 3
		};

	public:
		using EventCallbackFn = std::function<void(Event&)>;
		virtual ~Window() {}
		virtual void OnUpdate() = 0;

		virtual uint16_t GetHeight() const = 0;
		virtual uint16_t GetWidth() const = 0;

		virtual uint16_t GetPosX() const = 0;
		virtual uint16_t GetPosY() const = 0;

		virtual  void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enable) = 0;
		virtual bool IsVSync() const = 0;
		virtual void* GetNativeWindow() = 0;

		virtual Ref<GraphicsContext> GetContext() = 0;
		static Scope<Window> Create(const WindowProps& props = WindowProps());
	};
}