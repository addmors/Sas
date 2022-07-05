#pragma once
#include "SasPre.h"
#include "Panel/SceneHierarchyPanel.h"
#include "Sas/Renderer/EditorCamera.h"

namespace Sas {
	class EditorLayer : public Sas::Layer {
	public:
		EditorLayer();
		virtual ~EditorLayer() = default;
		void OnAttach() override;
		void OnDetach() override;
		void OnUpdate(Timestep ts) override;
		void OnImGuiRender() override;
		void OnEvent(Event& event) override;
	private:
		bool OnKeyPressed(KeyPressedEvent& e);
		bool OnMouseButtonPressed(MouseButtonPressedEvent& e);

		void NewScene();
		void OpenScene();
		void SaveSceneAs();
		void SaveScene();

	private:
		OrthographicCameraController m_CameraController;

		//Temp
		Ref<VertexArray> m_SquareVA;
		Ref<Shader> m_FlatColor;
		Ref<Framebuffer> m_Framebuffer;
		
		Ref<Texture2D> m_CheckboardTexture;
		Ref<Scene> m_ActiveScene;

		Entity m_SquareEntity;
		Entity m_CameraEntity;
		Entity m_SecondCamera;
		Entity m_HoveredEntity;


		//TEMP
		bool m_PrimaryCamera;
		EditorCamera m_EditorCamera;
		bool m_ViewPortFocused = false, m_ViewPortHovered = false;
		glm::vec2 m_ViewPortSize = {0,0};
		glm::vec2 m_ViewportBounds[2];


		glm::vec4 m_SquareColor = { 0.2f,0.3f,0.8f,1.0f };
		
		int m_GizmoType = -1;
		SceneHierarchyPanel m_SceneHierarchyPanel;
	};
}