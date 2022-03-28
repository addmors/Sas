#pragma once
#include "vulkan/vulkan.h"

#include <math.h>
#include <stdlib.h>
#include <string>
#include <cstring>
#include <fstream>
#include <assert.h>
#include <stdio.h>
#include <vector>
#include <iostream>
#include <stdexcept>
#include <fstream>
#if defined(_WIN32)
#include <windows.h>
#include <fcntl.h>
#include <io.h>
#endif

#define VK_CHECK_RESULT(f)																				\
{																										\
	VkResult res = (f);																					\
	if (res != VK_SUCCESS)																				\
	{																									\
		std::cout << "Fatal : VkResult is \"" << vks::tools::errorString(res) << "\" in " << __FILE__ << " at line " << __LINE__ << "\n"; \
		SS_CORE_ASSERT(res == VK_SUCCESS, "Vulkan Debug is not Sucsess");								\
	}																									\
}

namespace Sas {
	namespace vks
	{
		namespace tools {
			
			/** @brief Disable message boxes on fatal errors */
			extern bool errorModeSilent;

			VkBool32 getSupportedDepthFormat(VkPhysicalDevice physicalDevice, VkFormat* depthFormat);
			std::string errorString(VkResult errorCode);
			void exitFatal(const std::string& message, int32_t exitCode);
			void exitFatal(const std::string& message, VkResult resultCode);
		}
	}
}

