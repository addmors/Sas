#pragma once
#include<string>

namespace Sas {
	class Shader {
	public:
		Shader(const std::string& vertPath, const std::string& fragPath);
		~Shader();
		void Bind() const;
		void UnBind() const;
	private:
		uint32_t m_RenderID;

	};
}