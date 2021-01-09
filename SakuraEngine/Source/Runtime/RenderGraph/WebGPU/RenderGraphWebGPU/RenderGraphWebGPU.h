#pragma once
#include <RenderGraphWebGPU/DeviceWebGPU.h>
#include <RenderGraphWebGPU/SwapChainWebGPU.h>
#include <RenderGraphWebGPU/ShaderWebGPU.h>
#include <RenderGraphWebGPU/FenceWebGPU.h>
#include <RenderGraphWebGPU/RenderPipelineWebGPU.h>
#include <RenderGraphWebGPU/BufferWebGPU.h>
#include <RenderGraphWebGPU/SamplerWebGPU.h>
#include <RenderGraphWebGPU/TextureWebGPU.h>
#include "RenderGraph/RenderGraph.h"

#include "System/Log.h"

namespace sakura::graphics::webgpu
{
	WGPUShaderStage translate(const EShaderFrequency frequency);

	WGPUBindingType translate(const BindingLayout::EType type);

	WGPUTextureFormat translate(const ETextureFormat format);
	
	ETextureFormat translate(const WGPUTextureFormat format);

	WGPUBlendFactor translate(const EBlendFactor factor);
	
	WGPUBlendOperation translate(const EBlendOp op);
	
	WGPUBlendDescriptor translate(const BlendSetting desc);

	WGPUCompareFunction translate(const ECompareFunction compare_function);
	
	WGPUColorWriteMaskFlags translate(const ColorMask mask);

	WGPUVertexFormat translate(const EVertexFormat vertex_format);

	WGPUIndexFormat translate(const EIndexFormat index_format);
	
	WGPUInputStepMode translate(const VertexLayout::Frequency vertex_format);
	
	WGPUPrimitiveTopology translate(const EPrimitiveTopology vertex_format);

	WGPUBufferUsageFlags translate(const BufferUsages usage, const EBufferCPUAccess access);

	WGPUTextureDescriptor translate(const TextureDescriptor& desc);

	WGPUTextureDimension translate(const ETextureDimension dimension);
	
	WGPULoadOp translate(const ELoadOp op);
	WGPUStoreOp translate(const EStoreOp op);

	WGPUPresentMode translate(const EPresentMode mode);

	WGPUTextureAspect translate(const ETextureAspect aspect);
	WGPUTextureCopyView translate(const TextureSlice from, const WGPUTexture texture);
	WGPUBufferCopyView translate(const TextureDataLayout layout, const WGPUBuffer buffer);

	WGPUSamplerDescriptor translate(const SamplerDescriptor& desc);
	WGPUFilterMode translate(const EFilter filter);
	WGPUAddressMode translate(const ESamplerAddressMode mode);
	
	WGPUTextureViewDimension match(const WGPUTextureDimension dimension);

	namespace detail
	{
		constexpr auto WebGPUTextureFormatNameLut = sakura::map_c<WGPUTextureFormat, sakura::string_view>(
			{
				{ WGPUTextureFormat_Undefined, "Undefined" },
				{ WGPUTextureFormat_R8Unorm, "R8Unorm" },
				{ WGPUTextureFormat_R8Snorm, "R8Snorm" },
				{ WGPUTextureFormat_R8Uint, "R8Uint" },
				{ WGPUTextureFormat_R8Sint, "R8Sint" },
				{ WGPUTextureFormat_R16Uint, "R16Uint" },
				{ WGPUTextureFormat_R16Sint, "R16Sint" },
				{ WGPUTextureFormat_R16Float, "R16Float" },
				{ WGPUTextureFormat_RG8Unorm, "RG8Unorm" },
				{ WGPUTextureFormat_RG8Snorm, "RG8Snorm" },
				{ WGPUTextureFormat_RG8Uint, "RG8Uint" },
				{ WGPUTextureFormat_RG8Sint, "RG8Sint" },
				{ WGPUTextureFormat_R32Float, "R32Float" },
				{ WGPUTextureFormat_R32Uint, "R32Uint" },
				{ WGPUTextureFormat_R32Sint, "R32Sint" },
				{ WGPUTextureFormat_RG16Uint, "RG16Uint" },
				{ WGPUTextureFormat_RG16Sint, "RG16Sint" },
				{ WGPUTextureFormat_RG16Float, "RG16Float" },
				{ WGPUTextureFormat_RGBA8Unorm, "RGBA8Unorm" },
				{ WGPUTextureFormat_RGBA8UnormSrgb, "RGBA8UnormSrgb" },
				{ WGPUTextureFormat_RGBA8Snorm, "RGBA8Snorm" },
				{ WGPUTextureFormat_RGBA8Uint, "RGBA8Uint" },
				{ WGPUTextureFormat_RGBA8Sint, "RGBA8Sint" },
				{ WGPUTextureFormat_BGRA8Unorm, "RGBA8Unorm" },
				{ WGPUTextureFormat_BGRA8UnormSrgb, "RGBA8UnormSrgb" },
				{ WGPUTextureFormat_RGB10A2Unorm, "RGBA10A2Unorm" },
				{ WGPUTextureFormat_RG11B10Ufloat, "RG11B10Ufloat" },
#ifndef SAKURA_TARGET_PLATFORM_EMSCRIPTEN
				{ WGPUTextureFormat_RGB9E5Ufloat, "RGB9E5Ufloat" },
				{ WGPUTextureFormat_BC6HRGBFloat, "BC6HRGBFloat" },
#endif
				{ WGPUTextureFormat_RG32Float, "RG32Float" },
				{ WGPUTextureFormat_RG32Uint, "RG32Uint" },
				{ WGPUTextureFormat_RG32Sint, "RG32Sint" },
				{ WGPUTextureFormat_RGBA16Uint, "RGBA16Uint" },
				{ WGPUTextureFormat_RGBA16Sint, "RGBA16Sint" },
				{ WGPUTextureFormat_RGBA16Float, "RGBA16Float" },
				{ WGPUTextureFormat_RGBA32Float, "RGBA32Float" },
				{ WGPUTextureFormat_RGBA32Uint, "RGBA32Uint" },
				{ WGPUTextureFormat_RGBA32Sint, "RGBA32Sint" },
				{ WGPUTextureFormat_Depth32Float, "Depth32Float" },
				{ WGPUTextureFormat_Depth24Plus, "Depth24Plus" },
				{ WGPUTextureFormat_Depth24PlusStencil8, "Depth24PlusStencil8" },
				{ WGPUTextureFormat_BC1RGBAUnorm, "BC1RGBAUnorm" },
				{ WGPUTextureFormat_BC1RGBAUnormSrgb, "BC1RGBAUnormSrgb" },
				{ WGPUTextureFormat_BC2RGBAUnorm, "BC2RGBAUnorm" },
				{ WGPUTextureFormat_BC2RGBAUnormSrgb, "BC2RGBAUnormSrgb" },
				{ WGPUTextureFormat_BC3RGBAUnorm, "BC3RGBAUnorm" },
				{ WGPUTextureFormat_BC3RGBAUnormSrgb, "BC3RGBAUnormSrgb" },
				{ WGPUTextureFormat_BC4RUnorm, "BC4RUnorm" },
				{ WGPUTextureFormat_BC4RSnorm, "BC4RSnorm" },
				{ WGPUTextureFormat_BC5RGUnorm, "BC5RGUnorm" },
				{ WGPUTextureFormat_BC5RGSnorm, "BC5RGSnorm" },
				{ WGPUTextureFormat_BC6HRGBUfloat, "BC6HRGBUfloat" },
				{ WGPUTextureFormat_BC6HRGBUfloat, "BC6HRGBSfloat" },
				{ WGPUTextureFormat_BC7RGBAUnorm, "BC7RGBAUnorm" },
				{ WGPUTextureFormat_BC7RGBAUnormSrgb, "BC7RGBAUnormSrgb" },
			}
		);

	}
}

namespace fmt
{
	// formatters
	template<> struct formatter<WGPUTextureFormat> : fmt::formatter<sakura::string_view>
	{
		template<typename FormatContext>
		auto format(WGPUTextureFormat fmt, FormatContext& ctx)
		{
			using namespace sakura::graphics::webgpu::detail;
			auto fmtName = WebGPUTextureFormatNameLut.find(fmt)->second;
			return fmt::formatter<sakura::string_view>::format(fmtName, ctx);
		}
	};
}

#include "RenderGraphWebGPU/RenderGraphWebGPU.inl"
/*
 
set_scissor_rect, // 
draw, //  
draw_indirect,

dispatch,
dispatch_indirect,

update_binding, //  

copy_buffer,
copy_texture,

barriers,
transitions,

begin_timing,
end_timing,
resolve_timings,

begin_event,
end_event,

begin_render_pass, // 
end_render_pass,   //  

ray_trace,
update_top_level,
update_bottom_level,
update_shader_table,

*/