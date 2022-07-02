#include "EditorLayer.h"
#include <chrono>
#include "imgui/imgui.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"


namespace Sas {

	static bool ViewPortfocused = false;

	EditorLayer::EditorLayer()
		:Layer("Sendbox2D"), m_CameraController(1280.0f / 720.0f)
	{

	};

	void EditorLayer::OnAttach()
	{

		SS_PROFILE_FUNCTION();
		m_CheckboardTexture = Sas::Texture2D::Create("assets/textures/Checkerboard.png");

		FramebufferSpecification fbSpec;
		fbSpec.Attachments = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::RED_INTEGER, FramebufferTextureFormat::Depth };
		fbSpec.Width = 1280;
		fbSpec.Height = 720;
		m_Framebuffer = Framebuffer::Create(fbSpec);

		m_ActiveScene = CreateRef<Scene>();

		//Entity
		auto square = m_ActiveScene->CreateEntity("Green Square");
		square.AddComponent<SpriteRendererComponent>(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));

		auto redSquare = m_ActiveScene->CreateEntity("Red Square");
		redSquare.AddComponent<SpriteRendererComponent>(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
		m_SquareEntity = square;

		m_CameraEntity = m_ActiveScene->CreateEntity("Camera A");
		m_CameraEntity.AddComponent<CameraComponent>();
		m_SecondCamera = m_ActiveScene->CreateEntity("Camera B");
		auto& cc = m_SecondCamera.AddComponent<CameraComponent>();
		cc.Primary = false;

		class CameraController : public ScriptableEntity
		{
		public:
			void OnCreate() {
				auto& translation = GetComponent<TransformComponent>().Translation;
				translation.x = rand() % 10 - 5;
			}
			void OnDestroy() {

			}
			void OnUpdate(Timestep ts) {
				if (ViewPortfocused) {
					auto& translation = GetComponent<TransformComponent>().Translation;
					float Speed = 5.0f;
					if (Input::IsKeyPressed(Key::A))
						translation.x -= Speed * ts;
					if (Input::IsKeyPressed(Key::D))
						translation.x += Speed * ts;
					if (Input::IsKeyPressed(Key::W))
						translation.y += Speed * ts;
					if (Input::IsKeyPressed(Key::S))
						translation.y -= Speed * ts;
				}

			}

		};
		m_SecondCamera.AddComponent<NativeScriptComponent>().Bind<CameraController>();


		m_SceneHierarchyPanel.SetContext(m_ActiveScene);
	}

	void EditorLayer::OnDetach()
	{

		SS_PROFILE_FUNCTION();
	}

	void EditorLayer::OnUpdate(Sas::Timestep ts)
	{
		SS_PROFILE_FUNCTION();

		
		
		if (FramebufferSpecification spec = m_Framebuffer->GetSpecification();
			m_ViewPortSize.x > 0.0f && m_ViewPortSize.y > 0.0f && // zero sized framebuffer is invalid
			(spec.Width != m_ViewPortSize.x || spec.Height != m_ViewPortSize.y))
		{
			m_Framebuffer->Resize((uint32_t)m_ViewPortSize.x, (uint32_t)m_ViewPortSize.y);
			m_CameraController.OnResize(m_ViewPortSize.x, m_ViewPortSize.y);
			m_ActiveScene->OnViewPortResize((uint32_t)m_ViewPortSize.x, (uint32_t)m_ViewPortSize.y);
		}
		if(m_ViewPortFocused)
			m_CameraController.OnUpdate(ts);
		
		//Update Scene
		Sas::Renderer2D::ResetStats();
		m_Framebuffer->Bind();
		Sas::RenderCommand::SetClearColor({ 0.2f, 0.2f, 0.2f, 1.0f });
		Sas::RenderCommand::Clear();
		
		m_ActiveScene->OnUpdate(ts);
		
		m_Framebuffer->Unbind();
		
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
		ImGuiStyle& style = ImGui::GetStyle();
		float minWinSizeStyleX = style.WindowMinSize.x;
		style.WindowMinSize.x = 370.0f;
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}
		style.WindowMinSize.x = minWinSizeStyleX;

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
		m_SceneHierarchyPanel.OnImGuiRender();
		auto stat = Sas::Renderer2D::GetStats();
		ImGui::Begin("Stats");
		ImGui::Text("Render2D stat");
		ImGui::Text("Draw Call: %d", stat.DrawCalls);
		ImGui::Text("Quad: %d", stat.QuadCount);
		ImGui::Text("Vertices: %df", stat.GetTotalVertexCount());
		ImGui::Text("Indecies: %df", stat.GetTotalIndexCount());
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

		
		ImGui::End(); 
		
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{0,0});
		ImGui::Begin("Viewport");
		m_ViewPortFocused = ImGui::IsWindowFocused();
		ViewPortfocused = m_ViewPortFocused;
		m_ViewPortHovered = ImGui::IsWindowHovered();

		Application::Get().GetImGuiLayer()->BlockEvents(!m_ViewPortFocused || !m_ViewPortHovered);
		
		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		m_ViewPortSize = { viewportPanelSize.x, viewportPanelSize.y };

		uint64_t textureID = m_Framebuffer->GetColorAttachmentRendererID();
		ImGui::Image(reinterpret_cast<void*>(textureID), ImVec2{ m_ViewPortSize.x, m_ViewPortSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
		
		ImGui::End();
		ImGui::PopStyleVar();
		ImGui::End();

	}

	void EditorLayer::OnEvent(Sas::Event& event)
	{
		m_CameraController.OnEvent(event);
	}
}