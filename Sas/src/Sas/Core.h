#pragma once


#ifdef SS_PLATFORM_WINDOWS
	#ifdef SS_BUILD_DLL
		#define SAS_API __declspec(dllexport) 
	#else
		#define SAS_API __declspec(dllimport)
	#endif
#else 
	#error Sas omly support Windows;
#endif

#define BIT(x) (1<<x)