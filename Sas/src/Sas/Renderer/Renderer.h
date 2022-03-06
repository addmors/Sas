#pragma once
#include "RendererComand.h"
#include "OrthographicCamera.h"
#include "Shader.h"

namespace Sas {

	class Renderer {
	public:
		static void OnWindowResize(uint32_t width, uint32_t height);
		static void Init();
		static void BeginScene(OrthographicCamera& camera);
		static void EndScene();

		static void Submit(const Ref<Shader> shader, const Ref<VertexArray>& vertexArray,const glm::mat4& transform);
		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI();};


	private:
		struct  SceneData
		{
			glm::mat4 ViewProjectionMatrix; 
		};

		static SceneData* m_SceneData;
	};
}