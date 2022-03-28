#pragma once
#include <vulkan\vulkan.h>

namespace Sas {
	namespace vks {
		namespace initializers {
			
			inline VkCommandBufferAllocateInfo commandBufferAllocateInfo(
				VkCommandPool commandPool,
				VkCommandBufferLevel level,
				uint32_t bufferCount)
			{
				VkCommandBufferAllocateInfo commandBufferAllocateInfo{};
				commandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
				commandBufferAllocateInfo.commandPool = commandPool;
				commandBufferAllocateInfo.level = level;
				commandBufferAllocateInfo.commandBufferCount = bufferCount;
				return commandBufferAllocateInfo;
			}

			inline VkSubmitInfo submitInfo()
			{
				VkSubmitInfo submitInfo{};
				submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
				return submitInfo;
			}

			inline VkSemaphoreCreateInfo semaphoreCreateInfo()
			{
				VkSemaphoreCreateInfo semaphoreCreateInfo{};
				semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
				return semaphoreCreateInfo;
			}

			inline VkFenceCreateInfo fenceCreateInfo(VkFenceCreateFlags flags = 0)
			{
				VkFenceCreateInfo fenceCreateInfo{};
				fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
				fenceCreateInfo.flags = flags;
				return fenceCreateInfo;
			}
		}
	}
}