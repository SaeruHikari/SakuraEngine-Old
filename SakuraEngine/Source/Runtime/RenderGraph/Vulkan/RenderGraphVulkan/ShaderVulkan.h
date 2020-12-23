#pragma once
#include "RenderGraph/RenderGraphCore.h"
#include "vulkan/vulkan_core.h"

namespace sakura::graphics::vk
{
	class RenderDevice;

	class RenderGraphVulkanAPI GpuShader : public IGpuShader
	{
	public:
		GpuShader(const GpuShaderHandle handle,
			vk::RenderDevice& dev, VkDevice device, const ShaderDesc& desc) noexcept;
		~GpuShader();
		RenderResourceHandle handle() const override;
		size_t size() const override;
		EShaderFrequency frequency() const override;

		sakura::string_view entry_name() const override;
		EShaderCodeFormat code_format() const override;

		GpuShaderHandle handle_;
		ShaderDesc desc_;
		size_t size_ = 0;

		VkShaderModule shader_module_ = VK_NULL_HANDLE;
		VkDevice owned_device = VK_NULL_HANDLE;
	};
}