/*
 * @This File is Part of Sakura by SaeruHikari: 
 * @Descripttion: Common Pixel Format
 * @Version: 0.1.0
 * @Author: SaeruHikari
 * @Date: 2020-02-02 11:41:23
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2020-03-10 00:59:38
 */
#pragma once
#include <cinttypes>

namespace Sakura
{
    namespace Graphics
    {
        /**
         * @description: Color Components of RGBA, simply casted from Vk flags 
         * @author: SaeruHikari
         */
        enum ColorComponentFlags 
        {
            ColorComponentR = 0x00000001,
            ColorComponentG = 0x00000002,
            ColorComponentB = 0x00000004,
            ColorComponentA = 0x00000008
        };
        using ColorComoponents = std::uint32_t;

        /**
         * @description: Blend factor enum, simply casted from Vk Op enum. 
         * @author: SaeruHikari
         */
        enum BlendFactor
        {
            BlendFactorZero = 0,
            BlendFactorOne = 1,
            BlendFactorSrcColor = 2,
            BlendFactorOneMinusSrcColor = 3,
            BlendFactorDstColor = 4,
            BlendFactorOneMinusDstColor = 5,
            BlendFactorSrcAlpha = 6,
            BlendFactorOneMinusSrcAlpha = 7,
            BlendFactorDstAlpha = 8,
            BlendFactorOneMinusDstAlpha = 9,
            BlendFactorConstantColor = 10,
            BlendFactorOneMinusConstantColor = 11,
            BlendFactorConstantAlpha = 12,
            BlendFactorOneMinusConstantAlpha = 13,
            BlendFactorSrcAlphaSaturate = 14,
            BlendFactorSrc1Color = 15,
            BlendFactorOneMinusSrc1Color = 16,
            BlendFactorSrc1Alpha = 17,
            BlendFactorOneMinusSrc1Alpha = 18
        };

        /**
         * @description: Blend op enum, simply casted from Vk Op enum. 
         * @author: SaeruHikari
         */
        enum BlendOp
        {
            BlendOpAdd = 0,
            BlendOpSubstract = 1,
            BlendOpReverseSubstract = 2,
            BlendOpMin = 3,
            BlendOpMax = 4,
            BlendOpZeroExt = 1000148000,
            BlnedOpSrcExt = 1000148001,
            BlendOpDstExt = 1000148002,
            BlnedOpSrcOverExt = 1000148003,
            BlnedOpDstOverExt = 1000148004,
            BlnedOpSrcInExt = 1000148005,
            BlnedOpDstInExt = 1000148006,
            BlnedOpSrcOutExt = 1000148007,
            BlnedOpDstOutExt = 1000148008,
            BlendOpSrcAtopExt = 1000148009,
            BlendOpDstAtopExt = 1000148010,
            BlendOpXorExt = 1000148011,
            BlendOpMultiplyExt = 1000148012,
            BlendOpScreenExt = 1000148013,
            BlendOpOverlayExt = 1000148014,
            BlendOpDarkenExt = 1000148015,
            BlendOpLightenExt = 1000148016,
            BlendOpColorDodgeExt = 1000148017,
            BlendOpColorBurnExt = 1000148018,
            BlendOpHardLightExt = 1000148019,
            BlendOpSoftLightExt = 1000148020,
            BlendOpDifferenceExt = 1000148021,
            BlendOpExclusionExt = 1000148022,
            BlendOpInvertExt = 1000148023,
            BlendOpInvertRGBExt = 1000148024,
            BlendOpLinearDodgeExt = 1000148025,
            BlendOpLinearBurnExt = 1000148026,
            BlendOpVividLightExt = 1000148027,
            BlendOpLinearLightExt = 1000148028,
            BlendOpPinLightExt = 1000148029,
            BlendOpHardMixExt = 1000148030,
            BlendOpHSLHueExt = 1000148031,
            BlendOpHSLSaturationExt = 1000148032,
            BlendOpHSLColorExt = 1000148033,
            BlendOpHSLLuminosityExt = 1000148034,
            BlendOpPlusExt = 1000148035,
            BlendOpPlusClampedExt = 1000148036,
            BlendOpPlusClampedAlphaExt = 1000148037,
            BlendOpDarkerExt = 1000148038,
            BlendOpMinusExt = 1000148039,
            BlendOpMinusClampedExt = 1000148040,
            BlendOpContrastExt = 1000148041,
            BlendOpInvertOVGExt = 1000148042,
            BlendOpRedExt = 1000148043,
            BlendOpGreenExt = 1000148044,
            BlendOpBlueExt = 1000148045,
        };

        /**
         * @description: Logic op enum, simply casted from Vk Op enum. 
         * @author: SaeruHikari
         */
        enum LogicOp
        {
            LogicOpClear = 0,
            LogicOpAnd = 1,
            LogicOpAndReverse = 2,
            LogicOpCopy = 3,
            LogicOpAndInverted = 4,
            LogicOpNoOp = 5,
            LogicOpXor = 6,
            LogicOpOr = 7,
            LogicOpNor = 8,
            LogicOpEquivalent = 9,
            LogicOpInvert = 10,
            LogicOpOrReverse = 11,
            LogicOpCopyInverted = 12,
            LogicOpOrInverted = 13,
            LogicOpNand = 14,
            LogicOpSet = 15
        };

        enum Format 
        {   
            UNKNOWN,
            R64G64B64A64_UINT,
            R64G64B64A64_SINT,
            R64G64B64A64_SFLOAT,
            
            R64G64B64_UINT,
            R64G64B64_SINT,
            R64G64B64_SFLOAT,

            R32G32B32A32_TYPELESS,
            R32G32B32A32_UINT,
            R32G32B32A32_SINT,
            R32G32B32A32_SFLOAT,

            R64G64_UINT,
            R64G64_SINT,
            R64G64_SFLOAT,

            R32G32B32_TYPELESS,
            R32G32B32_UINT,
            R32G32B32_SINT,
            R32G32B32_SFLOAT,

            R16G16B16A16_TYPELESS,
            R16G16B16A16_UNORM,
            R16G16B16A16_SNORM,
            R16G16B16A16_USCALED,
            R16G16B16A16_SSCALED,
            R16G16B16A16_UINT,
            R16G16B16A16_SINT,
            R16G16B16A16_SFLOAT,
            
            R32G32_TYPELESS,
            R32G32_UINT,
            R32G32_SINT,
            R32G32_SFLOAT,
            
            R64_UINT,
            R64_SINT,
            R64_SFLOAT,

            R16G16B16_UNORM,
            R16G16B16_SNORM,
            R16G16B16_USCALED,
            R16G16B16_SSCALED,
            R16G16B16_UINT,
            R16G16B16_SINT,
            R16G16B16_SFLOAT,

            R10G10B10A2_TYPELESS_PACK32,
            R10G10B10A2_UNORM_PACK32,
            R10G10B10A2_SNORM_PACK32,
            R10G10B10A2_USCALED_PACK32,
            R10G10B10A2_SSCALED_PACK32,
            R10G10B10A2_UINT_PACK32,
            R10G10B10A2_SINT_PACK32,

            R10G10B10_XR_BIAS_A2_UNORM_PACK32,

            B10G10R10A2_UNORM_PACK32,
            B10G10R10A2_SNORM_PACK32,
            B10G10R10A2_USCALED_PACK32,
            B10G10R10A2_SSCALED_PACK32,
            B10G10R10A2_UINT_PACK32,
            B10G10R10A2_SINT_PACK32,
            
            A2B10G10R10_UNORM_PACK32,
            A2B10G10R10_SNORM_PACK32,
            A2B10G10R10_USCALED_PACK32,
            A2B10G10R10_SSCALED_PACK32,
            A2B10G10R10_UINT_PACK32,
            A2B10G10R10_SINT_PACK32,

            R9G9B9E5_UFLOAT_PACK32,

            R8G8B8A8_TYPELESS,
            R8G8B8A8_UNORM,
            R8G8B8A8_UNORM_PACK32,
            R8G8B8A8_SNORM,
            R8G8B8A8_SNORM_PACK32,
            R8G8B8A8_USCALED,
            R8G8B8A8_USCALED_PACK32,
            R8G8B8A8_SSCALED,
            R8G8B8A8_SSCALED_PACK32,
            R8G8B8A8_UINT,
            R8G8B8A8_UINT_PACK32,
            R8G8B8A8_SINT,
            R8G8B8A8_SINT_PACK32,
            R8G8B8A8_SRGB,
            R8G8B8A8_SRGB_PACK32,
            
            B8G8R8A8_TYPELESS,
            B8G8R8A8_UNORM,
            B8G8R8A8_SNORM,
            B8G8R8A8_USCALED,
            B8G8R8A8_SSCALED,
            B8G8R8A8_UINT,
            B8G8R8A8_SINT,
            B8G8R8A8_SRGB,

            R11G11B10_UFLOAT_PACK32,
            
            B8G8R8X8_TYPELESS,
            B8G8R8X8_UNORM,
            B8G8R8X8_SRGB,

            R16G16_TYPELESS,
            R16G16_UNORM,
            R16G16_SNORM,
            R16G16_USCALED,
            R16G16_SSCALED,
            R16G16_UINT,
            R16G16_SINT,
            R16G16_SFLOAT,
            
            R32_TYPELESS,
            R32_UINT,
            R32_SINT,
            R32_SFLOAT,
            R8G8B8_UNORM,
            R8G8B8_SNORM,
            R8G8B8_USCALED,
            R8G8B8_SSCALED,
            R8G8B8_UINT,
            R8G8B8_SINT,
            R8G8B8_SRGB,
            
            B8G8R8_UNORM,
            B8G8R8_SNORM,
            B8G8R8_USCALED,
            B8G8R8_SSCALED,
            B8G8R8_UINT,
            B8G8R8_SINT,
            B8G8R8_SRGB,

            B5G5R5A1_UNORM_PACK16,
            A1B5G5R5_UNORM_PACK16,
            A1R5G5B5_UNORM_PACK16,
            A4B4G4R4_UNORM_PACK16,
            A4R4G4B4_UNORM_PACK16,
            
            R5G6B5_UNORM_PACK16,
            B5G6R5_UNORM_PACK16,

            R8G8_TYPELESS,
            R8G8_UNORM,
            R8G8_SNORM,
            R8G8_USCALED,
            R8G8_SSCALED,
            R8G8_UINT,
            R8G8_SINT,
            R8G8_SRGB,

            R16_TYPELESS,
            R16_UNORM,
            R16_SNORM,
            R16_USCALED,
            R16_SSCALED,
            R16_UINT,
            R16_SINT,
            R16_SFLOAT,

            G4R4_UNORM_PACK8,
            R4G4_UNORM_PACK8,

            R8_TYPELESS,
            R8_UNORM,
            R8_SNORM,
            R8_USCALED,
            R8_SSCALED,
            R8_UINT,
            R8_SINT,
            R8_SRGB,
            
            A8_UNORM,
            R1_UNORM,
            R32G8X24_TYPELESS_2PACK32,
            
            R32_SFLOAT_X8X24_TYPELESS_2PACK32,
            X32_TYPELESS_G8X24_UINT_2PACK32,
            
            D32_SFLOAT_S8_UINT,
            R24G8_TYPELESS_PACK32,
            D24_UNORM_S8_UINT,
            R24_UNORM_X8_TYPELESS_PACK32,
            X24_TYPELESS_G8_UINT_PACK32,
            D24_UNORM_X8_TYPELESS_PACK32,
            D32_SFLOAT,

            D16_UNORM_S8_UINT,
            D16_UNORM,

            S8_UINT,
            
            BC1_TYPELESS_BLOCK,
            BC1_RGBA_UNORM_BLOCK,
            BC1_RGBA_SRGB_BLOCK,
            BC1_UNORM_BLOCK,
            BC1_SRGB_BLOCK,

            BC2_TYPELESS_BLOCK,
            BC2_UNORM_BLOCK,
            BC2_SRGB_BLOCK,

            BC3_TYPELESS_BLOCK,
            BC3_UNORM_BLOCK,
            BC3_SRGB_BLOCK,

            BC4_TYPELESS_BLOCK,
            BC4_UNORM_BLOCK,
            BC4_SNORM_BLOCK,

            BC5_TYPELESS_BLOCK,
            BC5_UNORM_BLOCK,
            BC5_SNORM_BLOCK,

            BC6H_TYPELESS_BLOCK,
            BC6H_UFLOAT_BLOCK,
            BC6H_SFLOAT_BLOCK,

            BC7_TYPELESS_BLOCK,
            BC7_UNORM_BLOCK,
            BC7_SRGB_BLOCK,
            
            ETC2_R8G8B8A8_UNORM_BLOCK,
            ETC2_R8G8B8A8_SRGB_BLOCK,
            ETC2_R8G8B8A1_UNORM_BLOCK,
            ETC2_R8G8B8A1_SRGB_BLOCK,

            ETC2_R8G8B8_UNORM_BLOCK,
            ETC2_R8G8B8_SRGB_BLOCK,
            
            EAC_R11G11_UNORM_BLOCK,
            EAC_R11G11_SNORM_BLOCK,
            EAC_R11_UNORM_BLOCK,
            EAC_R11_SNORM_BLOCK,

            ASTC_4x4_UNORM_BLOCK,
            ASTC_4x4_SRGB_BLOCK,
            ASTC_4x4_SFLOAT_BLOCK_EXT,

            ASTC_5x4_UNORM_BLOCK,
            ASTC_5x4_SRGB_BLOCK,
            ASTC_5x4_SFLOAT_BLOCK_EXT,

            ASTC_5x5_UNORM_BLOCK,
            ASTC_5x5_SRGB_BLOCK,
            ASTC_5x5_SFLOAT_BLOCK_EXT,

            ASTC_6x5_UNORM_BLOCK,
            ASTC_6x5_SRGB_BLOCK,
            ASTC_6x5_SFLOAT_BLOCK_EXT,

            ASTC_6x6_UNORM_BLOCK,
            ASTC_6x6_SRGB_BLOCK,
            ASTC_6x6_SFLOAT_BLOCK_EXT,
            
            ASTC_8x5_UNORM_BLOCK,
            ASTC_8x5_SRGB_BLOCK,
            ASTC_8x5_SFLOAT_BLOCK_EXT,

            ASTC_8x6_UNORM_BLOCK,
            ASTC_8x6_SRGB_BLOCK,
            ASTC_8x6_SFLOAT_BLOCK_EXT,

            ASTC_8x8_UNORM_BLOCK,
            ASTC_8x8_SRGB_BLOCK,
            ASTC_8x8_SFLOAT_BLOCK_EXT,

            ASTC_10x5_UNORM_BLOCK,
            ASTC_10x5_SRGB_BLOCK,
            ASTC_10x5_SFLOAT_BLOCK_EXT,

            ASTC_10x6_UNORM_BLOCK,
            ASTC_10x6_SRGB_BLOCK,
            ASTC_10x6_SFLOAT_BLOCK_EXT,

            ASTC_10x8_UNORM_BLOCK,
            ASTC_10x8_SRGB_BLOCK,
            ASTC_10x8_SFLOAT_BLOCK_EXT,

            ASTC_10x10_UNORM_BLOCK,
            ASTC_10x10_SRGB_BLOCK,
            ASTC_10x10_SFLOAT_BLOCK_EXT,

            ASTC_12x10_UNORM_BLOCK,
            ASTC_12x10_SRGB_BLOCK,
            ASTC_12x10_SFLOAT_BLOCK_EXT,

            ASTC_12x12_UNORM_BLOCK,
            ASTC_12x12_SRGB_BLOCK,
            ASTC_12x12_SFLOAT_BLOCK_EXT,

            R12X4G12X4B12X4A12X4_UNORM_4PACK16,
            R10X6G10X6B10X6A10X6_UNORM_4PACK16,
            R12X4G12X4_UNORM_2PACK16,
            R10X6G10X6_UNORM_2PACK16,
            R12X4_UNORM_PACK16,
            R10X6_UNORM_PACK16,
            R8G8_B8G8_UNORM,
            G16B16_G16R16_422_UNORM,
            G12X4B12X4_G12X4R12X4_422_UNORM_4PACK16,
            G10X6B10X6_G10X6R10X6_422_UNORM_4PACK16,
            G8B8_G8R8_422_UNORM,
            G8R8_G8B8_UNORM,
            B16G16_R16G16_422_UNORM,
            B12X4G12X4_R12X4G12X4_422_UNORM_4PACK16,
            B10X6G10X6_R10X6G10X6_422_UNORM_4PACK16,
            B8G8_R8G8_422_UNORM,

            G16_B16_R16_3PLANE_444_UNORM,
            G16_B16_R16_3PLANE_422_UNORM,
            // ----------------------
            G12X4_B12X4_R12X4_3PLANE_444_UNORM_3PACK16,
            G12X4_B12X4_R12X4_3PLANE_422_UNORM_3PACK16,
            G10X6_B10X6_R10X6_3PLANE_444_UNORM_3PACK16,
            G10X6_B10X6_R10X6_3PLANE_422_UNORM_3PACK16,
            G16_B16_R16_3PLANE_420_UNORM,
            G12X4_B12X4_R12X4_3PLANE_420_UNORM_3PACK16,
            G10X6_B10X6_R10X6_3PLANE_420_UNORM_3PACK16,
            G8_B8_R8_3PLANE_444_UNORM,
            G8_B8_R8_3PLANE_422_UNORM,
            G8_B8_R8_3PLANE_420_UNORM,
            G16_B16R16_2PLANE_422_UNORM,
            G12X4_B12X4R12X4_2PLANE_422_UNORM_3PACK16,
            G10X6_B10X6R10X6_2PLANE_422_UNORM_3PACK16,
            G16_B16R16_2PLANE_420_UNORM,
            G12X4_B12X4R12X4_2PLANE_420_UNORM_3PACK16,
            G10X6_B10X6R10X6_2PLANE_420_UNORM_3PACK16,
            G8_B8R8_2PLANE_422_UNORM,
            G8_B8R8_2PLANE_420_UNORM,
            PVRTC1_4BPP_UNORM_BLOCK_IMG,
            PVRTC1_4BPP_SRGB_BLOCK_IMG,
            PVRTC1_2BPP_UNORM_BLOCK_IMG,
            PVRTC1_2BPP_SRGB_BLOCK_IMG,
            PVRTC2_4BPP_UNORM_BLOCK_IMG,
            PVRTC2_4BPP_SRGB_BLOCK_IMG,
            PVRTC2_2BPP_UNORM_BLOCK_IMG,
            PVRTC2_2BPP_SRGB_BLOCK_IMG,
            //--------------------------------------------------------------
            // common formats (dxgi and vulkan)
            S_R32G32B32A32_UINT = R32G32B32A32_UINT,
            S_R32G32B32A32_SINT = R32G32B32A32_SINT,
            S_R32G32B32A32_SFLOAT = R32G32B32A32_SFLOAT,
            S_R32G32B32_UINT = R32G32B32_UINT,
            S_R32G32B32_SINT = R32G32B32_SINT,
            S_R32G32B32_SFLOAT = R32G32B32_SFLOAT,
            S_R16G16B16A16_UNORM = R16G16B16A16_UNORM,
            S_R16G16B16A16_SNORM = R16G16B16A16_SNORM,
            S_R16G16B16A16_UINT = R16G16B16A16_UINT,
            S_R16G16B16A16_SINT = R16G16B16A16_SINT,
            S_R16G16B16A16_SFLOAT = R16G16B16A16_SFLOAT,
            S_R32G32_UINT = R32G32_UINT,
            S_R32G32_SINT = R32G32_SINT,
            S_R32G32_SFLOAT = R32G32_SFLOAT,
            S_R10G10B10A2_UNORM_PACK32 = R10G10B10A2_UNORM_PACK32,
            S_R10G10B10A2_UINT_PACK32 = R10G10B10A2_UINT_PACK32,
            S_R9G9B9E5_UFLOAT_PACK32 = R9G9B9E5_UFLOAT_PACK32,
            S_R8G8B8A8_UNORM = R8G8B8A8_UNORM,
            S_R8G8B8A8_SNORM = R8G8B8A8_SNORM,
            S_R8G8B8A8_UINT = R8G8B8A8_UINT,
            S_R8G8B8A8_SINT = R8G8B8A8_SINT,
            S_R8G8B8A8_SRGB = R8G8B8A8_SRGB,
            S_B8G8R8A8_UNORM = B8G8R8A8_UNORM,
            S_B8G8R8A8_SRGB = B8G8R8A8_SRGB,
            S_R11G11B10_UFLOAT_PACK32 = R11G11B10_UFLOAT_PACK32,
            S_R16G16_UNORM = R16G16_UNORM,
            S_R16G16_SNORM = R16G16_SNORM,
            S_R16G16_UINT = R16G16_UINT,
            S_R16G16_SINT = R16G16_SINT,
            S_R16G16_SFLOAT = R16G16_SFLOAT,
            S_R32_UINT = R32_UINT,
            S_R32_SINT = R32_SINT,
            S_R32_SFLOAT = R32_SFLOAT,
            S_B5G5R5A1_UNORM_PACK16 = B5G5R5A1_UNORM_PACK16,
            S_B5G6R5_UNORM_PACK16 = B5G6R5_UNORM_PACK16,
            S_R8G8_UNORM = R8G8_UNORM,
            S_R8G8_SNORM = R8G8_SNORM,
            S_R8G8_UINT = R8G8_UINT,
            S_R8G8_SINT = R8G8_SINT,
            S_R16_UNORM = R16_UNORM,
            S_R16_SNORM = R16_SNORM,
            S_R16_UINT = R16_UINT,
            S_R16_SINT = R16_SINT,
            S_R16_SFLOAT = R16_SFLOAT,
            S_R8_UNORM = R8_UNORM,
            S_R8_SNORM = R8_SNORM,
            S_R8_UINT = R8_UINT,
            S_R8_SINT = R8_SINT,
            S_D32_SFLOAT_S8_UINT = D32_SFLOAT_S8_UINT,
            S_D24_UNORM_S8_UINT = D24_UNORM_S8_UINT,
            S_D32_SFLOAT = D32_SFLOAT,
            S_D16_UNORM = D16_UNORM,
            S_BC1_UNORM_BLOCK = BC1_UNORM_BLOCK,
            S_BC1_SRGB_BLOCK = BC1_SRGB_BLOCK,
            S_BC2_UNORM_BLOCK = BC2_UNORM_BLOCK,
            S_BC2_SRGB_BLOCK = BC2_SRGB_BLOCK,
            S_BC3_UNORM_BLOCK = BC3_UNORM_BLOCK,
            S_BC3_SRGB_BLOCK = BC3_SRGB_BLOCK,
            S_BC4_UNORM_BLOCK = BC4_UNORM_BLOCK,
            S_BC4_SNORM_BLOCK = BC4_SNORM_BLOCK,
            S_BC5_UNORM_BLOCK = BC5_UNORM_BLOCK,
            S_BC5_SNORM_BLOCK = BC5_SNORM_BLOCK,
            S_BC6H_UFLOAT_BLOCK = BC6H_UFLOAT_BLOCK,
            S_BC6H_SFLOAT_BLOCK = BC6H_SFLOAT_BLOCK,
            S_BC7_UNORM_BLOCK = BC7_UNORM_BLOCK,
            S_BC7_SRGB_BLOCK = BC7_SRGB_BLOCK,
        };
    }
}
