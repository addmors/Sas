#include "EditorLayer.h"
#include <chrono>
#include "imgui/imgui.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"


namespace Sas {
	EditorLayer::EditorLayer()
		:Layer("Sendbox2D"), m_CameraController(1280.0f / 720.0f)
	{

	};

	void EditorLayer::OnAttach()
	{

		SS_PROFILE_FUNCTION();
		m_CheckboardTexture = Sas::Texture2D::Create("assets/textures/Checkerboard.png");
#ifdef ZERO
		m_Particle.ColorBegin = { 254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f };
		m_Particle.ColorEnd = { 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 1.0f };
		m_Particle.SizeBegin = 0.2f, m_Particle.SizeVariation = 0.05f, m_Particle.SizeEnd = 0.0f;
		m_Particle.LifeTime = 1.0f;
		m_Particle.Velocity = { 0.0f, 0.0f };
		m_Particle.VelocityVariation = { 3.0f, 1.0f };
		m_Particle.Position = { 0.0f, 0.0f };
#endif // ZERO
		FramebufferTextureSpecification texspec{FramebufferTextureFormat::RGBA8};
		FramebufferAttachmentSpecification attach { texspec };
		FramebufferSpecification fbspec;
		fbspec.Width = 1280;
		fbspec.Height = 720;
		fbspec.Attachments = attach;

		m_Framebuffer = Sas::Framebuffer::Create(fbspec);
	}

	void EditorLayer::OnDetach()
	{

		SS_PROFILE_FUNCTION();
	}

	void EditorLayer::OnUpdate(Sas::Timestep ts)
	{
		time += ts;
		SS_PROFILE_FUNCTION();
		if(m_ViewPortFocused)
			m_CameraController.OnUpdate(ts);
		Sas::Renderer2D::ResetStats();
		{
			SS_PROFILE_SCOPE("EditorLayer clean");
			m_Framebuffer->Bind();
			Sas::RenderCommand::SetClearColor({ 0.2f, 0.2f, 0.2f, 1.0f });
			Sas::RenderCommand::Clear();
		}

		{
			Sas::Renderer2D::BeginScene(m_CameraController.GetCamera());
			SS_PROFILE_SCOPE("EditorLayer draw");
			Sas::Renderer2D::DrawQuad({ -1.0f,0.0f }, { 1.0f,1.0f }, { 0.8f,0.2f,0.3f,1.0f });
			Sas::Renderer2D::DrawQuad({ -0.5f,-0.5f }, { 0.2f,0.8f }, { 0.3f,0.2f,0.8f,1.0f });

			Sas::Renderer2D::DrawQuad({ -0.0f,-0.0f, -0.1f }, { 10.0f, 10.0f }, m_CheckboardTexture, 10.0f);
			Sas::Renderer2D::DrawRotatedQuad({ 2.0f,0.0f }, { 0.5f,0.5f }, -45.0f, m_CheckboardTexture, 10.0f);

			Sas::Renderer2D::EndScene();

			Sas::Renderer2D::BeginScene(m_CameraController.GetCamera());
			for (float y = -3.0f; y < 3.0f; y += 0.5f) {
				for (float x = -3.0f; x < 3.0f; x += 0.5f) {
					glm::vec4 rgba = { (x + 5.0f) / 10.0f, 0.4f,(y + 5.0f) / 10.0f , 0.5f };
					Sas::Renderer2D::DrawRotatedQuad({ x,y }, { 0.45f, 0.45f }, 0, rgba);
				}
			}
			Sas::Renderer2D::EndScene();
#ifdef ZERO 

			if (Sas::Input::IsMouseButtonPressed(Sas::Mouse::ButtonRight)) {
				auto [x, y] = Sas::Input::GetMousePosition();
				auto width = Sas::Application::Get().GetWindow().GetWidth();
				auto height = Sas::Application::Get().GetWindow().GetHeight();
				auto bound = m_CameraController.GetBounds();
				auto pos = m_CameraController.GetCamera().GetPosition();
				x = (x / width) * bound.GetWidth() - bound.GetWidth() * 0.5;
				y = bound.GetHeight() * 0.5 - (y / height) * bound.GetHeight();
				m_Particle.Position = { x + pos.x,y + pos.y };
				x_ = x;
				y_ = y;
				for (int i = 0; i < 1; i++)
					m_ParticleSystem.Emit(m_Particle);

			}
			m_ParticleSystem.OnUpdate(ts);
			Sas::Renderer2D::BeginScene(m_CameraController.GetCamera());
			m_ParticleSystem.OnRender();
			Sas::Renderer2D::EndScene();
#endif // 0
			m_Framebuffer->Unbind();
		}

	}

	void EditorLayer::OnImGuiRender()
	{


		static bool dockspaceOpen = true;
		static bool opt_fullscreen = true;
		static bool opt_padding = false;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)
		{
			const ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->WorkPos);
			ImGui::SetNextWindowSize(viewport->WorkSize);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}
		else
		{
			dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
		}

		// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
		// and handle the pass-thru hole, so we ask Begin() to not render a background.
		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
		// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
		// all active windows docked into it will lose their parent and become undocked.
		// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
		// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
		if (!opt_padding)
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
		if (!opt_padding)
			ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		// Submit the DockSpace
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("Options"))
			{
				// Disabling fullscreen would allow the window to be moved to the front of other windows,
				// which we can't undo at the moment without finer window depth/z control.
				if (ImGui::MenuItem("Exit")) Sas::Application::Get().Close();

				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}
		auto stat = Sas::Renderer2D::GetStats();
		ImGui::Begin("Setting");
		ImGui::Text("Render2D stat");
		ImGui::Text("Draw Call: %d", stat.DrawCalls);
		ImGui::Text("Quad: %d", stat.QuadCount);
		ImGui::Text("Vertices: %df", stat.GetTotalVertexCount());
		ImGui::Text("Indecies: %df", stat.GetTotalIndexCount());
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		uint32_t texID = m_Framebuffer->GetColorAttachmentRendererID();
		
		ImGui::End();
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{0,0});
		ImGui::Begin("Viewport");
		m_ViewPortFocused = ImGui::IsWindowFocused();
		m_ViewPortHovered = ImGui::IsWindowHovered();

		Application::Get().GetImGuiLayer()->BlockEvents(!m_ViewPortFocused || !m_ViewPortHovered);
		
		ImVec2 viewPanelSize = ImGui::GetContentRegionAvail();
		
		if (m_ViewPortSize != *(glm::vec2*)&viewPanelSize && viewPanelSize.x>0 && viewPanelSize.y > 0) {

			m_Framebuffer->Resize((uint32_t)viewPanelSize.x, (uint32_t)viewPanelSize.y);
			m_ViewPortSize = { viewPanelSize.x,viewPanelSize.y };
			m_CameraController.OnResize(viewPanelSize.x, viewPanelSize.y);
		}
		ImGui::Image((void*)texID, viewPanelSize, ImVec2(0, 1), ImVec2(1, 0));
		ImGui::End();
		ImGui::PopStyleVar();
		ImGui::End();

	}

	void EditorLayer::OnEvent(Sas::Event& event)
	{
		m_CameraController.OnEvent(event);
	}
}