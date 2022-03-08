#pragma once
#include "OrthographicCamera.h"
#include "Texture.h"
namespace Sas {
	class Renderer2D
	{
	public:
		static void Init();
		static void ShotDown();
		static void BeginScene(const OrthographicCamera& camera);
		static void EndScene();
		static void DrawQuad(const glm::vec2& pos, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec2& pos, const glm::vec2& size, const Ref<Texture>& texture);
		static void DrawQuad(const glm::vec3& pos, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec3& pos, const glm::vec2& size, const Ref<Texture>& texture);

	private:

	};
}