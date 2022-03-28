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
#include <sstream>

#include <windows.h>
#include <fcntl.h>
#include <io.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>


namespace Sas {
	namespace vks {
		namespace debug {

			// Default validation layers
			extern int validationLayerCount;
			extern const char* validationLayerNames[];
			// Default debug callback
			VKAPI_ATTR VkBool32 VKAPI_CALL messageCallback(
				VkDebugReportFlagsEXT flags,
				VkDebugReportObjectTypeEXT objType,
				uint64_t srcObject,
				size_t location,
				int32_t msgCode,
				const char* pLayerPrefix,
				const char* pMsg,
				void* pUserData);

			// Load debug function pointers and set debug callback
			// if callBack is NULL, default message callback will be used
			void setupDebugging(
				VkInstance instance,
				VkDebugReportFlagsEXT flags,
				VkDebugReportCallbackEXT callBack);
		}

		// Setup and functions for the VK_EXT_debug_marker_extension
		// Extension spec can be found at https://github.com/KhronosGroup/Vulkan-Docs/blob/1.0-VK_EXT_debug_marker/doc/specs/vulkan/appendices/VK_EXT_debug_marker.txt
		// Note that the extension will only be present if run from an offline debugging application
		// The actual check for extension presence and enabling it on the device is done in the example base class
		// See VulkanExampleBase::createInstance and VulkanExampleBase::createDevice (base/vulkanexamplebase.cpp)
		namespace debugmarker
		{
			// Set to true if function pointer for the debug marker are available
			extern bool active;

			// Get function pointers for the debug report extensions from the device
			void setup(VkDevice device);
		}
	}
}