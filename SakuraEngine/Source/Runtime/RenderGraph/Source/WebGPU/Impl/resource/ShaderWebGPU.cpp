#include "RenderGraphWebGPU/RenderGraphWebGPU.h"
#include "System/Log.h"

#include "System/vfs/adapter.h"

namespace shaders::spirv
{
	RenderGraphAPI const sakura::uint32* get_shader(const sakura::string& name);
	RenderGraphAPI const sakura::uint32 get_shader_size(const sakura::string& name);
}

sakura::graphics::webgpu::GPUShader::GPUShader(
	const RenderShaderHandle handle, webgpu::RenderDevice& dev,
	const ShaderDesc& description) noexcept
	:_handle(handle), desc(description)
{
	// Find From Created Modules
	auto tryFindExisted = dev.shaderModules.find(description.name);
	if(tryFindExisted != dev.shaderModules.end())
	{
		module_ref = tryFindExisted->second;
	}
	else //No Existed Shader Module, Create One.
	{
		const uint32_t* code = nullptr;
		if(desc.code.empty())
		{
			assert(0 && "Only Support WithCode Mode!");
		}
		else
		{ 
			code = reinterpret_cast<const uint32*>(desc.code.data());
			size_ = desc.code.size() * sizeof(std::byte);
		}
		const char* label = nullptr;

		WGPUShaderModuleSPIRVDescriptor spirv = {};
		spirv.chain.sType = WGPUSType_ShaderModuleSPIRVDescriptor;
		spirv.codeSize = size_ / sizeof(uint32);
		spirv.code = code;
		WGPUShaderModuleDescriptor moduleDesc = {};
		moduleDesc.nextInChain = reinterpret_cast<WGPUChainedStruct*>(&spirv);
		moduleDesc.label = label;
		module_ref = wgpuDeviceCreateShaderModule(dev.device, &moduleDesc);
		if (!module_ref)
		{
			sakura::error("[RenderGraphWebGPU]: Failed to create shader {}", desc);
		}
		dev.shaderModules.insert_or_assign(description.name, module_ref);
	}
}

sakura::graphics::webgpu::GPUShader::~GPUShader()
{
	
}

sakura::graphics::RenderResourceHandle sakura::graphics::webgpu::GPUShader::handle() const
{
	return _handle;
}

size_t sakura::graphics::webgpu::GPUShader::size() const
{
	return size_;
}

sakura::graphics::EShaderFrequency sakura::graphics::webgpu::GPUShader::frequency() const
{
	return desc.frequency;
}

sakura::string_view sakura::graphics::webgpu::GPUShader::entry_name() const
{
	return desc.entry;
}

sakura::graphics::EShaderCodeFormat sakura::graphics::webgpu::GPUShader::code_format() const
{
	return EShaderCodeFormat::Spirv;
}
