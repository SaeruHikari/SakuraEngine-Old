#pragma once
#include "BufferVulkan.h"
#include "TextureVulkan.h"
#include "DeviceVulkan.h"
#include "FenceVulkan.h"
#include "ShaderVulkan.h"
#include "SamplerVulkan.h"
#include "SwapChainVulkan.h"
#include "RenderPipelineVulkan.h"

namespace sakura::graphics::vk
{
	VkPresentModeKHR translate(EPresentMode present_mode);

	EPresentMode translate(VkPresentModeKHR present_mode);

	VkFormat translate(ETextureFormat format);

	ETextureFormat translate(VkFormat format);

	VkShaderStageFlagBits translate(EShaderFrequency frequency);

	VkColorComponentFlags translate(ColorMask mask);

	VkBlendOp translate(EBlendOp blendOp);

	VkBlendFactor translate(EBlendFactor blendFactor);

	VkPolygonMode translate(EPolygonMode polygonMode);

	VkAttachmentLoadOp translate(ELoadOp op);

	VkAttachmentStoreOp translate(EStoreOp op);

	VkBufferUsageFlags translate(const BufferUsages usage, const EBufferCPUAccess access);

	VkPrimitiveTopology translate(EPrimitiveTopology topo);

	VkVertexInputRate translate(VertexLayout::Frequency freq);

	VkFormat translate(EVertexFormat format);

	VkDescriptorType translate(BindingLayout::EType type);

	VkSamplerCreateInfo translate(SamplerDescriptor desc);

	VkSamplerAddressMode translate(ESamplerAddressMode mode);
	
	VkFilter translate(EFilter mode);
	
	
	const bool bEnableValidationLayers = false;

	// cn: 启动引擎所需的最小扩展集.
	// en: The minimum set of extensions required to start the engine.
	// jp: エンジンを始動するために必要なデバイスエクステンションの最小セット.
	FORCEINLINE const std::vector<const char*> basic_extentions()
	{
		std::vector<const char*> res = {
#ifdef SAKURA_TARGET_PLATFORM_WIN
				VK_KHR_WIN32_SURFACE_EXTENSION_NAME,
#endif
				VK_KHR_SURFACE_EXTENSION_NAME
		};
		if (bEnableValidationLayers)
		{
			res.emplace_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
		}
		return res;
	}

	// cn: physics device所需的最小设备扩展集.
	// en: The minimum set of device extensions required by the physics device.
	// jp: 物理デバイスオブジェクトが必要とするデバイスエクステンションの最小セット.
	const std::vector<const char*> basic_device_exts =
	{

	};

	// cn: 主设备对象所需的最小设备扩展集.
	// en: The minimum set of device extensions required by the main device.
	// jp: メインデバイスオブジェクトが必要とするデバイスエクステンションの最小セット.
	const std::vector<const char*> main_device_exts =
	{
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};

	const std::vector<const char*> validationLayers = {
		"VK_LAYER_KHRONOS_validation"
	};
}

#include "RenderGraphVulkan/RenderGraphVulkan.inl"