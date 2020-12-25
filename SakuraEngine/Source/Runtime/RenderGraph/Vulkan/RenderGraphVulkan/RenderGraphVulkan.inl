namespace sakura::graphics
{
	FORCEINLINE VkPresentModeKHR sakura::graphics::vk::translate(EPresentMode present_mode)
	{
		switch (present_mode)
		{
		case sakura::graphics::EPresentMode::Immediate:
			return VkPresentModeKHR::VK_PRESENT_MODE_IMMEDIATE_KHR;
		case sakura::graphics::EPresentMode::FIFO:
			return VkPresentModeKHR::VK_PRESENT_MODE_FIFO_RELAXED_KHR;
		case sakura::graphics::EPresentMode::Mailbox:
			return VkPresentModeKHR::VK_PRESENT_MODE_MAILBOX_KHR;
		case sakura::graphics::EPresentMode::Count:
		default:
			return VkPresentModeKHR::VK_PRESENT_MODE_MAX_ENUM_KHR;
		}
	}

	FORCEINLINE EPresentMode sakura::graphics::vk::translate(VkPresentModeKHR present_mode)
	{
		switch (present_mode)
		{
		case VK_PRESENT_MODE_IMMEDIATE_KHR:
			return sakura::graphics::EPresentMode::Immediate;
		case VK_PRESENT_MODE_MAILBOX_KHR:
			return sakura::graphics::EPresentMode::Mailbox;
		case VK_PRESENT_MODE_FIFO_KHR:
		case VK_PRESENT_MODE_FIFO_RELAXED_KHR:
			return sakura::graphics::EPresentMode::FIFO;
		case VK_PRESENT_MODE_SHARED_DEMAND_REFRESH_KHR:
		case VK_PRESENT_MODE_SHARED_CONTINUOUS_REFRESH_KHR:
		case VK_PRESENT_MODE_MAX_ENUM_KHR:
		default:
			return sakura::graphics::EPresentMode::Count;
		}
	}

#define trans_case2(_format, _vkformat) \
case ETextureFormat::_format:\
	return VkFormat::VK_FORMAT_##_vkformat

#define trans_case(_format) \
case ETextureFormat::_format:\
	return VkFormat::VK_FORMAT_##_format

	FORCEINLINE VkFormat sakura::graphics::vk::translate(ETextureFormat format)
	{
		switch (format)
		{
			trans_case(R32G32B32A32_UINT);
			trans_case(R32G32B32A32_SINT);
			trans_case2(R32G32B32A32_FLOAT, R32G32B32A32_SFLOAT);
			trans_case(R32G32B32_UINT);
			trans_case(R32G32B32_SINT);
			trans_case2(R32G32B32_FLOAT, R32G32B32_SFLOAT);
			trans_case(R16G16B16A16_UNORM);
			trans_case(R16G16B16A16_SNORM);
			trans_case(R16G16B16A16_UINT);
			trans_case(R16G16B16A16_SINT);
			trans_case2(R16G16B16A16_FLOAT, R16G16B16A16_SFLOAT);
			trans_case(R32G32_UINT);
			trans_case(R32G32_SINT);
			trans_case2(R32G32_FLOAT, R32G32_SFLOAT);
			trans_case2(R10G10B10A2_UNORM, A2R10G10B10_UNORM_PACK32);
			trans_case2(R10G10B10A2_UINT, A2R10G10B10_UINT_PACK32);
			trans_case2(R9G9B9E5_UFLOAT, E5B9G9R9_UFLOAT_PACK32);
			trans_case(R8G8B8A8_UNORM);
			trans_case(R8G8B8A8_SNORM);
			trans_case(R8G8B8A8_UINT);
			trans_case(R8G8B8A8_SINT);
			trans_case2(R8G8B8A8_UNORM_SRGB, ETC2_R8G8B8A8_SRGB_BLOCK);
			trans_case(B8G8R8A8_UNORM);
			trans_case2(B8G8R8A8_UNORM_SRGB, B8G8R8A8_SRGB);
			trans_case2(R11G11B10_FLOAT, B10G11R11_UFLOAT_PACK32);
			trans_case(R16G16_UNORM);
			trans_case(R16G16_SNORM);
			trans_case(R16G16_UINT);
			trans_case(R16G16_SINT);
			trans_case2(R16G16_FLOAT, R16G16_SFLOAT);
			trans_case(R32_UINT);
			trans_case(R32_SINT);
			trans_case2(R32_FLOAT, R32_SFLOAT);
			trans_case2(B5G5R5A1_UNORM, B5G5R5A1_UNORM_PACK16);
			trans_case2(B5G6R5_UNORM, B5G6R5_UNORM_PACK16);
			trans_case(R8G8_UNORM);
			trans_case(R8G8_SNORM);
			trans_case(R8G8_UINT);
			trans_case(R8G8_SINT);
			trans_case(R16_UNORM);
			trans_case(R16_SNORM);
			trans_case(R16_UINT);
			trans_case(R16_SINT);
			trans_case2(R16_FLOAT, R16_SFLOAT);
			trans_case(R8_UNORM);
			trans_case(R8_SNORM);
			trans_case(R8_UINT);
			trans_case(R8_SINT);
			trans_case(D24_UNORM_S8_UINT);
			trans_case2(D32_FLOAT, D32_SFLOAT);
			trans_case(D16_UNORM);
			trans_case2(BC1_UNORM, BC1_RGB_UNORM_BLOCK);
			trans_case2(BC1_UNORM_SRGB, BC1_RGB_SRGB_BLOCK);
			trans_case2(BC2_UNORM, BC2_UNORM_BLOCK);
			trans_case2(BC2_UNORM_SRGB, BC2_SRGB_BLOCK);
			trans_case2(BC3_UNORM, BC3_UNORM_BLOCK);
			trans_case2(BC3_UNORM_SRGB, BC3_SRGB_BLOCK);
			trans_case2(BC4_UNORM, BC4_UNORM_BLOCK);
			trans_case2(BC4_SNORM, BC4_SNORM_BLOCK);
			trans_case2(BC5_UNORM, BC5_UNORM_BLOCK);
			trans_case2(BC5_SNORM, BC5_SNORM_BLOCK);
			trans_case2(BC6H_UF16, BC6H_UFLOAT_BLOCK);
			trans_case2(BC6H_SF16, BC6H_SFLOAT_BLOCK);
			trans_case2(BC7_UNORM, BC7_UNORM_BLOCK);
			trans_case2(BC7_UNORM_SRGB, BC7_SRGB_BLOCK);
		default:
			return VkFormat::VK_FORMAT_UNDEFINED;
		}
		return VkFormat::VK_FORMAT_UNDEFINED;
	}

#undef trans_case
#undef trans_case2


#define trans_case2(_format, _vkformat) \
case VkFormat::VK_FORMAT_##_vkformat:\
	return ETextureFormat::_format

#define trans_case(_format) \
case VkFormat::VK_FORMAT_##_format:\
	return ETextureFormat::_format

	FORCEINLINE ETextureFormat sakura::graphics::vk::translate(VkFormat format)
	{
		switch (format)
		{
			trans_case(R32G32B32A32_UINT);
			trans_case(R32G32B32A32_SINT);
			trans_case2(R32G32B32A32_FLOAT, R32G32B32A32_SFLOAT);
			trans_case(R32G32B32_UINT);
			trans_case(R32G32B32_SINT);
			trans_case2(R32G32B32_FLOAT, R32G32B32_SFLOAT);
			trans_case(R16G16B16A16_UNORM);
			trans_case(R16G16B16A16_SNORM);
			trans_case(R16G16B16A16_UINT);
			trans_case(R16G16B16A16_SINT);
			trans_case2(R16G16B16A16_FLOAT, R16G16B16A16_SFLOAT);
			trans_case(R32G32_UINT);
			trans_case(R32G32_SINT);
			trans_case2(R32G32_FLOAT, R32G32_SFLOAT);
			trans_case2(R10G10B10A2_UNORM, A2R10G10B10_UNORM_PACK32);
			trans_case2(R10G10B10A2_UINT, A2R10G10B10_UINT_PACK32);
			trans_case2(R9G9B9E5_UFLOAT, E5B9G9R9_UFLOAT_PACK32);
			trans_case(R8G8B8A8_UNORM);
			trans_case(R8G8B8A8_SNORM);
			trans_case(R8G8B8A8_UINT);
			trans_case(R8G8B8A8_SINT);
			trans_case2(R8G8B8A8_UNORM_SRGB, ETC2_R8G8B8A8_SRGB_BLOCK);
			trans_case(B8G8R8A8_UNORM);
			trans_case2(B8G8R8A8_UNORM_SRGB, B8G8R8A8_SRGB);
			trans_case2(R11G11B10_FLOAT, B10G11R11_UFLOAT_PACK32);
			trans_case(R16G16_UNORM);
			trans_case(R16G16_SNORM);
			trans_case(R16G16_UINT);
			trans_case(R16G16_SINT);
			trans_case2(R16G16_FLOAT, R16G16_SFLOAT);
			trans_case(R32_UINT);
			trans_case(R32_SINT);
			trans_case2(R32_FLOAT, R32_SFLOAT);
			trans_case2(B5G5R5A1_UNORM, B5G5R5A1_UNORM_PACK16);
			trans_case2(B5G6R5_UNORM, B5G6R5_UNORM_PACK16);
			trans_case(R8G8_UNORM);
			trans_case(R8G8_SNORM);
			trans_case(R8G8_UINT);
			trans_case(R8G8_SINT);
			trans_case(R16_UNORM);
			trans_case(R16_SNORM);
			trans_case(R16_UINT);
			trans_case(R16_SINT);
			trans_case2(R16_FLOAT, R16_SFLOAT);
			trans_case(R8_UNORM);
			trans_case(R8_SNORM);
			trans_case(R8_UINT);
			trans_case(R8_SINT);
			trans_case(D24_UNORM_S8_UINT);
			trans_case2(D32_FLOAT, D32_SFLOAT);
			trans_case(D16_UNORM);
			trans_case2(BC1_UNORM, BC1_RGB_UNORM_BLOCK);
			trans_case2(BC1_UNORM_SRGB, BC1_RGB_SRGB_BLOCK);
			trans_case2(BC2_UNORM, BC2_UNORM_BLOCK);
			trans_case2(BC2_UNORM_SRGB, BC2_SRGB_BLOCK);
			trans_case2(BC3_UNORM, BC3_UNORM_BLOCK);
			trans_case2(BC3_UNORM_SRGB, BC3_SRGB_BLOCK);
			trans_case2(BC4_UNORM, BC4_UNORM_BLOCK);
			trans_case2(BC4_SNORM, BC4_SNORM_BLOCK);
			trans_case2(BC5_UNORM, BC5_UNORM_BLOCK);
			trans_case2(BC5_SNORM, BC5_SNORM_BLOCK);
			trans_case2(BC6H_UF16, BC6H_UFLOAT_BLOCK);
			trans_case2(BC6H_SF16, BC6H_SFLOAT_BLOCK);
			trans_case2(BC7_UNORM, BC7_UNORM_BLOCK);
			trans_case2(BC7_UNORM_SRGB, BC7_SRGB_BLOCK);
		default:
			return ETextureFormat::Count;
		}
		return ETextureFormat::Count;
	}
#undef trans_case
#undef trans_case2

	FORCEINLINE VkShaderStageFlagBits sakura::graphics::vk::translate(EShaderFrequency frequency)
	{
		switch (frequency)
		{
		case sakura::graphics::EShaderFrequency::VertexShader:
			return VkShaderStageFlagBits::VK_SHADER_STAGE_VERTEX_BIT;
		case sakura::graphics::EShaderFrequency::PixelShader:
			return VkShaderStageFlagBits::VK_SHADER_STAGE_FRAGMENT_BIT;
		case sakura::graphics::EShaderFrequency::ComputeShader:
			return VkShaderStageFlagBits::VK_SHADER_STAGE_COMPUTE_BIT;

		case sakura::graphics::EShaderFrequency::AnyHitShader:
			return VkShaderStageFlagBits::VK_SHADER_STAGE_ANY_HIT_BIT_KHR;
		case sakura::graphics::EShaderFrequency::CallableShader:
			return VkShaderStageFlagBits::VK_SHADER_STAGE_CALLABLE_BIT_KHR;
		case sakura::graphics::EShaderFrequency::ClosestHitShader:
			return VkShaderStageFlagBits::VK_SHADER_STAGE_CLOSEST_HIT_BIT_KHR;
		case sakura::graphics::EShaderFrequency::IntersectionShader:
			return VkShaderStageFlagBits::VK_SHADER_STAGE_INTERSECTION_BIT_KHR;
		case sakura::graphics::EShaderFrequency::MissShader:
			return VkShaderStageFlagBits::VK_SHADER_STAGE_MISS_BIT_KHR;
		case sakura::graphics::EShaderFrequency::RayGenerationShader:
			return VkShaderStageFlagBits::VK_SHADER_STAGE_RAYGEN_BIT_KHR;

			// As Nvdia Extensions.
		case sakura::graphics::EShaderFrequency::TaskShader:
			return VkShaderStageFlagBits::VK_SHADER_STAGE_TASK_BIT_NV;
		case sakura::graphics::EShaderFrequency::MeshShader:
			return VkShaderStageFlagBits::VK_SHADER_STAGE_MESH_BIT_NV;
		case sakura::graphics::EShaderFrequency::Count:
		case sakura::graphics::EShaderFrequency::None:
		default:
			return VkShaderStageFlagBits::VK_SHADER_STAGE_FLAG_BITS_MAX_ENUM;
		}
	}

	FORCEINLINE VkColorComponentFlags sakura::graphics::vk::translate(ColorMask mask)
	{
		VkColorComponentFlags wgpuMask = 0;
		if (mask & EColorMaskBits::R)
			wgpuMask |= VkColorComponentFlagBits::VK_COLOR_COMPONENT_R_BIT;
		if (mask & EColorMaskBits::G)
			wgpuMask |= VkColorComponentFlagBits::VK_COLOR_COMPONENT_G_BIT;
		if (mask & EColorMaskBits::B)
			wgpuMask |= VkColorComponentFlagBits::VK_COLOR_COMPONENT_B_BIT;
		if (mask & EColorMaskBits::A)
			wgpuMask |= VkColorComponentFlagBits::VK_COLOR_COMPONENT_A_BIT;
		return wgpuMask;
	}

	FORCEINLINE VkBlendOp sakura::graphics::vk::translate(EBlendOp blendOp)
	{
		switch (blendOp)
		{
		case sakura::graphics::EBlendOp::Add:
			return VkBlendOp::VK_BLEND_OP_ADD;
		case sakura::graphics::EBlendOp::Subtract:
			return VkBlendOp::VK_BLEND_OP_SUBTRACT;
		case sakura::graphics::EBlendOp::ReverseSubtract:
			return VkBlendOp::VK_BLEND_OP_REVERSE_SUBTRACT;
		case sakura::graphics::EBlendOp::Min:
			return VkBlendOp::VK_BLEND_OP_MIN;
		case sakura::graphics::EBlendOp::Max:
			return VkBlendOp::VK_BLEND_OP_MAX;
		default:
			return VkBlendOp::VK_BLEND_OP_MAX_ENUM;
		}
	}

	FORCEINLINE VkBlendFactor sakura::graphics::vk::translate(EBlendFactor blendFactor)
	{
		switch (blendFactor)
		{
		case sakura::graphics::EBlendFactor::Zero:
			return VkBlendFactor::VK_BLEND_FACTOR_ZERO;
		case sakura::graphics::EBlendFactor::One:
			return VkBlendFactor::VK_BLEND_FACTOR_ONE;
		case sakura::graphics::EBlendFactor::SrcColor:
			return VkBlendFactor::VK_BLEND_FACTOR_SRC_COLOR;
		case sakura::graphics::EBlendFactor::OneMinusSrcColor:
			return VkBlendFactor::VK_BLEND_FACTOR_ONE_MINUS_SRC_COLOR;
		case sakura::graphics::EBlendFactor::SrcAlpha:
			return VkBlendFactor::VK_BLEND_FACTOR_SRC_ALPHA;
		case sakura::graphics::EBlendFactor::OneMinusSrcAlpha:
			return VkBlendFactor::VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
		case sakura::graphics::EBlendFactor::DstColor:
			return VkBlendFactor::VK_BLEND_FACTOR_DST_COLOR;
		case sakura::graphics::EBlendFactor::OneMinusDstColor:
			return VkBlendFactor::VK_BLEND_FACTOR_ONE_MINUS_DST_COLOR;
		case sakura::graphics::EBlendFactor::DstAlpha:
			return VkBlendFactor::VK_BLEND_FACTOR_DST_ALPHA;
		case sakura::graphics::EBlendFactor::OneMinusDstAlpha:
			return VkBlendFactor::VK_BLEND_FACTOR_ONE_MINUS_DST_ALPHA;
		case sakura::graphics::EBlendFactor::SrcAlphaSaturated:
			return VkBlendFactor::VK_BLEND_FACTOR_SRC_ALPHA_SATURATE;
		case sakura::graphics::EBlendFactor::BlendColor:
			return VkBlendFactor::VK_BLEND_FACTOR_CONSTANT_COLOR;
		case sakura::graphics::EBlendFactor::OneMinusBlendColor:
			return VkBlendFactor::VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_COLOR;
		default:
			return VkBlendFactor::VK_BLEND_FACTOR_MAX_ENUM;
		}
	}

	FORCEINLINE VkPolygonMode sakura::graphics::vk::translate(EPolygonMode polygonMode)
	{
		switch (polygonMode)
		{
		case sakura::graphics::EPolygonMode::FILL:
			return VkPolygonMode::VK_POLYGON_MODE_FILL;
		case sakura::graphics::EPolygonMode::LINE:
			return VkPolygonMode::VK_POLYGON_MODE_LINE;
		case sakura::graphics::EPolygonMode::POINT:
			return VkPolygonMode::VK_POLYGON_MODE_POINT;
		default:
			return VkPolygonMode::VK_POLYGON_MODE_FILL;
		}
		return VkPolygonMode::VK_POLYGON_MODE_FILL;
	}

	FORCEINLINE VkAttachmentLoadOp sakura::graphics::vk::translate(ELoadOp op)
	{
		switch (op)
		{
		case sakura::graphics::ELoadOp::Clear:
			return VkAttachmentLoadOp::VK_ATTACHMENT_LOAD_OP_CLEAR;
		case sakura::graphics::ELoadOp::Load:
			return VkAttachmentLoadOp::VK_ATTACHMENT_LOAD_OP_LOAD;
		case sakura::graphics::ELoadOp::DontCare:
			return VkAttachmentLoadOp::VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		case sakura::graphics::ELoadOp::Count:
		default:
			return VkAttachmentLoadOp::VK_ATTACHMENT_LOAD_OP_MAX_ENUM;
		}
	}

	FORCEINLINE VkAttachmentStoreOp sakura::graphics::vk::translate(EStoreOp op)
	{
		switch (op)
		{
		case sakura::graphics::EStoreOp::Store:
			return VkAttachmentStoreOp::VK_ATTACHMENT_STORE_OP_STORE;
		case sakura::graphics::EStoreOp::DontCare:
			return VkAttachmentStoreOp::VK_ATTACHMENT_STORE_OP_DONT_CARE;
		case sakura::graphics::EStoreOp::Count:
		case sakura::graphics::EStoreOp::Clear:
		default:
			return VkAttachmentStoreOp::VK_ATTACHMENT_STORE_OP_MAX_ENUM;
		}
	}

	inline VkBufferUsageFlags sakura::graphics::vk::translate(const BufferUsages usage, const EBufferCPUAccess access)
	{
		using namespace sakura::graphics;
		VkBufferUsageFlags usages = 0;
		usages |= VkBufferUsageFlagBits::VK_BUFFER_USAGE_TRANSFER_DST_BIT;
		if (usage & EBufferUsage::IndexBuffer)
		{
			usages |= VkBufferUsageFlagBits::VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
		}
		if (usage & EBufferUsage::VertexBuffer)
		{
			usages |= VkBufferUsageFlagBits::VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
		}
		if (usage & EBufferUsage::UniformBuffer)
		{
			usages |= VkBufferUsageFlagBits::VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
		}
		if (usage & EBufferUsage::IndirectBuffer)
		{
			usages |= VkBufferUsageFlagBits::VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT;
		}
		if (usage & EBufferUsage::StorageBuffer)
		{
			usages |= VkBufferUsageFlagBits::VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;
		}
		if (usage & EBufferUsage::RayTracingAccelerateStructure)
		{
			usages |= VkBufferUsageFlagBits::VK_BUFFER_USAGE_RAY_TRACING_BIT_KHR;
		}
		if (usage & EBufferUsage::CopySrc)
		{
			usages |= VkBufferUsageFlagBits::VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
		}
		if (usage & EBufferUsage::CopyDst)
		{
			usages |= VkBufferUsageFlagBits::VK_BUFFER_USAGE_TRANSFER_DST_BIT;
		}
		return usages;
	}

	FORCEINLINE VkPrimitiveTopology sakura::graphics::vk::translate(EPrimitiveTopology topo)
	{
		switch (topo)
		{
		case sakura::graphics::EPrimitiveTopology::PointList:
			return VkPrimitiveTopology::VK_PRIMITIVE_TOPOLOGY_POINT_LIST;
		case sakura::graphics::EPrimitiveTopology::LineList:
			return VkPrimitiveTopology::VK_PRIMITIVE_TOPOLOGY_LINE_LIST;
		case sakura::graphics::EPrimitiveTopology::LineStrip:
			return VkPrimitiveTopology::VK_PRIMITIVE_TOPOLOGY_LINE_STRIP;
		case sakura::graphics::EPrimitiveTopology::TriangleList:
			return VkPrimitiveTopology::VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		case sakura::graphics::EPrimitiveTopology::TriangleStrip:
			return VkPrimitiveTopology::VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;
		case sakura::graphics::EPrimitiveTopology::Count:
		default:
			return VkPrimitiveTopology::VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		}
	}

	FORCEINLINE VkVertexInputRate sakura::graphics::vk::translate(VertexLayout::Frequency freq)
	{
		switch (freq)
		{
		case sakura::graphics::VertexLayout::Frequency::PerVertexData:
			return VkVertexInputRate::VK_VERTEX_INPUT_RATE_VERTEX;
		case sakura::graphics::VertexLayout::Frequency::PerInstanceData:
			return VkVertexInputRate::VK_VERTEX_INPUT_RATE_INSTANCE;
		default:
			return VkVertexInputRate::VK_VERTEX_INPUT_RATE_VERTEX;
		}
	}

	FORCEINLINE VkFormat sakura::graphics::vk::translate(EVertexFormat format)
	{
		switch (format)
		{
		case sakura::graphics::EVertexFormat::CHAR2:
			return VkFormat::VK_FORMAT_R8G8_SINT;
		case sakura::graphics::EVertexFormat::UCHAR2:
			return VkFormat::VK_FORMAT_R8G8_UINT;
		case sakura::graphics::EVertexFormat::UCHAR4:
			return VkFormat::VK_FORMAT_R8G8B8A8_UINT;
		case sakura::graphics::EVertexFormat::CHAR4:
			return VkFormat::VK_FORMAT_R8G8B8A8_SINT;
		case sakura::graphics::EVertexFormat::UCHAR2_NORM:
			return VkFormat::VK_FORMAT_R8G8_UNORM;
		case sakura::graphics::EVertexFormat::UCHAR4_NORM:
			return VkFormat::VK_FORMAT_R8G8B8A8_UNORM;
		case sakura::graphics::EVertexFormat::CHAR2_NORM:
			return VkFormat::VK_FORMAT_R8G8_SNORM;
		case sakura::graphics::EVertexFormat::CHAR4_NORM:
			return VkFormat::VK_FORMAT_R8G8B8A8_SNORM;
		case sakura::graphics::EVertexFormat::USHORT2:
			return VkFormat::VK_FORMAT_R16G16_UINT;
		case sakura::graphics::EVertexFormat::USHORT4:
			return VkFormat::VK_FORMAT_R16G16B16A16_UINT;
		case sakura::graphics::EVertexFormat::SHORT2:
			return VkFormat::VK_FORMAT_R16G16_SINT;
		case sakura::graphics::EVertexFormat::SHORT4:
			return VkFormat::VK_FORMAT_R16G16B16A16_SINT;
		case sakura::graphics::EVertexFormat::USHORT2_NORM:
			return VkFormat::VK_FORMAT_R16G16_UNORM;
		case sakura::graphics::EVertexFormat::USHORT4_NORM:
			return VkFormat::VK_FORMAT_R16G16B16A16_UNORM;
		case sakura::graphics::EVertexFormat::SHORT2_NORM:
			return VkFormat::VK_FORMAT_R16G16_SNORM;
		case sakura::graphics::EVertexFormat::SHORT4_NORM:
			return VkFormat::VK_FORMAT_R16G16B16A16_SNORM;
		case sakura::graphics::EVertexFormat::HALF2:
			return VkFormat::VK_FORMAT_R16G16_SFLOAT;
		case sakura::graphics::EVertexFormat::HALF4:
			return VkFormat::VK_FORMAT_R16G16B16A16_SFLOAT;
		case sakura::graphics::EVertexFormat::FLOAT:
			return VkFormat::VK_FORMAT_R32_SFLOAT;
		case sakura::graphics::EVertexFormat::FLOAT2:
			return VkFormat::VK_FORMAT_R32G32_SFLOAT;
		case sakura::graphics::EVertexFormat::FLOAT3:
			return VkFormat::VK_FORMAT_R32G32B32_SFLOAT;
		case sakura::graphics::EVertexFormat::FLOAT4:
			return VkFormat::VK_FORMAT_R32G32B32A32_SFLOAT;
		case sakura::graphics::EVertexFormat::UINT:
			return VkFormat::VK_FORMAT_R32_UINT;
		case sakura::graphics::EVertexFormat::UINT2:
			return VkFormat::VK_FORMAT_R32G32_UINT;
		case sakura::graphics::EVertexFormat::UINT3:
			return VkFormat::VK_FORMAT_R32G32B32_UINT;
		case sakura::graphics::EVertexFormat::UINT4:
			return VkFormat::VK_FORMAT_R32G32B32A32_UINT;
		case sakura::graphics::EVertexFormat::INT:
			return VkFormat::VK_FORMAT_R32_SINT;
		case sakura::graphics::EVertexFormat::INT2:
			return VkFormat::VK_FORMAT_R32G32_SINT;
		case sakura::graphics::EVertexFormat::INT3:
			return VkFormat::VK_FORMAT_R32G32B32_SINT;
		case sakura::graphics::EVertexFormat::INT4:
			return VkFormat::VK_FORMAT_R32G32B32A32_SINT;
		case sakura::graphics::EVertexFormat::COUNT:
		default:
			return VkFormat::VK_FORMAT_UNDEFINED;
		}
	}

	FORCEINLINE VkDescriptorType sakura::graphics::vk::translate(BindingLayout::EType type)
	{
		switch (type)
		{
		case sakura::graphics::BindingLayout::UniformBuffer:
			return VkDescriptorType::VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
		case sakura::graphics::BindingLayout::StorageBuffer:
			return VkDescriptorType::VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC;
		case sakura::graphics::BindingLayout::ReadonlyStorageBuffer:
			return VkDescriptorType::VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
		case sakura::graphics::BindingLayout::Sampler:
			return VkDescriptorType::VK_DESCRIPTOR_TYPE_SAMPLER;
		case sakura::graphics::BindingLayout::ComparisonSampler:
			return VkDescriptorType::VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		case sakura::graphics::BindingLayout::SampledTexture:
			return VkDescriptorType::VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;

			// TODO
		case sakura::graphics::BindingLayout::MultisampledTexture:
		case sakura::graphics::BindingLayout::ReadonlyStorageTexture:
		case sakura::graphics::BindingLayout::WriteonlyStorageTexture:
			// x
		case sakura::graphics::BindingLayout::Count:
		case sakura::graphics::BindingLayout::Invalid:
		default:
			return VkDescriptorType::VK_DESCRIPTOR_TYPE_MAX_ENUM;
		}
	}

	FORCEINLINE VkSamplerAddressMode vk::translate(ESamplerAddressMode mode)
	{
		switch (mode)
		{
		case ESamplerAddressMode::ClampToBorder:
			return VkSamplerAddressMode::VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
		case ESamplerAddressMode::ClampToEdge:
			return VkSamplerAddressMode::VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
		case ESamplerAddressMode::MirroredRepeat:
			return VkSamplerAddressMode::VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT;
		case ESamplerAddressMode::Repeat:
			return VkSamplerAddressMode::VK_SAMPLER_ADDRESS_MODE_REPEAT;
		case ESamplerAddressMode::MirrorClampToEdge:
			return VkSamplerAddressMode::VK_SAMPLER_ADDRESS_MODE_MIRROR_CLAMP_TO_EDGE;
		default:
			return VkSamplerAddressMode::VK_SAMPLER_ADDRESS_MODE_REPEAT;
		}
	}

	FORCEINLINE VkFilter vk::translate(EFilter filter)
	{
		switch (filter)
		{
		case EFilter::Linear:
			return VkFilter::VK_FILTER_LINEAR;
		case EFilter::Cubic:
			return VkFilter::VK_FILTER_CUBIC_IMG;
		case EFilter::Nearest:
			return VkFilter::VK_FILTER_NEAREST;
		default:
			return VkFilter::VK_FILTER_LINEAR;
		}
	}

	FORCEINLINE VkSamplerCreateInfo vk::translate(SamplerDescriptor desc)
	{
		VkSamplerCreateInfo sampler = {};
		sampler.addressModeU = translate(desc.address_mode_u);
		sampler.addressModeV = translate(desc.address_mode_v);
		sampler.addressModeW = translate(desc.address_mode_w);
		sampler.anisotropyEnable = desc.anisotropy;
		sampler.maxAnisotropy = desc.max_anisotropy;
		sampler.maxLod = desc.max_lod;
		sampler.minLod = desc.min_lod;
		sampler.magFilter = translate(desc.mag_filter);
		sampler.minFilter = translate(desc.min_filter);
		return sampler;
	}
}