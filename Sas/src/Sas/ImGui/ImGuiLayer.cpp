#include "ssph.h"
#include "Sas/ImGui/ImGuiLayer.h"

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include "Sas/Core/Application.h"

// TEMPORARY
#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include "ImGuizmo.h"

namespace Sas {

	ImGuiLayer::ImGuiLayer()
		: Layer("ImGuiLayer")
	{
	}

	void ImGuiLayer::OnAttach()
	{
		SS_PROFILE_FUNCTION();

		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoTaskBarIcons;
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoMerge;

		float fontSize = 18.0f;// *2.0f;
		io.Fonts->AddFontFromFileTTF("assets/fonts/opensans/OpenSans-Bold.ttf", fontSize);
		io.FontDefault = io.Fonts->AddFontFromFileTTF("assets/fonts/opensans/OpenSans-Regular.ttf", fontSize);

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();
		//ImGui::StyleColorsClassic();

		// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		SetDarkThemeColors();

		Application& app = Application::Get();
		GLFWwindow* window = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());

		// Setup Platform/Renderer bindings
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 450");
	}

	void ImGuiLayer::OnDetach()
	{
		SS_PROFILE_FUNCTION();

		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void ImGuiLayer::OnEvent(Event& e)
	{
		if (m_BlockEvents)
		{
			ImGuiIO& io = ImGui::GetIO();
			e.Handled |= e.IsInCategory(EventCategoryMouse) & io.WantCaptureMouse;
			e.Handled |= e.IsInCategory(EventCategoryKeyboard) & io.WantCaptureKeyboard;
		}
	}

	void ImGuiLayer::Begin()
	{
		SS_PROFILE_FUNCTION();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGuizmo::BeginFrame();
	}

	void ImGuiLayer::End()
	{
		SS_PROFILE_FUNCTION();

		ImGuiIO& io = ImGui::GetIO();
		Application& app = Application::Get();
		io.DisplaySize = ImVec2((float)app.GetWindow().GetWidth(), (float)app.GetWindow().GetHeight());

		// Rendering
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}
	}

	void ImGuiLayer::SetDarkThemeColors()
	{
		auto& style = ImGui::GetStyle();
		auto& colors = style.Colors;


		style.WindowRounding = 4.0;
		style.WindowTitleAlign = ImVec2(0.5, 0.5);
		style.ChildRounding = 2.0;
		style.FrameRounding = 4.0;
		style.ItemSpacing = ImVec2(10, 5);
		style.ScrollbarSize = 15;
		style.ScrollbarRounding = 0;
		style.GrabMinSize = 9.6;
		style.GrabRounding = 1.0;
		style.WindowPadding = ImVec2(10, 10);
		style.AntiAliasedLines = true;
		style.AntiAliasedFill = true;
		style.FramePadding = ImVec2(5, 4);
		style.DisplayWindowPadding = ImVec2(27, 27);
		style.DisplaySafeAreaPadding = ImVec2(5, 5);
		style.ButtonTextAlign = ImVec2(0.5, 0.5);
		style.IndentSpacing = 12.0;
		style.Alpha = 1.0;

		//colors[ImGuiCol_WindowBg] = ImVec4{ 0.1f, 0.105f, 0.11f, 1.0f };

		//// Headers
		//colors[ImGuiCol_Header] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
		//colors[ImGuiCol_HeaderHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
		//colors[ImGuiCol_HeaderActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

		//// Buttons
		//colors[ImGuiCol_Button] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
		//colors[ImGuiCol_ButtonHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
		//colors[ImGuiCol_ButtonActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

		//// Frame BG
		//colors[ImGuiCol_FrameBg] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
		//colors[ImGuiCol_FrameBgHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
		//colors[ImGuiCol_FrameBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

		//// Tabs
		colors[ImGuiCol_Tab] = ImVec4{ 0.13, 0.75, 0.55, 0.40};
		colors[ImGuiCol_TabHovered] = ImVec4{ 0.4f, 0.9f, 0.65f, 1.0f };
		colors[ImGuiCol_TabActive] = ImVec4{ 0.3, 0.8, 0.6, 1.0f };
		colors[ImGuiCol_TabUnfocused] = ImVec4{0.13, 0.75, 0.55, 0.40};
		colors[ImGuiCol_TabUnfocusedActive] = ImVec4{ 0.23, 0.85, 0.65, 0.60 };

		//// Title
		//colors[ImGuiCol_TitleBg] = ImVec4{ 0.13, 0.75, 0.55, 0.40 };
		//colors[ImGuiCol_TitleBgActive] = ImVec4{ 0.13, 0.75, 0.55, 0.40 };
		//colors[ImGuiCol_TitleBgCollapsed] = ImVec4{ 0.13, 0.75, 0.55, 0.40 };


		colors[ImGuiCol_Text] = ImVec4(1.00, 1.00, 1.00, 1.00);
		colors[ImGuiCol_TextDisabled] = ImVec4(0.50, 0.50, 0.50, 1.00);
		colors[ImGuiCol_WindowBg] = ImVec4(0.06, 0.06, 0.06, 0.94);
		colors[ImGuiCol_ChildBg] = ImVec4(0.00, 0.00, 0.00, 0.00);
		colors[ImGuiCol_PopupBg] = ImVec4(0.08, 0.08, 0.08, 0.94);
		colors[ImGuiCol_Border] = ImVec4(0.43, 0.43, 0.50, 0.50);
		colors[ImGuiCol_BorderShadow] = ImVec4(0.00, 0.00, 0.00, 0.00);
		colors[ImGuiCol_FrameBg] = ImVec4{ 0.04, 0.55, 0.45, 0.40 };
		colors[ImGuiCol_FrameBgHovered] = ImVec4(0.12, 0.62, 0.5, 0.70);
		colors[ImGuiCol_FrameBgActive] = ImVec4(0.15, 0.63, 0.6, 0.80);
		colors[ImGuiCol_TitleBg] = ImVec4(0.04, 0.04, 0.04, 1.00);
		colors[ImGuiCol_TitleBgActive] = ImVec4(0.16, 0.16, 0.16, 1.00);
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00, 0.00, 0.00, 0.60);
		colors[ImGuiCol_MenuBarBg] = ImVec4(0.14, 0.14, 0.14, 1.00);
		colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02, 0.02, 0.02, 0.53);
		colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31, 0.31, 0.31, 1.00);
		colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41, 0.41, 0.41, 1.00);
		colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51, 0.51, 0.51, 1.00);
		colors[ImGuiCol_CheckMark] = ImVec4(0.13, 0.75, 0.55, 0.80);
		colors[ImGuiCol_SliderGrab] = ImVec4(0.13, 0.75, 0.75, 0.80);
		colors[ImGuiCol_SliderGrabActive] = ImVec4(0.13, 0.75, 1.00, 0.80);
		colors[ImGuiCol_Button] = ImVec4(0.13, 0.75, 0.55, 0.40);
		colors[ImGuiCol_ButtonHovered] = ImVec4(0.13, 0.75, 0.75, 0.60);
		colors[ImGuiCol_ButtonActive] = ImVec4(0.13, 0.75, 1.00, 0.80);
		colors[ImGuiCol_Header] = ImVec4(0.13, 0.75, 0.55, 0.40);
		colors[ImGuiCol_HeaderHovered] = ImVec4(0.13, 0.75, 0.75, 0.60);
		colors[ImGuiCol_HeaderActive] = ImVec4(0.13, 0.75, 1.00, 0.80);
		colors[ImGuiCol_Separator] = ImVec4(0.13, 0.75, 0.55, 0.40);
		colors[ImGuiCol_SeparatorHovered] = ImVec4(0.13, 0.75, 0.75, 0.60);
		colors[ImGuiCol_SeparatorActive] = ImVec4(0.13, 0.75, 1.00, 0.80);
		colors[ImGuiCol_ResizeGrip] = ImVec4(0.13, 0.75, 0.55, 0.40);
		colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.13, 0.75, 0.75, 0.60);
		colors[ImGuiCol_ResizeGripActive] = ImVec4(0.13, 0.75, 1.00, 0.80);
		colors[ImGuiCol_PlotLines] = ImVec4(0.61, 0.61, 0.61, 1.00);
		colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00, 0.43, 0.35, 1.00);
		colors[ImGuiCol_PlotHistogram] = ImVec4(0.90, 0.70, 0.00, 1.00);
		colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00, 0.60, 0.00, 1.00);
		colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26, 0.59, 0.98, 0.35);
		colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80, 0.80, 0.80, 0.35);
	}

}
