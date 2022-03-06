#include "SandBox2D.h"

#include "imgui/imgui.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

SandBox2D::SandBox2D()
	:Layer("Sendbox2D"), m_CameraController(1280.0f/720.0f)
{

};

void SandBox2D::OnAttach()
{


}

void SandBox2D::OnDetach()
{

}

void SandBox2D::OnUpdate(Sas::Timestep ts)
{
	m_CameraController.OnUpdate(ts);
	Sas::RendererComand::SetClearColor({ 0.2f, 0.2f, 0.2f, 1.0f });

	Sas::RendererComand::Clear();

	Sas::Renderer2D::BeginScene(m_CameraController.GetCamera());

	Sas::Renderer2D::DrawQuad({ 0.0f,0.0f }, { 1.0f,1.0f }, {0.8f,0.2f,0.3f,1.0f});
	Sas::Renderer2D::EndScene();


}

void SandBox2D::OnImGuiRender()
{
	ImGui::Begin("Test");
	ImGui::ColorEdit4("Squares color", glm::value_ptr(m_SquareColor));
	ImGui::End();

}

void SandBox2D::OnEvent(Sas::Event& event)
{
	m_CameraController.OnEvent(event);
}
