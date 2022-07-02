#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/string_cast.hpp"

#include "Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/fmt/ostr.h"

namespace Sas {

	class SAS_API Log {
	public:
		static void Init();

		inline static Ref<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; };
		inline static Ref<spdlog::logger>& GetClientLogger() { return s_ClientLogger; };
		
	private:
		static Ref<spdlog::logger> s_CoreLogger;
		static Ref<spdlog::logger> s_ClientLogger;

	};
}

template<typename OStream, glm::length_t L, typename T, glm::qualifier Q>
inline OStream& operator<<(OStream& os, const glm::vec<L, T, Q>& vector)
{
	return os << glm::to_string(vector);
}

template<typename OStream, glm::length_t C, glm::length_t R, typename T, glm::qualifier Q>
inline OStream& operator<<(OStream& os, const glm::mat<C, R, T, Q>& matrix)
{
	return os << glm::to_string(matrix);
}

template<typename OStream, typename T, glm::qualifier Q>
inline OStream& operator<<(OStream& os, glm::qua<T, Q> quaternion)
{
	return os << glm::to_string(quaternion);
}

// Core log macros
#define SS_CORE_TRACE(...)    ::Sas::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define SS_CORE_INFO(...)     ::Sas::Log::GetCoreLogger()->info(__VA_ARGS__)
#define SS_CORE_WARN(...)     ::Sas::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define SS_CORE_ERROR(...)    ::Sas::Log::GetCoreLogger()->error(__VA_ARGS__)
#define SS_CORE_CRITICAL(...) ::Sas::Log::GetCoreLogger()->critical(__VA_ARGS__)

// Client log macros
#define SS_TRACE(...)         ::Sas::Log::GetClientLogger()->trace(__VA_ARGS__)
#define SS_INFO(...)          ::Sas::Log::GetClientLogger()->info(__VA_ARGS__)
#define SS_WARN(...)          ::Sas::Log::GetClientLogger()->warn(__VA_ARGS__)
#define SS_ERROR(...)         ::Sas::Log::GetClientLogger()->error(__VA_ARGS__)
#define SS_CRITICAL(...)      ::Sas::Log::GetClientLogger()->critical(__VA_ARGS__)