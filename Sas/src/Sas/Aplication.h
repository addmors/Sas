#pragma once 

#include "Core.h"

#include "Window.h"
#include "LayerStack.h"
#include "Sas/Events/Event.h"

#include "Renderer/Shader.h"
#include "Renderer/Buffer.h"
#include "Renderer/VertexArray.h"
#include "Renderer/OrthographicCamera.h"



namespace Sas {

	class WindowCloseEvent;
	class ImGuiLayer;

	class SAS_API Application
	{
	public:
		Application();	
		virtual ~Application();
		
		void Run();
		void onEvent(Event& e);


		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);
		
		inline static Application& Get() { return *s_Instanse; };
		inline Window& GetWindow() {
			return *m_Window;
		};

	private:
		bool OnWindowClosed(WindowCloseEvent& e );
		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;


		
		bool m_Running = true;
		LayerStack m_LayerStack;
		std::shared_ptr<Shader> m_Shader;
		std::shared_ptr<IndexBuffer> m_IndexBuffer;
		std::shared_ptr<VertexBuffer> m_VertexBuffer;
		std::shared_ptr<VertexArray> m_VertexArray;
		std::shared_ptr<VertexArray> m_SquareVA;

		std::shared_ptr<Shader> m_Shader2;
		OrthographicCamera m_Camera;

	private:
		static Application* s_Instanse;

	};


	//To be difene in CLIENT
	Application* CreateApplication();
}