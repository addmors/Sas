#pragma once

#include <memory>

#ifdef SS_PLATFORM_WINDOWS
#if SS_DYNAMIC_LINK
	#ifdef SS_BUILD_DLL
		#define SAS_API __declspec(dllexport) 
	#else
		#define SAS_API __declspec(dllimport)
	#endif
#else
	#define SAS_API
#endif
#else 
	#error Sas omly support Windows;
#endif

#ifdef SS_DEBUG
	#if defined(SS_PLATFORM_WINDOWS)
		#define SS_DEBUGBREAK() __debugbreak()
	#elif defined(SS_PLATFORM_LINUX)
		#include <signal.h>
		#define SS_DEBUGBREAK() raise(SIGTRAP)
	#else
		#error "Platform doesn't support debugbreak yet!"
	#endif
		#define HZ_ENABLE_ASSERTS
#else
#define SS_DEBUGBREAK()
#endif

#define SS_EXPAND_MACRO(x) x
#define SS_STRINGIFY_MACRO(x) #x

#define BIT(x) (1<<x)

#define  SS_BIND_EVENT_FN(fn) [this](auto&&... args)->decltype(auto) {return this->fn(std::forward<decltype(args)>(args)...);} 


namespace Sas {
	template<typename T>
	using Scope = std::unique_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}
	template<typename T2, typename T>
	constexpr Scope<T2> Cast(Scope<T> scope){
		return std::static_pointer_cast<T2>(scope);
	}

	template<typename T>
	using Ref = std::shared_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}
	template<typename T2, typename T>
	constexpr Ref<T2> Cast(Ref<T> ref) {
		return std::static_pointer_cast<T2>(ref);
	}

}

#include "Sas/Core/Log.h"
#include "Sas/Core/Assert.h"