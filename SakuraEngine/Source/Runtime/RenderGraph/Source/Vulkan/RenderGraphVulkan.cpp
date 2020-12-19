#include "RenderGraphVulkan/RenderGraphVulkan.h"

using namespace sakura::graphics;
using namespace sakura::graphics::vk;

RenderGraphVulkanAPI VkPresentModeKHR sakura::graphics::vk::translate(EPresentMode present_mode)
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

RenderGraphVulkanAPI EPresentMode sakura::graphics::vk::translate(VkPresentModeKHR present_mode)
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

RenderGraphVulkanAPI VkFormat sakura::graphics::vk::translate(ETextureFormat format)
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

RenderGraphVulkanAPI ETextureFormat sakura::graphics::vk::translate(VkFormat format)
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

RenderGraphVulkanAPI VkShaderStageFlagBits sakura::graphics::vk::translate(EShaderFrequency frequency)
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

RenderGraphVulkanAPI VkColorComponentFlags sakura::graphics::vk::translate(ColorMask mask)
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

RenderGraphVulkanAPI VkBlendOp sakura::graphics::vk::translate(EBlendOp blendOp)
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

RenderGraphVulkanAPI VkBlendFactor sakura::graphics::vk::translate(EBlendFactor blendFactor)
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

RenderGraphVulkanAPI VkPolygonMode sakura::graphics::vk::translate(EPolygonMode polygonMode)
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

RenderGraphVulkanAPI VkAttachmentLoadOp sakura::graphics::vk::translate(ELoadOp op)
{
	switch (op)
	{
	case sakura::graphics::ELoadOp::Clear:
		return VkAttachmentLoadOp::VK_ATTACHMENT_LOAD_OP_CLEAR;
	case sakura::graphics::ELoadOp::Load:
		return VkAttachmentLoadOp::VK_ATTACHMENT_LOAD_OP_LOAD;
	case sakura::graphics::ELoadOp::Count:
	default:
		return VkAttachmentLoadOp::VK_ATTACHMENT_LOAD_OP_MAX_ENUM;
	}
}

RenderGraphVulkanAPI VkAttachmentStoreOp sakura::graphics::vk::translate(EStoreOp op)
{
	switch (op)
	{
	case sakura::graphics::EStoreOp::Store:
		return VkAttachmentStoreOp::VK_ATTACHMENT_STORE_OP_STORE;
	case sakura::graphics::EStoreOp::Clear:
		return VkAttachmentStoreOp::VK_ATTACHMENT_STORE_OP_DONT_CARE;
	case sakura::graphics::EStoreOp::Count:
	default:
		return VkAttachmentStoreOp::VK_ATTACHMENT_STORE_OP_MAX_ENUM;
	}
}
