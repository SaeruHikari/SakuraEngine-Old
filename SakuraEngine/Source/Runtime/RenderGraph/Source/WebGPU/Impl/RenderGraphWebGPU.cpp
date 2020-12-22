#include <RenderGraphWebGPU/RenderGraphWebGPU.h>

RenderGraphWebGPUAPI WGPUShaderStage sakura::graphics::webgpu::translate(const EShaderFrequency frequency)
{
	switch (frequency)
	{
	case EShaderFrequency::VertexShader:
		return WGPUShaderStage::WGPUShaderStage_Vertex;
	case EShaderFrequency::PixelShader:
		return WGPUShaderStage_Fragment;
	case EShaderFrequency::ComputeShader:
		return WGPUShaderStage_Compute;
	default:
		sakura::warn("Shader Frequency {} Not Supported By WebGPU!", frequency);
		return WGPUShaderStage::WGPUShaderStage_None;
	}
}

RenderGraphWebGPUAPI WGPUBindingType sakura::graphics::webgpu::translate(const BindingLayout::EType type)
{
	switch (type)
	{
	case BindingLayout::EType::UniformBuffer:
		return WGPUBindingType_UniformBuffer;
	case BindingLayout::EType::StorageBuffer:
		return WGPUBindingType_StorageBuffer;
	case BindingLayout::EType::ReadonlyStorageBuffer:
		return WGPUBindingType_ReadonlyStorageBuffer;

	case BindingLayout::EType::Sampler:
		return WGPUBindingType_Sampler;
	case BindingLayout::EType::ComparisonSampler:
		return WGPUBindingType_ComparisonSampler;

	case BindingLayout::EType::SampledTexture:
		return WGPUBindingType_SampledTexture;
#ifndef SAKURA_TARGET_PLATFORM_EMSCRIPTEN
	case BindingLayout::EType::MultisampledTexture:
		return WGPUBindingType_MultisampledTexture;
#endif
	case BindingLayout::EType::ReadonlyStorageTexture:
		return WGPUBindingType_ReadonlyStorageTexture;
	case BindingLayout::EType::WriteonlyStorageTexture:
		return WGPUBindingType_WriteonlyStorageTexture;

	default:
		sakura::warn("Binding Type {} Not Supported By WebGPU!", type);
		return WGPUBindingType::WGPUBindingType_Force32;
	}
}

#ifdef trans_cases
#undef trans_cases
#endif
#define trans_cases \
	trans_case(R8_UNORM, WGPUTextureFormat_R8Unorm)\
	trans_case(R8_SNORM, WGPUTextureFormat_R8Snorm)\
	trans_case(R8_UINT, WGPUTextureFormat_R8Uint)\
	trans_case(R8_SINT, WGPUTextureFormat_R8Sint)\
	trans_case(R16_UINT, WGPUTextureFormat_R16Uint)\
	trans_case(R16_SINT, WGPUTextureFormat_R16Sint)\
	trans_case(R32_FLOAT, WGPUTextureFormat_R32Float)\
	trans_case(R32_UINT, WGPUTextureFormat_R32Uint)\
	trans_case(R32_SINT, WGPUTextureFormat_R32Sint)\
	trans_case(R16G16_UINT, WGPUTextureFormat_RG16Uint)\
	trans_case(R16G16_SINT, WGPUTextureFormat_RG16Sint)\
	trans_case(R16G16_FLOAT, WGPUTextureFormat_RG16Float)\
	trans_case(R8G8B8A8_UNORM, WGPUTextureFormat_RGBA8Unorm)\
	trans_case(R8G8B8A8_UNORM_SRGB, WGPUTextureFormat_RGBA8UnormSrgb)\
	trans_case(R8G8B8A8_SNORM, WGPUTextureFormat_RGBA8Snorm)\
	trans_case(R8G8B8A8_UINT, WGPUTextureFormat_RGBA8Uint)\
	trans_case(R8G8B8A8_SINT, WGPUTextureFormat_RGBA8Sint)\
	trans_case(B8G8R8A8_UNORM, WGPUTextureFormat_BGRA8Unorm)\
	trans_case(B8G8R8A8_UNORM_SRGB, WGPUTextureFormat_BGRA8UnormSrgb)\
	trans_case(R10G10B10A2_UNORM, WGPUTextureFormat_RGB10A2Unorm)\
	trans_case(R11G11B10_FLOAT, WGPUTextureFormat_RG11B10Ufloat)\
	trans_case(R32G32_FLOAT, WGPUTextureFormat_RG32Float)\
	trans_case(R32G32_UINT, WGPUTextureFormat_RG32Uint)\
	trans_case(R32G32_SINT, WGPUTextureFormat_RG32Sint)\
	trans_case(R16G16B16A16_UINT, WGPUTextureFormat_RGBA16Uint)\
	trans_case(R16G16B16A16_SINT, WGPUTextureFormat_RGBA16Sint)\
	trans_case(R16G16B16A16_FLOAT, WGPUTextureFormat_RGBA16Float)\
	trans_case(R32G32B32A32_FLOAT, WGPUTextureFormat_RGBA32Float)\
	trans_case(R32G32B32A32_UINT, WGPUTextureFormat_RGBA32Uint)\
	trans_case(R32G32B32A32_SINT, WGPUTextureFormat_RGBA32Sint)\
	trans_case(D32_FLOAT, WGPUTextureFormat_Depth32Float)\
	trans_case(D24_UNORM_S8_UINT, WGPUTextureFormat_Depth24PlusStencil8)\
	trans_case(BC1_UNORM, WGPUTextureFormat_BC1RGBAUnorm)\
	trans_case(BC1_UNORM_SRGB, WGPUTextureFormat_BC1RGBAUnormSrgb)\
	trans_case(BC2_UNORM, WGPUTextureFormat_BC2RGBAUnorm)\
	trans_case(BC2_UNORM_SRGB, WGPUTextureFormat_BC2RGBAUnormSrgb)\
	trans_case(BC3_UNORM, WGPUTextureFormat_BC3RGBAUnorm)\
	trans_case(BC3_UNORM_SRGB, WGPUTextureFormat_BC3RGBAUnormSrgb)\
	trans_case(BC4_UNORM, WGPUTextureFormat_BC4RUnorm)\
	trans_case(BC4_SNORM, WGPUTextureFormat_BC4RSnorm)\
	trans_case(BC5_UNORM, WGPUTextureFormat_BC5RGUnorm)\
	trans_case(BC5_SNORM, WGPUTextureFormat_BC5RGSnorm)\
	trans_case(BC6H_UF16, WGPUTextureFormat_BC6HRGBUfloat)\
	trans_case(BC7_UNORM, WGPUTextureFormat_BC7RGBAUnorm)\
	trans_case(BC7_UNORM_SRGB, WGPUTextureFormat_BC7RGBAUnormSrgb)\

#ifdef trans_cases_addon
#undef trans_cases_addon
#endif
#define trans_cases_addon \
	trans_case(R9G9B9E5_UFLOAT, WGPUTextureFormat_RGB9E5Ufloat)\
	trans_case(BC6H_SF16, WGPUTextureFormat_BC6HRGBFloat)

#define trans_case(_format, _format_translated) \
case ETextureFormat::_format:\
	return _format_translated;
RenderGraphWebGPUAPI WGPUTextureFormat sakura::graphics::webgpu::translate(const ETextureFormat format)
{
	switch (format)
	{
		trans_cases
#ifndef SAKURA_TARGET_PLATFORM_EMSCRIPTEN
		trans_cases_addon
#endif
	default:
		sakura::warn("TextureFormat {} Not Supported By WebGPU!", format);
		return WGPUTextureFormat_Undefined;
	}
}
#undef trans_case

#define trans_case(_format, _format_translated) \
case _format_translated:\
	return ETextureFormat::_format;
RenderGraphWebGPUAPI sakura::graphics::ETextureFormat sakura::graphics::webgpu::translate(const WGPUTextureFormat format)
{
	switch (format)
	{
		trans_cases
#ifndef SAKURA_TARGET_PLATFORM_EMSCRIPTEN
			trans_cases_addon
#endif
	default:
		sakura::error("WebGPU TextureFormat {} Not Supported By RenderGraph!", format);
		return ETextureFormat::Count;
	}
}
#undef trans_cases
#undef trans_cases_addon

RenderGraphWebGPUAPI WGPUBlendFactor sakura::graphics::webgpu::translate(const EBlendFactor factor)
{
	switch (factor)
	{
		case EBlendFactor::Zero: return WGPUBlendFactor_Zero;
		case EBlendFactor::One: return WGPUBlendFactor_One;
		case EBlendFactor::SrcColor: return WGPUBlendFactor_SrcColor;
		case EBlendFactor::OneMinusSrcColor: return WGPUBlendFactor_OneMinusSrcColor;
		case EBlendFactor::SrcAlpha: return WGPUBlendFactor_SrcAlpha;
		case EBlendFactor::OneMinusSrcAlpha: return WGPUBlendFactor_OneMinusSrcAlpha;
		case EBlendFactor::DstColor: return WGPUBlendFactor_DstColor;
		case EBlendFactor::OneMinusDstColor: return WGPUBlendFactor_OneMinusDstColor;
		case EBlendFactor::DstAlpha: return WGPUBlendFactor_DstAlpha;
		case EBlendFactor::OneMinusDstAlpha: return WGPUBlendFactor_OneMinusDstAlpha;
		case EBlendFactor::SrcAlphaSaturated: return WGPUBlendFactor_SrcAlphaSaturated;
		case EBlendFactor::BlendColor: return WGPUBlendFactor_BlendColor;
		case EBlendFactor::OneMinusBlendColor: return WGPUBlendFactor_OneMinusBlendColor;
		default:
			sakura::error("BlendFactor {} Not Supported By WebGPU!", factor);
			return WGPUBlendFactor_Zero;
	}
}

RenderGraphWebGPUAPI WGPUBlendOperation sakura::graphics::webgpu::translate(const EBlendOp op)
{
	switch (op)
	{
		case EBlendOp::Add: return WGPUBlendOperation_Add;
		case EBlendOp::Subtract: return WGPUBlendOperation_Subtract;
		case EBlendOp::ReverseSubtract: return WGPUBlendOperation_ReverseSubtract;
		case EBlendOp::Min: return WGPUBlendOperation_Min;
		case EBlendOp::Max: return WGPUBlendOperation_Max;
		default:
			sakura::error("BlendOp {} Not Supported By WebGPU!", op);
			return WGPUBlendOperation_Add;
	}
}

RenderGraphWebGPUAPI WGPUBlendDescriptor sakura::graphics::webgpu::translate(const BlendDesc desc)
{
	WGPUBlendDescriptor result;
	result.srcFactor = translate(desc.src_factor);
	result.dstFactor = translate(desc.dst_factor);
	result.operation = translate(desc.operation);
	return result;
}

RenderGraphWebGPUAPI WGPUColorWriteMaskFlags sakura::graphics::webgpu::translate(const ColorMask mask)
{
	WGPUColorWriteMaskFlags wgpuMask = WGPUColorWriteMask_None;
	if (mask & EColorMaskBits::R)
		wgpuMask |= WGPUColorWriteMask::WGPUColorWriteMask_Red;
	if (mask & EColorMaskBits::G)
		wgpuMask |= WGPUColorWriteMask::WGPUColorWriteMask_Green;	
	if (mask & EColorMaskBits::B)
		wgpuMask |= WGPUColorWriteMask::WGPUColorWriteMask_Blue;
	if (mask & EColorMaskBits::A)
		wgpuMask |= WGPUColorWriteMask::WGPUColorWriteMask_Alpha;
	return wgpuMask;
}

RenderGraphWebGPUAPI WGPUVertexFormat sakura::graphics::webgpu::translate(const EVertexFormat vertex_format)
{
	switch (vertex_format)
	{
		case EVertexFormat::UCHAR2: return WGPUVertexFormat_UChar2;
		case EVertexFormat::UCHAR4: return WGPUVertexFormat_UChar4;
		case EVertexFormat::CHAR2: return WGPUVertexFormat_Char2;
		case EVertexFormat::CHAR4: return WGPUVertexFormat_Char4;
		case EVertexFormat::UCHAR2_NORM: return WGPUVertexFormat_UChar2Norm;
		case EVertexFormat::UCHAR4_NORM: return WGPUVertexFormat_UChar4Norm;
		case EVertexFormat::CHAR2_NORM: return WGPUVertexFormat_Char2Norm;
		case EVertexFormat::CHAR4_NORM: return WGPUVertexFormat_Char4Norm;
		case EVertexFormat::USHORT2: return WGPUVertexFormat_UShort2;
		case EVertexFormat::USHORT4: return WGPUVertexFormat_UShort4;
		case EVertexFormat::USHORT2_NORM: return WGPUVertexFormat_UShort2Norm;
		case EVertexFormat::USHORT4_NORM: return WGPUVertexFormat_UShort4Norm;
		case EVertexFormat::HALF2: return WGPUVertexFormat_Half2;
		case EVertexFormat::HALF4: return WGPUVertexFormat_Half4;
		case EVertexFormat::FLOAT: return WGPUVertexFormat_Float;
		case EVertexFormat::FLOAT2: return WGPUVertexFormat_Float2;
		case EVertexFormat::FLOAT3: return WGPUVertexFormat_Float3;
		case EVertexFormat::FLOAT4: return WGPUVertexFormat_Float4;
		case EVertexFormat::UINT: return WGPUVertexFormat_UInt;
		case EVertexFormat::UINT2: return WGPUVertexFormat_UInt2;
		case EVertexFormat::UINT3: return WGPUVertexFormat_UInt3;
		case EVertexFormat::UINT4: return WGPUVertexFormat_UInt4;
		case EVertexFormat::INT: return WGPUVertexFormat_Int;
		case EVertexFormat::INT2: return WGPUVertexFormat_Int2;
		case EVertexFormat::INT3: return WGPUVertexFormat_Int3;
		case EVertexFormat::INT4: return WGPUVertexFormat_Int4;
		default:
			sakura::error("VertexFormat {} Not Supported By WebGPU!", vertex_format);
			return WGPUVertexFormat_Force32;
	}
}

WGPUIndexFormat sakura::graphics::webgpu::translate(const EIndexFormat index_format)
{
	if (index_format == EIndexFormat::UINT16)
		return WGPUIndexFormat_Uint16;
	if (index_format == EIndexFormat::UINT32)
		return WGPUIndexFormat_Uint32;
	return WGPUIndexFormat_Force32;
}

RenderGraphWebGPUAPI WGPUInputStepMode sakura::graphics::webgpu::translate(const VertexLayout::Frequency freq)
{
	switch (freq)
	{
		case VertexLayout::Frequency::PerInstanceData: return WGPUInputStepMode_Instance;
		case VertexLayout::Frequency::PerVertexData: return WGPUInputStepMode_Vertex;
		default:
			sakura::error("VertexLayout::Frequency(InputStepMode) {} Not Supported By WebGPU!", freq);
			return WGPUInputStepMode_Force32;
	}
}

RenderGraphWebGPUAPI WGPUPrimitiveTopology sakura::graphics::webgpu::translate(const EPrimitiveTopology topo)
{
	switch (topo)
	{
		case EPrimitiveTopology::TriangleList: return WGPUPrimitiveTopology_TriangleList;
		case EPrimitiveTopology::LineList: return WGPUPrimitiveTopology_LineList;
		case EPrimitiveTopology::LineStrip: return WGPUPrimitiveTopology_LineStrip;
		case EPrimitiveTopology::PointList: return WGPUPrimitiveTopology_PointList;
		case EPrimitiveTopology::TriangleStrip: return WGPUPrimitiveTopology_TriangleStrip;
		default:
			sakura::error("PrimitiveTopology {} Not Supported By WebGPU!", topo);
			return WGPUPrimitiveTopology_Force32;
	}
}

WGPUBufferUsageFlags sakura::graphics::webgpu::translate(const BufferUsages usage, const EBufferCPUAccess access)
{
	using namespace sakura::graphics;
	WGPUBufferUsageFlags usages = WGPUBufferUsage_None;
	if (usage & EBufferUsage::IndexBuffer)
	{
		usages |= WGPUBufferUsage_Index;
	}
	if (usage & EBufferUsage::VertexBuffer)
	{
		usages |= WGPUBufferUsage_Vertex;
	}
	if (usage & EBufferUsage::UniformBuffer)
	{
		usages |= WGPUBufferUsage_Uniform;
	}
	if (usage & EBufferUsage::IndirectBuffer)
	{
		usages |= WGPUBufferUsage_Indirect;
	}
	if (usage & EBufferUsage::StorageBuffer)
	{
		usages |= WGPUBufferUsage_Storage;
	}
	if (usage & EBufferUsage::RayTracingAccelerateStructure)
	{
		
	}
	if (usage & EBufferUsage::CopySrc)
	{
		usages |= WGPUBufferUsage_CopySrc;
	}
	if (usage & EBufferUsage::CopyDst)
	{
		usages |= WGPUBufferUsage_CopyDst;
	}
	if (usage & EBufferUsage::Query)
	{
		usages |= WGPUBufferUsage_QueryResolve;
	}
	
	switch (access)
	{
	case EBufferCPUAccess::None:
		usages = usages | WGPUBufferUsage_None; break;
	case EBufferCPUAccess::Read:
		usages = usages | WGPUBufferUsage_MapRead; break;
	case EBufferCPUAccess::Write:
		usages = usages | WGPUBufferUsage_MapWrite; break;
	case EBufferCPUAccess::ReadWrite:
		usages = usages | WGPUBufferUsage_MapWrite | WGPUBufferUsage_MapRead; break;
	}
	return usages;
}

WGPUTextureDescriptor sakura::graphics::webgpu::translate(const TextureDesc& desc)
{
	WGPUTextureDescriptor res = {};
	res.size = { desc.size.width, desc.size.height, desc.size.depth };
	res.usage = WGPUTextureUsage::WGPUTextureUsage_None;
	if (desc.usages & ETextureUsage::Attachment)
		res.usage |= WGPUTextureUsage::WGPUTextureUsage_OutputAttachment;
	if (desc.usages & ETextureUsage::CopyDst)
		res.usage |= WGPUTextureUsage::WGPUTextureUsage_CopyDst;
	if (desc.usages & ETextureUsage::CopySrc)
		res.usage |= WGPUTextureUsage::WGPUTextureUsage_CopySrc;
	if (desc.usages & ETextureUsage::Present)
		res.usage |= WGPUTextureUsage::WGPUTextureUsage_Present;
	if (desc.usages & ETextureUsage::Sampled)
		res.usage |= WGPUTextureUsage::WGPUTextureUsage_Sampled;
	if (desc.usages & ETextureUsage::Storage)
		res.usage |= WGPUTextureUsage::WGPUTextureUsage_Storage;

	res.dimension = translate(desc.dimension);
	res.format = translate(desc.format);
	res.mipLevelCount = desc.mip_levels;
	res.sampleCount = desc.sample_count;
	return res;
}

WGPUTextureDimension sakura::graphics::webgpu::translate(const ETextureDimension dimension)
{
	switch (dimension)
	{
	case ETextureDimension::Texture1D:
		return WGPUTextureDimension_1D;
	case ETextureDimension::Texture2D:
		return WGPUTextureDimension_2D;
	case ETextureDimension::Texture3D:
		return WGPUTextureDimension_3D;
	default:
		return WGPUTextureDimension_2D;
	}
}

WGPULoadOp sakura::graphics::webgpu::translate(const ELoadOp op)
{
	switch (op)
	{
	case ELoadOp::Clear: return WGPULoadOp_Clear;
	case ELoadOp::Load: return WGPULoadOp_Load;
	default:
		sakura::error("ELoadOp {} Not Supported By WebGPU!", op);
		return WGPULoadOp_Force32;
	}
}

WGPUStoreOp sakura::graphics::webgpu::translate(const EStoreOp op)
{
	switch (op)
	{
	case EStoreOp::Clear: return WGPUStoreOp_Clear;
	case EStoreOp::Store: return WGPUStoreOp_Store;
	default:
		sakura::error("EStoreOp {} Not Supported By WebGPU!", op);
		return WGPUStoreOp_Force32;
	}
}

WGPUPresentMode sakura::graphics::webgpu::translate(const EPresentMode mode)
{
	switch (mode)
	{
	case EPresentMode::Mailbox:
		return WGPUPresentMode_Mailbox;
	case EPresentMode::FIFO:
		return WGPUPresentMode_Fifo;
	case EPresentMode::Immediate:
		return WGPUPresentMode_Immediate;
	default:
		return WGPUPresentMode_Mailbox;
	}
}

WGPUTextureAspect sakura::graphics::webgpu::translate(const ETextureAspect aspect)
{
	switch (aspect)
	{
	case ETextureAspect::All:
		return WGPUTextureAspect::WGPUTextureAspect_All;
	case ETextureAspect::DepthOnly:
		return WGPUTextureAspect_DepthOnly;
	case ETextureAspect::StencilOnly:
		return WGPUTextureAspect_StencilOnly;
	default:
		return WGPUTextureAspect_Force32;
	}
}

WGPUTextureCopyView sakura::graphics::webgpu::translate(const TextureSlice from, const WGPUTexture texture)
{
	WGPUTextureCopyView copyView = {};
	copyView.texture = texture;
	copyView.mipLevel = from.mip_level;
	copyView.origin = { from.origin.x, from.origin.y, from.origin.z };
	copyView.aspect = translate(from.aspect);
	return copyView;
}

WGPUBufferCopyView sakura::graphics::webgpu::translate(const TextureDataLayout layout, const WGPUBuffer buffer)
{
	WGPUBufferCopyView copyView = {};
	copyView.layout.bytesPerRow = layout.bytes_per_raw;
	copyView.layout.rowsPerImage = layout.rows_per_image;
	copyView.layout.offset = layout.offset;
	copyView.buffer = buffer;
	return copyView;
}



