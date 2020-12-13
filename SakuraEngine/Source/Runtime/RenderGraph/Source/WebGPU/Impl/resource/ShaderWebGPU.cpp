#include "RenderGraphWebGPU/RenderGraphWebGPU.h"
#include "system/Log.h"

#include "system/vfs/adapter.h"

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
	sakura::vector<uint32> readed;
	if(tryFindExisted != dev.shaderModules.end())
	{
		module_ref = tryFindExisted->second;
	}
	else //No Existed Shader Module, Create One.
	{
		const uint32_t* code = nullptr;
		uint32_t size = 0;
		if(desc.builtIn)
		{

		}
		else
		{ 
			sakura::vfs::adapter* adpt_local_dev = sakura::vfs::adapter::mounted_adapters[u8"Project:"];
			sakura::vfs::path shader_p(
				desc.name == "triangle_vert" ? u8"/Resources/Shaders/spirv/Triangle.vs.spv" : u8"/Resources/Shaders/spirv/Triangle.ps.spv");
			auto shader_entry = sakura::unique_ptr<vfs::entry>(adpt_local_dev->entry(shader_p));
			auto shader_file = sakura::unique_ptr<vfs::file>(shader_entry->open_as_file("r"));
			shader_file->seek(0, vfs::VFS_SEEK_END);
			readed.resize(shader_file->tell());
			shader_file->seek(0, vfs::VFS_SEEK_SET);
			shader_file->read(readed.data(), sizeof(uint32), readed.size());
			code = readed.data();
			size = readed.size();
		}
		const char* label = nullptr;

		WGPUShaderModuleSPIRVDescriptor spirv = {};
		spirv.chain.sType = WGPUSType_ShaderModuleSPIRVDescriptor;
		spirv.codeSize = size / sizeof(uint32_t);
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
	return 0;
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
