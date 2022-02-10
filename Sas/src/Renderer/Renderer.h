#pragma once

namespace Sas {

	enum class RendererAPI
	{
		None = 0,
		OpenGL = 1,
		Vulcak = 2,
		DirectX = 3
	};
	class Renderer {
	public:
		inline static RendererAPI GetAPI() { return s_RendererAPI; };
	private:
		static RendererAPI s_RendererAPI;
	};
}