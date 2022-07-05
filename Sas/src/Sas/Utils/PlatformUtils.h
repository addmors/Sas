#pragma once

#include <string>

namespace Sas{

	class SAS_API FileDialogs
	{
	public:
		// These return empty strings if canceled
		static std::string OpenFile(const char* filter);
		static std::string SaveFile(const char* filter);
	};

	class Time
	{
	public:
		static float GetTime();
	};

}