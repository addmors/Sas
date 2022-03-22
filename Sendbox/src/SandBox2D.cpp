#include "SandBox2D.h"
#include <chrono>
#include "imgui/imgui.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"


SandBox2D::SandBox2D()
	:Layer("Sendbox2D"), m_CameraController(1280.0f/720.0f)
{

};

void SandBox2D::OnAttach()
{

	SS_PROFILE_FUNCTION();
	m_CheckboardTexture = Sas::Texture2D::Create("assets/textures/Checkerboard.png");
}

void SandBox2D::OnDetach()
{

	SS_PROFILE_FUNCTION();
}

void SandBox2D::OnUpdate(Sas::Timestep ts)
{
	SS_PROFILE_FUNCTION(); 
	m_CameraController.OnUpdate(ts);
	Sas::Renderer2D::ResetStats();
	{
		SS_PROFILE_SCOPE("Sandbox2D clean");

		Sas::RenderCommand::SetClearColor({ 0.2f, 0.2f, 0.2f, 1.0f });
		Sas::RenderCommand::Clear();
	}

	{
		Sas::Renderer2D::BeginScene(m_CameraController.GetCamera());
		SS_PROFILE_SCOPE("Sandbox2D draw");
		Sas::Renderer2D::DrawQuad({ -1.0f,0.0f }, { 1.0f,1.0f }, { 0.8f,0.2f,0.3f,1.0f });
		Sas::Renderer2D::DrawQuad({ -0.5f,-0.5f }, { 0.2f,0.8f }, { 0.3f,0.2f,0.8f,1.0f });
		
		Sas::Renderer2D::DrawQuad({ -0.0f,-0.0f, -0.1f }, { 10.0f, 10.0f }, m_CheckboardTexture, 10.0f); 
		Sas::Renderer2D::DrawRotatedQuad({ 2.0f,0.0f}, {0.0f,0.0f}, -45.0f, m_CheckboardTexture,10.0f);

		Sas::Renderer2D::EndScene();

		Sas::Renderer2D::BeginScene(m_CameraController.GetCamera());
		for (float y = -50.0f; y < 50.0f; y += 0.5f) {
			for (float x = -50.0f; x < 50.0f; x += 0.5f) {
				glm::vec4 rgba = { (x + 50.0f) / 100.0f, 0.4f,(y + 50.0f) / 100.0f , 0.5f };
				Sas::Renderer2D::DrawQuad({ x,y }, { 0.45f, 0.45f }, rgba);
			}
		}
		Sas::Renderer2D::EndScene();
	}
}

void SandBox2D::OnImGuiRender()
{
	auto stat = Sas::Renderer2D::GetStats();
	ImGui::Begin("Setting");
	ImGui::Text("Render2D stat");
	ImGui::Text("Draw Call: %d", stat.DrawCalls);
	ImGui::Text("Quad: %d", stat.QuadCount);
	ImGui::Text("Vertices: %d", stat.GetTotalVertexCount());
	ImGui::Text("Indecies: %d", stat.GetTotalIndexCount());
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::End();

}

void SandBox2D::OnEvent(Sas::Event& event)
{
	m_CameraController.OnEvent(event);
}
