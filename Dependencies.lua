
-- Silent as Snake Dependencies

VULKAN_SDK = os.getenv("VULKAN_SDK")

IncludeDir = {}
IncludeDir["GLFW"] = "%{wks.location}/Sas/vendor/GLFW/include"
IncludeDir["Glad"] = "%{wks.location}/Sas/vendor/Glad/include"
IncludeDir["ImGui"] = "%{wks.location}/Sas/vendor/ImGui"
IncludeDir["glm"] = "%{wks.location}/Sas/vendor/glm"
IncludeDir["shaderc"] = "%{wks.location}/Sas/vendor/shaderc/include"
IncludeDir["SPIRV_Cross"] = "%{wks.location}/Sas/vendor/SPIRV-Cross"
IncludeDir["stb_image"] = "%{wks.location}/Sas/vendor/stb_image;"
IncludeDir["VulkanSDK"] = "%{VULKAN_SDK}/Include"

LibraryDir = {}

LibraryDir["VulkanSDK"] = "%{VULKAN_SDK}/Lib"
LibraryDir["VulkanSDK_Debug"] = "%{wks.location}/Sas/vendor/VulkanSDK/Lib"

Library = {}
Library["Vulkan"] = "%{LibraryDir.VulkanSDK}/vulkan-1.lib"
Library["VulkanUtils"] = "%{LibraryDir.VulkanSDK}/VkLayer_utils.lib"

Library["ShaderC_Debug"] = "%{LibraryDir.VulkanSDK_Debug}/shaderc_shared.lib"
Library["ShaderC_Utils_Debug"] = "%{LibraryDir.VulkanSDK_Debug}/shaderc_utild.lib"
Library["SPIRV_Cross_Debug"] = "%{LibraryDir.VulkanSDK_Debug}/spirv-cross-cored.lib"
Library["SPIRV_Cross_GLSL_Debug"] = "%{LibraryDir.VulkanSDK_Debug}/spirv-cross-glsld.lib"
Library["SPIRV_Tools_Debug"] = "%{LibraryDir.VulkanSDK_Debug}/SPIRV-Toolsd.lib"

Library["ShaderC_Release"] = "%{LibraryDir.VulkanSDK}/shaderc_shared.lib"
Library["ShaderC_Utils_Release"] = "%{LibraryDir.VulkanSDK}/shaderc_util.lib"
Library["SPIRV_Cross_Release"] = "%{LibraryDir.VulkanSDK}/spirv-cross-core.lib"
Library["SPIRV_Cross_GLSL_Release"] = "%{LibraryDir.VulkanSDK}/spirv-cross-glsl.lib"

