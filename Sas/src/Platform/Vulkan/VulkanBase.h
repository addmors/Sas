#pragma once
#include <vulkan\vulkan.h>
#include "VulkanTools.h"
#include "VulkanDebug.h"
#include "VulkanDevice.h"
#include "VulkanSwapChain.h"
#include "VulkanInitializers.hpp"

namespace Sas {

	class VulkanBase {
	private:
		bool viewUpdated = false;

	private:
		void initSwapchain();
		void setupSwapChain();
		void createCommandPool();
		void createCommandBuffers();
		void createSynchronizationPrimitives();
		void setupDepthStencil();
		void setupRenderPass();
		void createPipelineCache();
		void setupFrameBuffer();

		void nextFrame();

		/** @brief (Virtual) Called when the camera view has changed */
		virtual void viewChanged();


	public:
		VkResult createInstance(bool enableValidation);
		bool InitVulkan();
		void renderLoop();

		void render()
		{
			if (!prepared)
				return;
			renderFrame();
		}



		/** Prepare the next frame for workload submission by acquiring the next swap chain image */
		void prepareFrame();
		/** @brief Presents the current image to the swap chain */
		void submitFrame();
		/** @brief (Virtual) Default image acquire + submission and command buffer submission function */
		virtual void renderFrame();

		/** @brief Prepares all Vulkan resources and functions required to run the sample */
		virtual void prepare();

		/** @brief (Virtual) Called after the physical device features have been read, can be used to set features to enable on the device */
		virtual void getEnabledFeatures();

	protected:

		bool prepared = false;

		// Frame counter to display fps
		uint32_t frameCounter = 0;
		uint32_t lastFPS = 0;
		/** @brief Pipeline stages used to wait at for graphics queue submissions */
		VkPipelineStageFlags submitPipelineStages = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		// Contains command buffers and semaphores to be presented to the queue
		VkSubmitInfo submitInfo;
		// Command buffers used for rendering
		std::vector<VkCommandBuffer> drawCmdBuffers;
		// Global render pass for frame buffer writes
		VkRenderPass renderPass = VK_NULL_HANDLE;
		// List of available frame buffers (same as number of swap chain images)
		std::vector<VkFramebuffer>frameBuffers;
		// Active frame buffer index
		uint32_t currentBuffer = 0;
		// Pipeline cache object
		VkPipelineCache pipelineCache;
		// Wraps the swap chain to present images (framebuffers) to the windowing system
		VulkanSwapChain swapChain;
		// Synchronization semaphores
		struct {
			// Swap chain image presentation
			VkSemaphore presentComplete;
			// Command buffer submission and execution
			VkSemaphore renderComplete;
		} semaphores;
		std::vector<VkFence> waitFences;
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
		// Handle to the device graphics queue that command buffers are submitted to
		VkQueue queue;
		// Depth buffer format (selected during Vulkan initialization)
		VkFormat depthFormat;
		// Command buffer pool
		VkCommandPool cmdPool;
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
		struct {
			VkImage image;
			VkDeviceMemory mem;
			VkImageView view;
		} depthStencil;
	};
}