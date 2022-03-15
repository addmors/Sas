#include "SandBox2D.h"
#include <chrono>
#include "imgui/imgui.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

template<typename Fn>
class Timer {
public:
	Timer(const char* name, Fn&& func) 
		:m_Name(name),m_Func(func), m_Stoped(false)
	{
		m_StartPoint = std::chrono::high_resolution_clock::now();
	};
	~Timer() {
		if (!m_Stoped)
			Stop();
	}
	void Stop() {
		
		float time = std::chrono::duration_cast<std::chrono::nanoseconds>
			(std::chrono::high_resolution_clock::now() - m_StartPoint).count() * 0.001f * 0.001f;
		//SS_CORE_TRACE("{0} Duration : {1} ms", m_Name, time);
		m_Func({m_Name,time});
	};

private:
	const char* m_Name;
	std::chrono::time_point<std::chrono::high_resolution_clock> m_StartPoint;
	Fn m_Func;
	bool m_Stoped;
};

#define PROFILE_SCOPE(name) Timer timer##__LINE__(name, [&](ProfileRes profileResult){m_ProfileVector.push_back(profileResult);})

SandBox2D::SandBox2D()
	:Layer("Sendbox2D"), m_CameraController(1280.0f/720.0f)
{

};

void SandBox2D::OnAttach()
{
	m_CheckboardTexture = Sas::Texture2D::Create("assets/textures/Checkerboard.png");
}

void SandBox2D::OnDetach()
{

}

void SandBox2D::OnUpdate(Sas::Timestep ts)
{
	PROFILE_SCOPE("Sandbox2D update"); 
	{
		PROFILE_SCOPE("Sandbox2D camer update");
		m_CameraController.OnUpdate(ts);
	}
	{
		PROFILE_SCOPE("Sandbox2D clean");

		Sas::RenderCommand::SetClearColor({ 0.2f, 0.2f, 0.2f, 1.0f });
		Sas::RenderCommand::Clear();
	}

	Sas::Renderer2D::BeginScene(m_CameraController.GetCamera());
	{
		PROFILE_SCOPE("Sandbox2D draw");
		Sas::Renderer2D::DrawQuad({ -1.0f,0.0f }, { 1.0f,1.0f }, { 0.8f,0.2f,0.3f,1.0f });
		Sas::Renderer2D::DrawQuad({ -0.5f,-0.5f }, { 0.2f,0.2f }, { 0.3f,0.2f,0.8f,1.0f });
		Sas::Renderer2D::DrawQuad({ 0.0f,0.0f, -0.1f }, { 10.0f, 10.0f }, m_CheckboardTexture); 
	}
	Sas::Renderer2D::EndScene();


}

void SandBox2D::OnImGuiRender()
{
	ImGui::Begin("Test");
	ImGui::ColorEdit4("Squares color", glm::value_ptr(m_SquareColor));
	
	for (auto& res : m_ProfileVector) {
		char label[50];

		strcpy(label, "%.3fms");
		strcat(label, res.Name);
		ImGui::Text(label, res.Time);
	}
	m_ProfileVector.clear();
	ImGui::End();



}

void SandBox2D::OnEvent(Sas::Event& event)
{
	m_CameraController.OnEvent(event);
}
