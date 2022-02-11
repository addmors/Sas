#pragma once 

#include "Core.h"

#include "Window.h"
#include "LayerStack.h"
#include "Sas/Events/Event.h"

#include "Renderer/Shader.h"
#include "Renderer/Buffer.h"




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


		unsigned int  m_VertexArray;

		bool m_Running = true;
		LayerStack m_LayerStack;
		std::unique_ptr<Shader> m_Shader;
		std::unique_ptr<IndexBuffer> m_IndexBuffer;
		std::unique_ptr<VertexBuffer> m_VertexBuffer;
		
	private:
		static Application* s_Instanse;

	};


	//To be difene in CLIENT
	Application* CreateApplication();
}