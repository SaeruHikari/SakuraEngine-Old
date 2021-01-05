#include "RenderGraphWebGPU/RenderGraphWebGPU.h"
#include "System/Log.h"

#include "System/vfs/adapter.h"

namespace shaders::spirv
{
	RenderGraphAPI const sakura::uint32* get_shader(const sakura::string& name);
	RenderGraphAPI const sakura::uint32 get_shader_size(const sakura::string& name);
}

sakura::graphics::webgpu::GpuShader::GpuShader(
	const GpuShaderHandle handle, webgpu::RenderDevice& dev,
	const ShaderDescriptor& description) noexcept
	:_handle(handle), desc(description)
{
	// Find From Created Modules
	auto tryFindExisted = dev.shader_modules.find(description.name);
	if(tryFindExisted != dev.shader_modules.end())
	{
		module_ref = tryFindExisted->second;
	}
	else //No Existed Shader Module, Create One.
	{
		const char* src = nullptr;
		const uint32_t* code = nullptr;
		if(desc.code.empty())
		{
			if (desc.src)
			{
				src = desc.src;
			}
			else
			{
				assert(0 && "Only Support WithCode Mode!");
			}
		}
		else
		{ 
			code = reinterpret_cast<const uint32*>(desc.code.data());
			size_ = desc.code.size() * sizeof(std::byte);
		}
		const char* label = nullptr;


		WGPUShaderModuleDescriptor moduleDesc = {};
		WGPUShaderModuleSPIRVDescriptor spirv = {};
		WGPUShaderModuleWGSLDescriptor wgsl = {};
		if (desc.shader_language == EShaderLanguage::SPIRV)
		{
			spirv.chain.sType = WGPUSType_ShaderModuleSPIRVDescriptor;
			spirv.codeSize = size_ / sizeof(uint32);
			spirv.code = code;

			moduleDesc.nextInChain = reinterpret_cast<WGPUChainedStruct*>(&spirv);
			moduleDesc.label = label;
		}
		else if (desc.shader_language == EShaderLanguage::WGSL)
		{
			wgsl.chain.sType = WGPUSType_ShaderModuleWGSLDescriptor;
			wgsl.source = src;

			moduleDesc.nextInChain = reinterpret_cast<WGPUChainedStruct*>(&wgsl);
			moduleDesc.label = label;
		}
		module_ref = wgpuDeviceCreateShaderModule(dev.device, &moduleDesc);
		if (!module_ref)
		{
			sakura::error("[RenderGraphWebGPU]: Failed to create shader {}", desc);
		}
		dev.shader_modules.insert_or_assign(description.name, module_ref);
	}
}

sakura::graphics::webgpu::GpuShader::~GpuShader()
{
	
}

sakura::graphics::RenderResourceHandle sakura::graphics::webgpu::GpuShader::handle() const
{
	return _handle;
}

size_t sakura::graphics::webgpu::GpuShader::size() const
{
	return size_;
}

sakura::graphics::EShaderFrequency sakura::graphics::webgpu::GpuShader::frequency() const
{
	return desc.frequency;
}

sakura::string_view sakura::graphics::webgpu::GpuShader::entry_name() const
{
	return desc.entry;
}

sakura::graphics::EShaderCodeFormat sakura::graphics::webgpu::GpuShader::code_format() const
{
	return EShaderCodeFormat::Spirv;
}
