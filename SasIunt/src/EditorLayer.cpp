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
		auto square = m_ActiveScene->CreateEntity("Red Square");
		square.AddComponent<SpriteRendererComponent>(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
		m_SquareEntity = square;

		m_CameraEntity = m_ActiveScene->CreateEntity("Camera Entity");
		m_CameraEntity.AddComponent<CameraComponent>();
		m_SecondCamera = m_ActiveScene->CreateEntity("Clip-Space Entity");
		auto& cc = m_SecondCamera.AddComponent<CameraComponent>();
		cc.Primary = false;

		class CameraController : public ScriptableEntity
		{
		public:
			void OnCreate() {

			}
			void OnDestroy() {

			}
			void OnUpdate(Timestep ts) {
				if (ViewPortfocused) {
					auto& transform = GetComponent<TransformComponent>().Transform;
					float Speed = 5.0f;
					if (Input::IsKeyPressed(Key::A))
						transform[3][0] -= Speed * ts;
					if (Input::IsKeyPressed(Key::D))
						transform[3][0] += Speed * ts;
					if (Input::IsKeyPressed(Key::W))
						transform[3][1] += Speed * ts;
					if (Input::IsKeyPressed(Key::S))
						transform[3][1] -= Speed * ts;
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
		m_SceneHierarchyPanel.OnImGuiRender();
		auto stat = Sas::Renderer2D::GetStats();
		ImGui::Begin("Setting");
		ImGui::Text("Render2D stat");
		ImGui::Text("Draw Call: %d", stat.DrawCalls);
		ImGui::Text("Quad: %d", stat.QuadCount);
		ImGui::Text("Vertices: %df", stat.GetTotalVertexCount());
		ImGui::Text("Indecies: %df", stat.GetTotalIndexCount());
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

		//Set
		if (m_SquareEntity) {
			ImGui::Separator();

			auto& tag = m_SquareEntity.GetComponent<TagComponent>().Tag;
			ImGui::Text("%s", tag.c_str());
			auto& color = m_SquareEntity.GetComponent<SpriteRendererComponent>().Color;
			ImGui::ColorEdit4("Square Color", glm::value_ptr(color));
			ImGui::Separator();
		}
		{
		ImGui::DragFloat3("Camera Transform", glm::value_ptr(m_CameraEntity.GetComponent<TransformComponent>().Transform[3]));
		ImGui::Checkbox("Camera Editor", &m_PrimaryCamera);

		m_CameraEntity.GetComponent<CameraComponent>().Primary = m_PrimaryCamera;
		m_SecondCamera.GetComponent<CameraComponent>().Primary = !m_PrimaryCamera;
		}

		{
			auto& camera = m_SecondCamera.GetComponent<CameraComponent>().Camera;
			float ortho = camera.GetOrthographicSize();
			if(ImGui::DragFloat("Second Camera Ort Size :", &ortho))
				camera.SetOrthographicSize(ortho);
		}
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