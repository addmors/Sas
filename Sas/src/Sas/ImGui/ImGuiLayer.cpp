#include "ssph.h"
#include "ImGuiLayer.h"

#include "imgui.h"
#include "Platform\OpenGL\ImGuiOpenGLRenderer.h"
#include "GLFW\glfw3.h"
#include "Sas\Aplication.h"

#include <glad/glad.h>

namespace Sas {

 	static ImGuiKey ImGui_ImplSas_KeyToImGuiKey(KeyCode kode) {
		switch (kode) {
			case Key::Tab: return ImGuiKey_Tab;
			case Key::Left: return ImGuiKey_LeftArrow;
			case Key::Right: return ImGuiKey_RightArrow;
			case Key::Up: return ImGuiKey_UpArrow;
			case Key::Down: return ImGuiKey_DownArrow;
			case Key::PageUp: return ImGuiKey_PageUp;
			case Key::PageDown: return ImGuiKey_PageDown;
			case Key::Home: return ImGuiKey_Home;
			case Key::End: return ImGuiKey_End;
			case Key::Insert: return ImGuiKey_Insert;
			case Key::Delete: return ImGuiKey_Delete;
			case Key::Backspace: return ImGuiKey_Backspace;
			case Key::Space: return ImGuiKey_Space;
			case Key::Enter: return ImGuiKey_Enter;
			case Key::Escape: return ImGuiKey_Escape;
			case Key::Apostrophe: return ImGuiKey_Apostrophe;
			case Key::Comma: return ImGuiKey_Comma;
			case Key::Minus: return ImGuiKey_Minus;
			case Key::Period: return ImGuiKey_Period;
			case Key::Slash: return ImGuiKey_Slash;
			case Key::Semicolon: return ImGuiKey_Semicolon;
			case Key::Equal: return ImGuiKey_Equal;
			case Key::LeftBracket: return ImGuiKey_LeftBracket;
			case Key::Backslash: return ImGuiKey_Backslash;
			case Key::RightBracket: return ImGuiKey_RightBracket;
			case Key::GraveAccent: return ImGuiKey_GraveAccent;
			case Key::CapsLock: return ImGuiKey_CapsLock;
			case Key::ScrollLock: return ImGuiKey_ScrollLock;
			case Key::NumLock: return ImGuiKey_NumLock;
			case Key::PrintScreen: return ImGuiKey_PrintScreen;
			case Key::Pause: return ImGuiKey_Pause;
			case Key::KP0: return ImGuiKey_Keypad0;
			case Key::KP1: return ImGuiKey_Keypad1;
			case Key::KP2: return ImGuiKey_Keypad2;
			case Key::KP3: return ImGuiKey_Keypad3;
			case Key::KP4: return ImGuiKey_Keypad4;
			case Key::KP5: return ImGuiKey_Keypad5;
			case Key::KP6: return ImGuiKey_Keypad6;
			case Key::KP7: return ImGuiKey_Keypad7;
			case Key::KP8: return ImGuiKey_Keypad8;
			case Key::KP9: return ImGuiKey_Keypad9;
			case Key::KPDecimal: return ImGuiKey_KeypadDecimal;
			case Key::KPDivide: return ImGuiKey_KeypadDivide;
			case Key::KPMultiply: return ImGuiKey_KeypadMultiply;
			case Key::KPSubtract: return ImGuiKey_KeypadSubtract;
			case Key::KPAdd: return ImGuiKey_KeypadAdd;
			case Key::KPEnter: return ImGuiKey_KeypadEnter;
			case Key::KPEqual: return ImGuiKey_KeypadEqual;
			case Key::LeftShift: return ImGuiKey_LeftShift;
			case Key::LeftControl: return ImGuiKey_LeftCtrl;
			case Key::LeftAlt: return ImGuiKey_LeftAlt;
			case Key::LeftSuper: return ImGuiKey_LeftSuper;
			case Key::RightShift: return ImGuiKey_RightShift;
			case Key::RightControl: return ImGuiKey_RightCtrl;
			case Key::RightAlt: return ImGuiKey_RightAlt;
			case Key::RightSuper: return ImGuiKey_RightSuper;
			case Key::Menu: return ImGuiKey_Menu;
			case Key::D0: return ImGuiKey_0;
			case Key::D1: return ImGuiKey_1;
			case Key::D2: return ImGuiKey_2;
			case Key::D3: return ImGuiKey_3;
			case Key::D4: return ImGuiKey_4;
			case Key::D5: return ImGuiKey_5;
			case Key::D6: return ImGuiKey_6;
			case Key::D7: return ImGuiKey_7;
			case Key::D8: return ImGuiKey_8;
			case Key::D9: return ImGuiKey_9;
			case Key::A: return ImGuiKey_A;
			case Key::B: return ImGuiKey_B;
			case Key::C: return ImGuiKey_C;
			case Key::D: return ImGuiKey_D;
			case Key::E: return ImGuiKey_E;
			case Key::F: return ImGuiKey_F;
			case Key::G: return ImGuiKey_G;
			case Key::H: return ImGuiKey_H;
			case Key::I: return ImGuiKey_I;
			case Key::J: return ImGuiKey_J;
			case Key::K: return ImGuiKey_K;
			case Key::L: return ImGuiKey_L;
			case Key::M: return ImGuiKey_M;
			case Key::N: return ImGuiKey_N;
			case Key::O: return ImGuiKey_O;
			case Key::P: return ImGuiKey_P;
			case Key::Q: return ImGuiKey_Q;
			case Key::R: return ImGuiKey_R;
			case Key::S: return ImGuiKey_S;
			case Key::T: return ImGuiKey_T;
			case Key::U: return ImGuiKey_U;
			case Key::V: return ImGuiKey_V;
			case Key::W: return ImGuiKey_W;
			case Key::X: return ImGuiKey_X;
			case Key::Y: return ImGuiKey_Y;
			case Key::Z: return ImGuiKey_Z;
			case Key::F1: return ImGuiKey_F1;
			case Key::F2: return ImGuiKey_F2;
			case Key::F3: return ImGuiKey_F3;
			case Key::F4: return ImGuiKey_F4;
			case Key::F5: return ImGuiKey_F5;
			case Key::F6: return ImGuiKey_F6;
			case Key::F7: return ImGuiKey_F7;
			case Key::F8: return ImGuiKey_F8;
			case Key::F9: return ImGuiKey_F9;
			case Key::F10: return ImGuiKey_F10;
			case Key::F11: return ImGuiKey_F11;
			case Key::F12: return ImGuiKey_F12;
		default: return ImGuiKey_None;
		}
	}


	ImGuiLayer::ImGuiLayer() : Layer("ImGui Layer"), m_Time(0.0f)
	{

	}
	ImGuiLayer::~ImGuiLayer() {

	};

	void ImGuiLayer::OnAttach()
	{
		ImGui::CreateContext();
		ImGui::StyleColorsDark();

		ImGuiIO& io = ImGui::GetIO();
		io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
		io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;
		

		ImGui_ImplOpenGL3_Init("#version 450");

	};
	void ImGuiLayer::OnDetach()
	{
	}
	void ImGuiLayer::OnUpdate()
	{
		Application& app = Application::Get();
		ImGuiIO& io = ImGui::GetIO();

		float time = (float)glfwGetTime();
		io.DeltaTime = m_Time > 0.0f ? (time - m_Time) : (1.00 / 60.0f);
		io.DisplaySize = ImVec2(app.GetWindow().GetWidth(), app.GetWindow().GetHeight());

		ImGui_ImplOpenGL3_NewFrame();
		ImGui::NewFrame();

		static bool show = true;
		ImGui::ShowDemoWindow(&show);

		ImGui::Render();

		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}



	void ImGuiLayer::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);

		dispatcher.Dispatch<MouseButtonPressedEvent>(SS_BIND_EVENT_FN(ImGuiLayer::OnButtonPresedEvent));
		dispatcher.Dispatch<MouseButtonReleasedEvent>(SS_BIND_EVENT_FN(ImGuiLayer::OnButtonReleasedEvent));
		dispatcher.Dispatch<MouseMovedEvent>(SS_BIND_EVENT_FN(ImGuiLayer::OnMouseMovedEvent));
		dispatcher.Dispatch<MouseScrolledEvent>(SS_BIND_EVENT_FN(ImGuiLayer::OnMouseScrolledEvent));
		dispatcher.Dispatch<KeyPressedEvent>(SS_BIND_EVENT_FN(ImGuiLayer::OnKeyPressedEvent));
		dispatcher.Dispatch<KeyTypedEvent>(SS_BIND_EVENT_FN(ImGuiLayer::OnKeyTypedEvent));
		dispatcher.Dispatch<KeyReleasedEvent>(SS_BIND_EVENT_FN(ImGuiLayer::OnKeyReleasedEvent));
		dispatcher.Dispatch<WindowResizeEvent>(SS_BIND_EVENT_FN(ImGuiLayer::OnWindowResizeEvent));

	}

	bool ImGuiLayer::OnButtonPresedEvent(MouseButtonPressedEvent& event)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.AddMouseButtonEvent(event.GetMouseButton(), true);
		return false;
	}

	bool ImGuiLayer::OnButtonReleasedEvent(MouseButtonReleasedEvent& event)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.AddMouseButtonEvent(event.GetMouseButton(), false);
		return false;
	}

	bool ImGuiLayer::OnMouseMovedEvent(MouseMovedEvent& event)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.AddMousePosEvent(event.GetX(), event.GetY());
		return false;
	}

	bool ImGuiLayer::OnMouseScrolledEvent(MouseScrolledEvent& event)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.AddMouseWheelEvent(event.GetXOffset(), event.GetYOffset());

		return false;
	}

	bool ImGuiLayer::OnKeyPressedEvent(KeyPressedEvent& event)
	{
		ImGuiIO& io = ImGui::GetIO();
		int mods = event.GetMode();
		io.AddKeyEvent(ImGuiKey_ModCtrl, (mods & GLFW_MOD_CONTROL) != 0);
		io.AddKeyEvent(ImGuiKey_ModShift, (mods & GLFW_MOD_SHIFT) != 0);
		io.AddKeyEvent(ImGuiKey_ModAlt, (mods & GLFW_MOD_ALT) != 0);
		io.AddKeyEvent(ImGuiKey_ModSuper, (mods & GLFW_MOD_SUPER) != 0);

		ImGuiKey imgui_key = ImGui_ImplSas_KeyToImGuiKey(event.GetKeyKode());
		io.AddKeyEvent(imgui_key, true);
		io.SetKeyEventNativeData(imgui_key, event.GetKeyKode(), event.GetKeyKode());

		return false;
	}

	bool ImGuiLayer::OnKeyReleasedEvent(KeyReleasedEvent& event)
	{
		ImGuiIO& io = ImGui::GetIO();
		
		ImGuiKey imgui_key = ImGui_ImplSas_KeyToImGuiKey(event.GetKeyKode());
		io.AddKeyEvent(imgui_key, false);
		io.SetKeyEventNativeData(imgui_key, event.GetKeyKode(), event.GetKeyKode());

		return false;
	}

	bool ImGuiLayer::OnKeyTypedEvent(KeyTypedEvent& event)
	{

		ImGuiIO& io = ImGui::GetIO();
		io.AddInputCharacter(event.GetKeyKode());

		return false;
	}



	bool ImGuiLayer::OnWindowResizeEvent(WindowResizeEvent& event)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize = ImVec2(event.GetWidth(), event.GetHeight());
		io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);
		glViewport(0, 0, event.GetWidth(), event.GetHeight());

		return false;
	}

}