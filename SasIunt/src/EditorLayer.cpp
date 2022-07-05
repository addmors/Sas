#include "EditorLayer.h"
#include <chrono>
#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "Sas/Scene/SceneSerializer.h"

#include "Sas/Utils/PlatformUtils.h"
#include "ImGuizmo.h"

#include "Sas/Math.h"
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
		
		auto commandLineArgs = Application::Get().GetCommandLineArgs();
		if (commandLineArgs.Count > 1)
		{
			auto sceneFilePath = commandLineArgs[1];
			SceneSerializer serializer(m_ActiveScene);
			serializer.Deserialize(sceneFilePath);
		}

		m_EditorCamera = EditorCamera(30.0f, 1.778f, 0.1f, 1000.0f);
#if 0
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
		m_CameraEntity.AddComponent<NativeScriptComponent>().Bind<CameraController>();
#endif
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
			m_EditorCamera.SetViewportSize(m_ViewPortSize.x, m_ViewPortSize.y);
			m_ActiveScene->OnViewPortResize((uint32_t)m_ViewPortSize.x, (uint32_t)m_ViewPortSize.y);
		}
		
		//Update Scene
		if (m_ViewPortFocused) 
		{
			m_CameraController.OnUpdate(ts);
			m_EditorCamera.OnUpdate(ts);
		}

		Sas::Renderer2D::ResetStats();
		m_Framebuffer->Bind();
		Sas::RenderCommand::SetClearColor({ 0.2f, 0.2f, 0.2f, 1.0f });
		Sas::RenderCommand::Clear();

		// Clear our entity ID attachment to -1
		m_Framebuffer->ClearAttachment(1, -1);

		m_ActiveScene->OnUpdateEditor(ts, m_EditorCamera);
		auto [mx, my] = ImGui::GetMousePos();
		mx -= m_ViewportBounds[0].x;
		my -= m_ViewportBounds[0].y;
		glm::vec2 viewportSize = m_ViewportBounds[1] - m_ViewportBounds[0];
		my = viewportSize.y - my;
		int mouseX = (int)mx;
		int mouseY = (int)my;

		if (mouseX >= 0 && mouseY >= 0 && mouseX < (int)viewportSize.x && mouseY < (int)viewportSize.y)
		{
			int pixelData = m_Framebuffer->ReadPixel(1, mouseX, mouseY);
			m_HoveredEntity = pixelData == -1 ? Entity() : Entity( (entt::entity)pixelData, m_ActiveScene.get());
		
		}
		m_Framebuffer->Unbind();
		
	}

	void EditorLayer::OnImGuiRender()
	{


		static bool dockspaceOpen = true;
		static bool opt_fullscreen_persistant = true;
		bool opt_fullscreen = opt_fullscreen_persistant;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)
		{
			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->Pos);
			ImGui::SetNextWindowSize(viewport->Size);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}
	

		// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
		// and handle the pass-thru hole, so we ask Begin() to not render a background.
		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
		// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
		// all active windows docked into it will lose their parent and become unlocked.
		// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
		// any change of dock space/settings would lead to windows being stuck in limbo and never being visible.
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
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

			if (ImGui::BeginMenu("File"))
			{
				
				// Disabling full screen would allow the window to be moved to the front of other windows,
				// which we can't undo at the moment without finer window depth/z control.
				
				
				if (ImGui::MenuItem("New", "Ctrl+N"))
				{
					NewScene();
				}
				if (ImGui::MenuItem("Open...", "Ctrl+O"))
				{
					OpenScene();
				}
				if (ImGui::MenuItem("Save as...", "Ctrl+Shift+S"))
				{
					SaveSceneAs();
				}
				if (ImGui::MenuItem("Exit")) Sas::Application::Get().Close();


				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}
		m_SceneHierarchyPanel.OnImGuiRender();
		auto stat = Sas::Renderer2D::GetStats();
		
		ImGui::Begin("Stats");

		std::string name = "None";
		if (m_HoveredEntity) {
			name = m_HoveredEntity.GetComponent<TagComponent>().Tag;
			ImGui::Text("Hovered Entity: %s", name.c_str());
		}
		ImGui::Text("Render2D stat");
		ImGui::Text("Draw Call: %d", stat.DrawCalls);
		ImGui::Text("Quad: %d", stat.QuadCount);
		ImGui::Text("Vertices: %df", stat.GetTotalVertexCount());
		ImGui::Text("Indices: %df", stat.GetTotalIndexCount());
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

		
		ImGui::End(); 
		
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{0,0});
		ImGui::Begin("Viewport");
		auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
		auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
		auto viewportOffset = ImGui::GetWindowPos();
		m_ViewportBounds[0] = { viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y };
		m_ViewportBounds[1] = { viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y };

		m_ViewPortFocused = ImGui::IsWindowFocused();
		ViewPortfocused = m_ViewPortFocused;
		m_ViewPortHovered = ImGui::IsWindowHovered();

		Application::Get().GetImGuiLayer()->BlockEvents(!m_ViewPortFocused && !m_ViewPortHovered);

		
		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		m_ViewPortSize = { viewportPanelSize.x, viewportPanelSize.y };

		uint64_t textureID = m_Framebuffer->GetColorAttachmentRendererID();
		ImGui::Image(reinterpret_cast<void*>(textureID), ImVec2{ m_ViewPortSize.x, m_ViewPortSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
		
		//Gizmo's

		Entity selectedEntity = m_SceneHierarchyPanel.GetSelectedEntity();


		//auto posx = Application::Get().GetWindow().GetPosX() + 70;
		//auto posy = Application::Get().GetWindow().GetPosY() + 40;
		if(selectedEntity && m_GizmoType != -1)
		{
			ImGuizmo::SetOrthographic(false);
			ImGuizmo::SetDrawlist();
			ImGuizmo::SetRect(m_ViewportBounds[0].x, m_ViewportBounds[0].y, m_ViewportBounds[1].x - m_ViewportBounds[0].x, m_ViewportBounds[1].y - m_ViewportBounds[0].y);

			//Camera
			//auto cameraEntity = m_ActiveScene->GetPrimaryCameraEntity();
			//const auto& camera = cameraEntity.GetComponent<CameraComponent>().Camera;
			//glm::mat4 cameraProjection = camera.GetProjection();
 			//glm::mat4 cameraView = glm::inverse(cameraEntity.GetComponent<TransformComponent>().GetTransform());

			//Editor Camera
			glm::mat4 cameraProjection = m_EditorCamera.GetProjection();
			glm::mat4 cameraView = m_EditorCamera.GetViewMatrix();


			//Entity Transformation
			auto& tc = selectedEntity.GetComponent<TransformComponent>();
			glm::mat4 transform = tc.GetTransform();

			// Snapping
			bool snap = Input::IsKeyPressed(Key::LeftControl);

			float snapValue = 0.5f; // Snap to 0.5m for translation/scale
			// Snap to 45 degrees for rotation
			if (m_GizmoType == ImGuizmo::OPERATION::ROTATE)
				snapValue = 45.0f;

			float snapValues[3] = { snapValue, snapValue, snapValue };

			ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection),
				(ImGuizmo::OPERATION)m_GizmoType, ImGuizmo::LOCAL, glm::value_ptr(transform),
				nullptr, snap ? snapValues : nullptr);

			if (ImGuizmo::IsUsing()) {
				glm::vec3 translation, rotation, scale;
				Math::DecomposeTransform(transform, translation, rotation, scale);
	
				glm::vec3 deltaRotation = rotation - tc.Rotation;
				tc.Translation = translation;
				tc.Rotation += deltaRotation;
				tc.Scale = scale;
			}
		}

		ImGui::End();
		ImGui::PopStyleVar();
		ImGui::End();

	}

	void EditorLayer::OnEvent(Sas::Event& event)
	{
		m_CameraController.OnEvent(event);
		m_EditorCamera.OnEvent(event);

		EventDispatcher dispather(event);
		dispather.Dispatch<KeyPressedEvent>(SS_BIND_EVENT_FN(EditorLayer::OnKeyPressed));
		dispather.Dispatch<MouseButtonPressedEvent>(SS_BIND_EVENT_FN(EditorLayer::OnMouseButtonPressed));
	}


	void EditorLayer::NewScene()
	{
		m_ActiveScene = CreateRef<Scene>();
		m_ActiveScene->OnViewPortResize((uint32_t)m_ViewPortSize.x, (uint32_t)m_ViewPortSize.y);
		m_SceneHierarchyPanel.SetContext(m_ActiveScene);
	}

	void EditorLayer::OpenScene()
	{
		std::string filepath = FileDialogs::OpenFile("Sas Scene (*.sas)\0*.sas\0");

		if (!filepath.empty())
		{

			m_ActiveScene = CreateRef<Scene>();
			m_ActiveScene->OnViewPortResize((uint32_t)m_ViewPortSize.x, (uint32_t)m_ViewPortSize.y);
			m_SceneHierarchyPanel.SetContext(m_ActiveScene);
			SceneSerializer serializer(m_ActiveScene);
			serializer.Deserialize(filepath);
		}
	}

	void EditorLayer::SaveSceneAs()
	{
		std::string filepath = FileDialogs::SaveFile("Sas Scene (*.sas)\0*.sas\0");

		if (!filepath.empty())
		{
			SceneSerializer serializer(m_ActiveScene);
			serializer.Serialize(filepath);
		}
	}

	void EditorLayer::SaveScene()
	{

	}

	bool EditorLayer::OnKeyPressed(KeyPressedEvent& e)
	{
		// Shortcuts
		if (e.GetRepeatCount() > 0)
			return false;

		bool control = Input::IsKeyPressed(Key::LeftControl) || Input::IsKeyPressed(Key::RightControl);
		bool shift = Input::IsKeyPressed(Key::LeftShift) || Input::IsKeyPressed(Key::RightShift);
		
		switch (e.GetKeyKode())
		{
			case Key::N:
			{
				if (control)
					NewScene();
			
				break;
			}
			case Key::O:
			{
				if (control)
					OpenScene();
			
				break;
			}
			case Key::S:
			{
				if (control)
				{
					if (shift)
						SaveSceneAs();
					else
						SaveScene();
				}
				break;
			}
			case Key::Q:
			{
				if (!ImGuizmo::IsUsing())
					m_GizmoType = -1;
				break;
			}
			case Key::W:
			{
				if (!ImGuizmo::IsUsing())
					m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;
				break;
			}
			case Key::E:
			{
				if (!ImGuizmo::IsUsing())
					m_GizmoType = ImGuizmo::OPERATION::ROTATE;
				break;
			}
			case Key::R:
			{
				if (!ImGuizmo::IsUsing())
					m_GizmoType = ImGuizmo::OPERATION::SCALE;
				break;
			}
		}
	}
	bool EditorLayer::OnMouseButtonPressed(MouseButtonPressedEvent& e)
	{

		if (e.GetMouseButton() == Mouse::Button0) {
			if(m_ViewPortHovered && !ImGuizmo::IsOver() && !Input::IsKeyPressed(Key::LeftAlt))
				m_SceneHierarchyPanel.SetSelectedEntity(m_HoveredEntity);
		}
		return false;
	}
}