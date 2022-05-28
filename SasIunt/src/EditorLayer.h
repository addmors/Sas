#pragma once
#include "SasPre.h"
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
		OrthographicCameraController m_CameraController;

		//Temp
		Ref<VertexArray> m_SquareVA;
		Ref<Shader> m_FlatColor;
		Ref<Framebuffer> m_Framebuffer;
		Ref<Texture2D> m_CheckboardTexture;
		bool m_ViewPortFocused = false;
		bool m_ViewPortHovered = false;
		glm::vec2 m_ViewPortSize = {0,0};
		float time = 0;
		glm::vec4 m_SquareColor = { 0.2f,0.3f,0.8f,1.0f };
	};
}