#pragma once
#include "RenderGraph/RenderGraphCore.h"
#include "vulkan/vulkan_core.h"

namespace sakura::graphics::vk
{
	class RenderDevice;

	class RenderGraphVulkanAPI GPUShader : public IGPUShader
	{
	public:
		GPUShader(const RenderShaderHandle handle,
			vk::RenderDevice& dev, VkDevice device, const ShaderDesc& desc) noexcept;
		~GPUShader();
		RenderResourceHandle handle() const override;
		size_t size() const override;
		EShaderFrequency frequency() const override;

		sakura::string_view entry_name() const override;
		EShaderCodeFormat code_format() const override;

		RenderShaderHandle handle_;
		ShaderDesc desc_;
		size_t size_ = 0;

		VkShaderModule shader_module_ = VK_NULL_HANDLE;
		VkDevice owned_device = VK_NULL_HANDLE;
	};
}