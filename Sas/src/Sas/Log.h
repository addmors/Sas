#pragma once
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


#define SS_CORE_ERROR(...) ::Sas::Log::GetCoreLogger()->error(__VA_ARGS__);
#define SS_CORE_WARN(...)  ::Sas::Log::GetCoreLogger()->warn(__VA_ARGS__);
#define SS_CORE_TRACE(...) ::Sas::Log::GetCoreLogger()->trace(__VA_ARGS__);
#define SS_CORE_INFO(...)  ::Sas::Log::GetCoreLogger()->info(__VA_ARGS__);
#define SS_CORE_FATAL(...) ::Sas::Log::GetCoreLogger()->fatal(__VA_ARGS__);



#define SS_ERROR(...) ::Sas::Log::GetClientLogger()->error(__VA_ARGS__);
#define SS_WARN(...)  ::Sas::Log::GetClientLogger()->warn(__VA_ARGS__);
#define SS_TRACE(...) ::Sas::Log::GetClientLogger()->trace(__VA_ARGS__);
#define SS_INFO(...)  ::Sas::Log::GetClientLogger()->info(__VA_ARGS__);
#define SS_FATAL(...) ::Sas::Log::GetClientLogger()->fatal(__VA_ARGS__);