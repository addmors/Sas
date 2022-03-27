#include "ssph.h"
#include "VulkanBase.h"
#include <vulkan\vulkan_win32.h>

namespace Sas{
	VkResult VulkanBase::createInstance(bool enableValidation)
	{
		VkApplicationInfo appInfo = {};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = name.c_str();
		appInfo.pEngineName = name.c_str();
		appInfo.apiVersion = apiVersion;

		std::vector<const char*> instanceExtensions = { VK_KHR_SURFACE_EXTENSION_NAME };
		#if defined(_WIN32)
			instanceExtensions.push_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);
		#endif

		uint32_t extCount = 0;
		vkEnumerateInstanceExtensionProperties(nullptr, &extCount, nullptr);
		if (extCount > 0)
		{
			std::vector<VkExtensionProperties> extensions(extCount);
			if (vkEnumerateInstanceExtensionProperties(nullptr, &extCount, &extensions.front()) == VK_SUCCESS)
			{
				for (VkExtensionProperties extension : extensions)
				{
					supportedInstanceExtensions.push_back(extension.extensionName);
				}
			}
		}

		// Enabled requested instance extensions
		if (enabledInstanceExtensions.size() > 0)
		{
			for (const char* enabledExtension : enabledInstanceExtensions)
			{
				// Output message if requested extension is not available
				if (std::find(supportedInstanceExtensions.begin(), supportedInstanceExtensions.end(), enabledExtension) == supportedInstanceExtensions.end())
				{
					std::cerr << "Enabled instance extension \"" << enabledExtension << "\" is not present at instance level\n";
				}
				instanceExtensions.push_back(enabledExtension);
			}
		}

		VkInstanceCreateInfo instanceCreateInfo = {};
		instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		instanceCreateInfo.pNext = NULL;
		instanceCreateInfo.pApplicationInfo = &appInfo;
		if (instanceExtensions.size() > 0)
		{
			if (settings.validation)
			{
				instanceExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
			}
			instanceCreateInfo.enabledExtensionCount = (uint32_t)instanceExtensions.size();
			instanceCreateInfo.ppEnabledExtensionNames = instanceExtensions.data();
		}

		const char* validationLayerName = "VK_LAYER_KHRONOS_validation";
		if (settings.validation)
		{
			// Check if this layer is available at instance level
			uint32_t instanceLayerCount;
			vkEnumerateInstanceLayerProperties(&instanceLayerCount, nullptr);
			std::vector<VkLayerProperties> instanceLayerProperties(instanceLayerCount);
			vkEnumerateInstanceLayerProperties(&instanceLayerCount, instanceLayerProperties.data());
			bool validationLayerPresent = false;
			for (VkLayerProperties layer : instanceLayerProperties) {
				if (strcmp(layer.layerName, validationLayerName) == 0) {
					validationLayerPresent = true;
					break;
				}
			}
			if (validationLayerPresent) {
				instanceCreateInfo.ppEnabledLayerNames = &validationLayerName;
				instanceCreateInfo.enabledLayerCount = 1;
			}
			else {
				std::cerr << "Validation layer VK_LAYER_KHRONOS_validation not present, validation is disabled";
			}
		}
		return vkCreateInstance(&instanceCreateInfo, nullptr, &instance);
	}


	bool VulkanBase::InitVulkan()
	{
		VkResult err;
		err = createInstance(settings.validation);
		if (err) {
			vks::tools::exitFatal("Could not create Vulkan instance : \n" + vks::tools::errorString(err), err);
			return false;
		}
		// If requested, we enable the default validation layers for debugging
		if (settings.validation)
		{
			// The report flags determine what type of messages for the layers will be displayed
			// For validating (debugging) an application the error and warning bits should suffice
			VkDebugReportFlagsEXT debugReportFlags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT;
			// Additional flags include performance info, loader and layer debug messages, etc.
			vks::debug::setupDebugging(instance, debugReportFlags, VK_NULL_HANDLE);
		}

		// Physical device
		uint32_t gpuCount = 0;
		// Get number of available physical devices
		VK_CHECK_RESULT(vkEnumeratePhysicalDevices(instance, &gpuCount, nullptr));
		if (gpuCount == 0) {
			vks::tools::exitFatal("No device with Vulkan support found", -1);
			return false;
		}
		// Enumerate devices
		std::vector<VkPhysicalDevice> physicalDevices(gpuCount);
		err = vkEnumeratePhysicalDevices(instance, &gpuCount, physicalDevices.data());
		if (err) {
			vks::tools::exitFatal("Could not enumerate physical devices : \n" + vks::tools::errorString(err), err);
			return false;
		}
		uint32_t selectedDevice = 0;
#if 0
#if !defined(VK_USE_PLATFORM_ANDROID_KHR)
		// GPU selection via command line argument
		if (commandLineParser.isSet("gpuselection")) {
			uint32_t index = commandLineParser.getValueAsInt("gpuselection", 0);
			if (index > gpuCount - 1) {
				std::cerr << "Selected device index " << index << " is out of range, reverting to device 0 (use -listgpus to show available Vulkan devices)" << "\n";
			}
			else {
				selectedDevice = index;
			}
		}
		if (commandLineParser.isSet("gpulist")) {
			std::cout << "Available Vulkan devices" << "\n";
			for (uint32_t i = 0; i < gpuCount; i++) {
				VkPhysicalDeviceProperties deviceProperties;
				vkGetPhysicalDeviceProperties(physicalDevices[i], &deviceProperties);
				std::cout << "Device [" << i << "] : " << deviceProperties.deviceName << std::endl;
				std::cout << " Type: " << vks::tools::physicalDeviceTypeString(deviceProperties.deviceType) << "\n";
				std::cout << " API: " << (deviceProperties.apiVersion >> 22) << "." << ((deviceProperties.apiVersion >> 12) & 0x3ff) << "." << (deviceProperties.apiVersion & 0xfff) << "\n";
			}
		}
#endif
#endif
		physicalDevice = physicalDevices[selectedDevice];

		// Store properties (including limits), features and memory properties of the physical device (so that examples can check against them)
		vkGetPhysicalDeviceProperties(physicalDevice, &deviceProperties);
		vkGetPhysicalDeviceFeatures(physicalDevice, &deviceFeatures);
		vkGetPhysicalDeviceMemoryProperties(physicalDevice, &deviceMemoryProperties);

		// Derived examples can override this to set actual features (based on above readings) to enable for logical device creation
		getEnabledFeatures();
		// Vulkan device creation
		// This is handled by a separate class that gets a logical device representation
		// and encapsulates functions related to a device
		vulkanDevice = new vks::VulkanDevice(physicalDevice);
		VkResult res = vulkanDevice->createLogicalDevice(enabledFeatures, enabledDeviceExtensions, deviceCreatepNextChain);
		if (res != VK_SUCCESS) {
			vks::tools::exitFatal("Could not create Vulkan device: \n" + vks::tools::errorString(res), res);
			return false;
		}
		device = vulkanDevice->logicalDevice;


		return true;
	};

	void VulkanBase::getEnabledFeatures() {};

}