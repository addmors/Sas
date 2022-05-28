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
	#define SS_ENABLE_ASSERTS
#endif

#ifdef SS_ENABLE_ASSERTS
	#define SS_ASSERT(x, ...) {if(!(x)) {SS_ERROR("Assert Failed: {0}", __VA_ARGS__); __debugbreak();} }
	#define SS_CORE_ASSERT(x, ...) {if(!(x)) {SS_CORE_ERROR("Assert Failed: {0}", __VA_ARGS__); __debugbreak();} }
#else
	#define SS_ASSERT(x, ...)
	#define SS_CORE_ASSERT(x, ...)

#endif

#define BIT(x) (1<<x)

#define  SS_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1) 


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