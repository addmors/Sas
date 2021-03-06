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
#ifdef ZERO
	m_Particle.ColorBegin = { 254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f };
	m_Particle.ColorEnd = { 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 1.0f };
	m_Particle.SizeBegin = 0.2f, m_Particle.SizeVariation = 0.05f, m_Particle.SizeEnd = 0.0f;
	m_Particle.LifeTime = 1.0f;
	m_Particle.Velocity = { 0.0f, 0.0f };
	m_Particle.VelocityVariation = { 3.0f, 1.0f };
	m_Particle.Position = { 0.0f, 0.0f };
#endif // ZERO

}

void SandBox2D::OnDetach()
{

	SS_PROFILE_FUNCTION();
}

void SandBox2D::OnUpdate(Sas::Timestep ts)
{
	time += ts;
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
		Sas::Renderer2D::DrawRotatedQuad({ 2.0f,0.0f}, {0.5f,0.5f}, -45.0f, m_CheckboardTexture,10.0f);

		Sas::Renderer2D::EndScene();

		Sas::Renderer2D::BeginScene(m_CameraController.GetCamera());
		for (float y = -3.0f; y < 3.0f; y += 0.5f) {
			for (float x = -3.0f; x < 3.0f; x += 0.5f) {
				glm::vec4 rgba = { (x + 5.0f) / 10.0f, 0.4f,(y + 5.0f) / 10.0f , 0.5f };
				Sas::Renderer2D::DrawRotatedQuad({ x,y }, { 0.45f, 0.45f },0, rgba);
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
	}
	
}

void SandBox2D::OnImGuiRender()
{
	
	auto stat = Sas::Renderer2D::GetStats();
	ImGui::Begin("Setting");
	ImGui::Text("Render2D stat");
	ImGui::Text("Draw Call: %d", stat.DrawCalls);
	ImGui::Text("Quad: %d", stat.QuadCount);
	ImGui::Text("Vertices: %df", stat.GetTotalVertexCount());
	ImGui::Text("Indecies: %df", stat.GetTotalIndexCount());

	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	
	ImGui::End();
}
 
void SandBox2D::OnEvent(Sas::Event& event)
{
	m_CameraController.OnEvent(event);
}
