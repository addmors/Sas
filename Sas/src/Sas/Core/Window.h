#pragma once
#include "ssph.h"
#include "Sas/Events/Event.h"
#include "Sas/Core/Log.h"


namespace Sas {

	struct  WindowProps
	{
		std::string Title;
		uint16_t  Height;
		uint16_t  Width;

		WindowProps(const std::string& title = "Sas Engine",
			uint16_t height = 720,
			uint16_t width = 1280)
			:Title(title),Height(height),Width(width)
		{};
	};

	class SAS_API Window {
	public:
		using EventCallbackFn = std::function<void(Event&)>;
		virtual ~Window() {}
		virtual void OnUpdate() = 0;

		virtual uint16_t GetHeight() const = 0;
		virtual uint16_t GetWidth() const = 0;

		virtual  void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enable) = 0;
		virtual bool IsVSync() const = 0;
		virtual void* GetNativeWindow() = 0;

		static Scope<Window> Create(const WindowProps& props = WindowProps());
	};
}