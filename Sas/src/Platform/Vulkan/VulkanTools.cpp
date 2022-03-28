#include "ssph.h"
#include "VulkanTools.h"
#include <vulkan\vulkan.h>

namespace Sas {
	namespace vks
	{
		namespace tools{
			std::string errorString(VkResult errorCode)
			{
				switch (errorCode)
				{
#define STR(r) case VK_ ##r: return #r
					STR(NOT_READY);
					STR(TIMEOUT);
					STR(EVENT_SET);
					STR(EVENT_RESET);
					STR(INCOMPLETE);
					STR(ERROR_OUT_OF_HOST_MEMORY);
					STR(ERROR_OUT_OF_DEVICE_MEMORY);
					STR(ERROR_INITIALIZATION_FAILED);
					STR(ERROR_DEVICE_LOST);
					STR(ERROR_MEMORY_MAP_FAILED);
					STR(ERROR_LAYER_NOT_PRESENT);
					STR(ERROR_EXTENSION_NOT_PRESENT);
					STR(ERROR_FEATURE_NOT_PRESENT);
					STR(ERROR_INCOMPATIBLE_DRIVER);
					STR(ERROR_TOO_MANY_OBJECTS);
					STR(ERROR_FORMAT_NOT_SUPPORTED);
					STR(ERROR_SURFACE_LOST_KHR);
					STR(ERROR_NATIVE_WINDOW_IN_USE_KHR);
					STR(SUBOPTIMAL_KHR);
					STR(ERROR_OUT_OF_DATE_KHR);
					STR(ERROR_INCOMPATIBLE_DISPLAY_KHR);
					STR(ERROR_VALIDATION_FAILED_EXT);
					STR(ERROR_INVALID_SHADER_NV);
#undef STR
				default:
					return "UNKNOWN_ERROR";
				}
			}

			void exitFatal(const std::string& message, int32_t exitCode)
			{
				if (!errorModeSilent) {
					MessageBoxA(NULL, message.c_str(), NULL, MB_OK | MB_ICONERROR);
				}
				exit(exitCode);
			}
			
			void exitFatal(const std::string& message, VkResult resultCode)
			{
				exitFatal(message, (int32_t)resultCode);
			}

			VkBool32 getSupportedDepthFormat(VkPhysicalDevice physicalDevice, VkFormat* depthFormat)
			{
				// Since all depth formats may be optional, we need to find a suitable depth format to use
				// Start with the highest precision packed format
				std::vector<VkFormat> depthFormats = {
					VK_FORMAT_D32_SFLOAT_S8_UINT,
					VK_FORMAT_D32_SFLOAT,
					VK_FORMAT_D24_UNORM_S8_UINT,
					VK_FORMAT_D16_UNORM_S8_UINT,
					VK_FORMAT_D16_UNORM
				};

				for (auto& format : depthFormats)
				{
					VkFormatProperties formatProps;
					vkGetPhysicalDeviceFormatProperties(physicalDevice, format, &formatProps);
					// Format must support depth stencil attachment for optimal tiling
					if (formatProps.optimalTilingFeatures & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT)
					{
						*depthFormat = format;
						return true;
					}
				}

				return false;
			}
		}
	}
}