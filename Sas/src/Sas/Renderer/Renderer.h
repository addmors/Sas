#pragma once
#include "RendererComand.h"
#include "OrthographicCamera.h"
#include "Shader.h"

namespace Sas {

	class Renderer {
	public:
		static void BeginScene(OrthographicCamera& camera);
		static void EndScene();

		static void Submit(const std::shared_ptr<Shader> shader, const std::shared_ptr<VertexArray>& vertexArray,const glm::mat4& transform);
		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI();};


	private:
		struct  SceneData
		{
			glm::mat4 ViewProjectionMatrix; 
		};

		static SceneData* m_SceneData;
	};
}