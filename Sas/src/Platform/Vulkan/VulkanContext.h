#pragma once
#include <vulkan\vulkan.h>
#include "Sas\Renderer\GraphicsContext.h"
#include "Sas/Renderer/Renderer.h"
#include "VulkanDevice.h"

namespace Sas {
	class VulkanContext : public GraphicsContext
	{
	public:
		VulkanContext();
		virtual ~VulkanContext();

		virtual void Init() override;
		
		Ref<VulkanDevice> GetDevice() { return m_Device; }

		static VkInstance GetInstance() { return s_VulkanInstance; }

		static Ref<VulkanContext> Get() { return Cast<VulkanContext>(Renderer::GetContext());}
		static Ref<VulkanDevice> GetCurrentDevice() { return Get()->GetDevice(); }
	private:
		// Devices
		Ref<VulkanPhysicalDevice> m_PhysicalDevice;
		Ref<VulkanDevice> m_Device;

		// Vulkan instance
		inline static VkInstance s_VulkanInstance;
		VkDebugReportCallbackEXT m_DebugReportCallback = VK_NULL_HANDLE;
		VkPipelineCache m_PipelineCache = nullptr;
	};
}