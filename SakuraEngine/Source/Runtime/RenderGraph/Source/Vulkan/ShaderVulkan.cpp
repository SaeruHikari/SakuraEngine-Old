#include "RenderGraphVulkan/RenderGraphVulkan.h"

using namespace sakura::graphics::vk;
using namespace sakura::graphics;

sakura::graphics::vk::GpuShader::GpuShader(
	const GpuShaderHandle handle, vk::RenderDevice& render_device,
	VkDevice device,
	const ShaderDescriptor& description) noexcept
	:handle_(handle), desc_(description)
{
	VkShaderModuleCreateInfo createInfo = {};
	if (description.code.empty())
	{
		assert(0 && "Only Support WithCode Mode!");
	}
	else
	{
		size_ = description.code.size() * sizeof(std::byte) / sizeof(uint32);
		createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		createInfo.codeSize = desc_.code.size();
		createInfo.pCode = reinterpret_cast<const uint32_t*>(desc_.code.data());
	}
	if (vkCreateShaderModule(device, &createInfo, nullptr, &shader_module_) != VK_SUCCESS) 
	{
		sakura::error("[ShaderVulkan]: failed to create shader module!");
	}
	owned_device = device;

	VkPipelineShaderStageCreateInfo shaderStageInfo{};
	shaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	shaderStageInfo.stage = translate(desc_.frequency);
	shaderStageInfo.module = shader_module_;
	shaderStageInfo.pName = desc_.entry.c_str();
}

sakura::graphics::vk::GpuShader::~GpuShader()
{
	vkDestroyShaderModule(owned_device, shader_module_, nullptr);
}

sakura::graphics::RenderResourceHandle sakura::graphics::vk::GpuShader::handle() const
{
	return handle_;
}

size_t sakura::graphics::vk::GpuShader::size() const
{
	return size_;
}

sakura::graphics::EShaderFrequency sakura::graphics::vk::GpuShader::frequency() const
{
	return desc_.frequency;
}

sakura::string_view sakura::graphics::vk::GpuShader::entry_name() const
{
	return desc_.entry;
}

sakura::graphics::EShaderCodeFormat sakura::graphics::vk::GpuShader::code_format() const
{
	return EShaderCodeFormat::Spirv;
}

