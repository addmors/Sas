#pragma once
#include <vulkan\vulkan.h>
#include "VulkanTools.h"
#include "VulkanDebug.h"
#include "VulkanDevice.h"

namespace Sas {

	class VulkanBase {
	public:
		VkResult createInstance(bool enableValidation);
		bool InitVulkan();

		/** @brief (Virtual) Called after the physical device features have been read, can be used to set features to enable on the device */
		virtual void getEnabledFeatures();

	private:
		// Vulkan instance, stores all per-application states
		VkInstance instance;
		std::vector<std::string> supportedInstanceExtensions;

		// Physical device (GPU) that Vulkan will use
		VkPhysicalDevice physicalDevice;
		// Stores physical device properties (for e.g. checking device limits)
		VkPhysicalDeviceProperties deviceProperties;
		// Stores the features available on the selected physical device (for e.g. checking if a feature is available)
		VkPhysicalDeviceFeatures deviceFeatures;
		// Stores all available memory (type) properties for the physical device
		VkPhysicalDeviceMemoryProperties deviceMemoryProperties;
		/** @brief Set of physical device features to be enabled for this example (must be set in the derived constructor) */
		VkPhysicalDeviceFeatures enabledFeatures{};
		/** @brief Set of device extensions to be enabled for this example (must be set in the derived constructor) */
		std::vector<const char*> enabledDeviceExtensions;
		std::vector<const char*> enabledInstanceExtensions;
		/** @brief Optional pNext structure for passing extension structures to device creation */
		void* deviceCreatepNextChain = nullptr;
		/** @brief Logical device, application's view of the physical device (GPU) */
		VkDevice device;

		/** @brief Encapsulated physical and logical vulkan device */
		vks::VulkanDevice* vulkanDevice;

		struct Settings {
			/** @brief Activates validation layers (and message output) when set to true */
			bool validation = false;
			/** @brief Set to true if fullscreen mode has been requested via command line */
			bool fullscreen = false;
			/** @brief Set to true if v-sync will be forced for the swapchain */
			bool vsync = false;
			/** @brief Enable UI overlay */
			bool overlay = true;
		} settings;

		std::string title = "Vulkan Example";
		std::string name = "vulkanExample";
		uint32_t apiVersion = VK_API_VERSION_1_0;

	};
}