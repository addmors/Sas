#pragma once
#include "SasPre.h"

class SandBox2D : public Sas::Layer{
public:
	SandBox2D();
	virtual ~SandBox2D() = default;
	void OnAttach() override;
	void OnDetach() override;
	void OnUpdate(Sas::Timestep ts) override;
	void OnImGuiRender() override;
	void OnEvent(Sas::Event& event) override;
private:
	Sas::OrthographicCameraController m_CameraController;
	Sas::Ref<Sas::VertexArray> m_SquareVA;
	Sas::Ref<Sas::Shader> m_FlatColor;
	
	Sas::Ref<Sas::Texture2D> m_CheckboardTexture;
	glm::vec4 m_SquareColor = { 0.2f,0.3f,0.8f,1.0f};
};