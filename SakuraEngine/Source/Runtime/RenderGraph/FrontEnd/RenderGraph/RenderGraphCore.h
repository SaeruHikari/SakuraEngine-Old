#pragma once
#include "Base/GenerationalId.h"
#include "Containers/constexpr_map.hpp"
#include "System/vfs/path.h"
#include "System/Window.h"
#include "RuntimeCore/RuntimeCore.h"

namespace sakura::graphics
{
	using extent2d = sakura::extent2d;
	using extent3d = sakura::extent3d;
	struct RenderCommandBuffer;
	class RenderGraph;
	namespace ____
	{
		template<bool True> struct uhalf {};
		template<> struct uhalf<sizeof(size_t) == sizeof(uint32)> { using type = uint16; constexpr static uint16 max = UINT16_MAX; };
		template<> struct uhalf<sizeof(size_t) == sizeof(uint64)> { using type = uint32; constexpr static uint32 max = UINT32_MAX; };
		static_assert(sizeof(uhalf<true>::type) == sizeof(size_t) / 2, "uhalf type error! size should be half as size_t.");
	};
}

namespace sakura::graphics
{
	enum class EBackend : uint16
	{
		Stub,
		D3D12,
		Vulkan,
		WebGPU,
		Metal,
		PS5, //PlayStation 5
		Count
	};

	enum class EIndexFormat : uint8
	{
		UINT16 = 0x00000001,
		UINT32 = 0x00000002,

		// Useful for clusters or mesh-pipeline-workflow.
		UINT8 = 0x00000003
	};

	enum class ESharingMode : uint8
	{
		Exclusive = 0,
		Concurrent = 1
	};
	
	enum class EVertexFormat : uint16
	{
		UCHAR2,
		UCHAR4,
		CHAR2,
		CHAR4,
		UCHAR2_NORM,
		UCHAR4_NORM,
		CHAR2_NORM,
		CHAR4_NORM,
		USHORT2,
		USHORT4,
		SHORT2,
		SHORT4,
		USHORT2_NORM,
		USHORT4_NORM,
		SHORT2_NORM,
		SHORT4_NORM,
		HALF2,
		HALF4,
		FLOAT,
		FLOAT2,
		FLOAT3,
		FLOAT4,
		UINT,
		UINT2,
		UINT3,
		UINT4,
		INT,
		INT2,
		INT3,
		INT4,
		COUNT,
		
		R8G8_UNORM = UCHAR2_NORM,
		R8G8_NORM = CHAR2_NORM,
		R8G8B8A8_UNORM = UCHAR4_NORM,
		R8G8B8A8_NORM = CHAR4_NORM,
		R16G16_UNORM = USHORT2_NORM,
		R16G16_NORM = SHORT2_NORM,
		R16G16B16A16_UNORM = USHORT4_NORM,
		R16G16B16A16_NORM = SHORT4_NORM,
		R32G32B32A32_UINT = UINT4,
		R32G32B32A32_INT = INT4,
		R32G32B32A32_SINT = INT4,
		R32G32_UINT = UINT2,
		R32G32_INT = INT2,
		R32G32_SINT = INT2,
	};

	enum class ETextureFormat : uint16
	{
		R32G32B32A32_UINT = 0,
		R32G32B32A32_SINT = 1,
		R32G32B32A32_FLOAT = 2,
		R32G32B32_UINT = 3,
		R32G32B32_SINT = 4,
		R32G32B32_FLOAT = 5,
		R16G16B16A16_UNORM = 6,
		R16G16B16A16_SNORM = 7,
		R16G16B16A16_UINT = 8,
		R16G16B16A16_SINT = 9,
		R16G16B16A16_FLOAT = 10,
		R32G32_UINT = 11,
		R32G32_SINT = 12,
		R32G32_FLOAT = 13,
		R10G10B10A2_UNORM = 14,
		R10G10B10A2_UINT = 15,
		R9G9B9E5_UFLOAT = 16,
		R8G8B8A8_UNORM = 17,
		R8G8B8A8_SNORM = 18,
		R8G8B8A8_UINT = 19,
		R8G8B8A8_SINT = 20,
		R8G8B8A8_UNORM_SRGB = 21,
		B8G8R8A8_UNORM = 22,
		B8G8R8A8_UNORM_SRGB = 23,
		R11G11B10_FLOAT = 24,
		R16G16_UNORM = 25,
		R16G16_SNORM = 26,
		R16G16_UINT = 27,
		R16G16_SINT = 28,
		R16G16_FLOAT = 29,
		R32_UINT = 30,
		R32_SINT = 31,
		R32_FLOAT = 32,
		B5G5R5A1_UNORM = 33,
		B5G6R5_UNORM = 34,
		R8G8_UNORM = 35,
		R8G8_SNORM = 36,
		R8G8_UINT = 37,
		R8G8_SINT = 38,
		R16_UNORM = 39,
		R16_SNORM = 40,
		R16_UINT = 41,
		R16_SINT = 42,
		R16_FLOAT = 43,
		R8_UNORM = 44,
		R8_SNORM = 45,
		R8_UINT = 46,
		R8_SINT = 47,
		D24_UNORM_S8_UINT = 49,
		D32_FLOAT = 50,
		D16_UNORM = 51,
		BC1_UNORM = 52,
		BC1_UNORM_SRGB = 53,
		BC2_UNORM = 54,
		BC2_UNORM_SRGB = 55,
		BC3_UNORM = 56,
		BC3_UNORM_SRGB = 57,
		BC4_UNORM = 58,
		BC4_SNORM = 59,
		BC5_UNORM = 60,
		BC5_SNORM = 61,
		BC6H_UF16 = 62,
		BC6H_SF16 = 63,
		BC7_UNORM = 64,
		BC7_UNORM_SRGB = 65,
		Count = BC7_UNORM_SRGB + 1
	};

	enum class ERenderCommandType : uint32_t
	{
		set_scissor_rect,
		set_vbs,
		set_ib,

		draw,
		draw_indirect,

		dispatch,
		dispatch_indirect,

		update_binding,

		copy_buffer_to_buffer,
		copy_buffer_to_texture,
		copy_texture_to_texture,
		copy_texture_to_buffer,

		fence, // ? necessary ?
		// GPUQueue wait GPUQueue (semaphores) ==> Pass dependencies.
		// GPU wait CPU ==> .....Seems useless, why not ComputePipeline ?
		// CPU wait GPU ==> What about non-block callback pass->on_finish() ?

		barriers, // optional
		transitions, // optional

		begin_timing,
		end_timing,
		resolve_timings,

		begin_event,
		end_event,

		begin_render_pass,
		end_render_pass,

		begin_compute_pass,
		end_compute_pass,

		ray_trace,
		update_top_level,
		update_bottom_level,
		update_shader_table,
		count
	};

	enum ERenderQueueType
	{
		QueueTypeNone = 0x00,
		Copy = 0x01,
		Compute = 0x02,
		Graphics = 0x04,
		QueueTypeCopy = Copy,
		QueueTypeCompute = Compute,
		QueueTypeGraphics = Graphics,
		QueueTypeAll = Copy | Compute | Graphics
	};

	enum class EBufferCPUAccess : uint8
	{
		None,
		Read,
		Write,
		ReadWrite
	};

	enum class EShaderLanguage
	{
		HLSL,
		SPIRV,
		GLSL,
		WGSL,
		PSSL,
		PSSL2,
		MTL,
		Count
	};

	// Shader Frequency
	enum EShaderFrequency 
	{
		Invalid = 0x00000000,
		// Default pipeline 3
		VertexShader = 0x00000001,
		PixelShader = 0x00000002,
		ComputeShader = 0x00000004,
		// Mesh pipeline 2
		TaskShader = 0x00000010,
		MeshShader = 0x00000020,
		// RayTracing 6
		AnyHitShader = 0x00000100,
		CallableShader = 0x00000200,
		ClosestHitShader = 0x00000400,
		IntersectionShader = 0x00000800,
		MissShader = 0x00001000,
		RayGenerationShader = 0x00002000,
		// 1 + 3 + 2 + 6
		Count = 12
	};
	using EShaderFrequencys = uint32;

	RenderGraphAPI bool isMeshPipelineStage(const EShaderFrequency freq);
	RenderGraphAPI bool isRayTracingStage(const EShaderFrequency freq);
	RenderGraphAPI bool isRenderPipelineStage(const EShaderFrequency freq);

	enum class EShaderCodeFormat
	{
		Spirv,
		DXBC,
		DXIL,
		GLSL,
		PSSL2, //PlayStation 5
		Count
	};

	enum class EPrimitiveTopology : uint32
	{
		PointList,
		LineList,
		LineStrip,
		TriangleList,
		TriangleStrip,
		Count
	};

	enum class EPolygonMode
	{
		FILL = 0,
		LINE = 1,
		POINT = 2
	};

	enum class EBlendOp : uint8
	{
		Add,
		Subtract,
		ReverseSubtract,
		Min,
		Max
	};

	enum class EBlendFactor : uint8
	{
		Zero = 0x00,
		One = 0x01,
		SrcColor = 0x02,
		OneMinusSrcColor = 0x03,
		SrcAlpha = 0x04,
		OneMinusSrcAlpha = 0x05,
		DstColor = 0x06,
		OneMinusDstColor = 0x07,
		DstAlpha = 0x08,
		OneMinusDstAlpha = 0x09,
		SrcAlphaSaturated = 0x0A,
		BlendColor = 0x0B,
		OneMinusBlendColor = 0x0C
	};

	enum class EFilter : uint8
	{
		Nearest = 0,
		Linear = 1,
		Cubic = 2
	};
	
	enum EColorMaskBits : uint8
	{
		None = 0x00,
		R = 0x01,
		G = 0x02,
		B = 0x04,
		A = 0x08,
		All = 0x0F,
	};
	using ColorMask = uint8;

	enum class EStencilOp : uint8
	{
		Keep = 0,
		Zero = 1,
		Replace = 2,
		Invert = 3,
		IncrementClamp = 4,
		DecrementClamp = 5,
		IncrementWrap = 6,
		DecrementWrap = 7
	};

	enum class ELoadOp : uint8
	{
		Clear = 0,
		Load = 1,
		DontCare = 2,
		Count
	};

	enum class EStoreOp : uint8
	{
		Store = 0,
		Clear = 1,
		DontCare = 2,
		Count
	};

	enum class ECullMode : uint8
	{
		None = 0,
		Back = 1,
		Front = 2,
		FrontAndBack = 3,
		Count
	};

	using RenderGraphId = sakura::GenerationalId;
	struct RenderGraphAPI RenderObjectHandle
	{
		RenderObjectHandle(const RenderGraphId id_);
		inline operator size_t() const { return id_; }
		inline operator bool() const { return id_; }
		inline RenderGraphId id() const { return id_; }
	protected:
		RenderObjectHandle();
		RenderGraphId id_;
	};

	struct RenderGraphAPI RenderPassHandle
	{
		RenderPassHandle(const RenderGraphId id_);
		inline operator size_t() const { return id_; }
		inline operator bool() const { return id_; }
		inline RenderGraphId id() const { return id_; }
	protected:
		RenderPassHandle();
		RenderGraphId id_;
	};

	struct RenderGraphAPI RenderResourceHandle
	{
		RenderResourceHandle(const RenderGraphId id_);
		RenderResourceHandle();
		inline operator size_t() const { return id_; }
		inline operator bool() const { return id_; }
		inline RenderGraphId id() const { return id_; }
	protected:
		RenderGraphId id_;
	};

	struct RenderGraphAPI IGpuObject
	{
		virtual ~IGpuObject() = default;
		virtual RenderObjectHandle handle() const = 0;
		virtual sakura::string format() const { return ""; }
	};

	struct RenderGraphAPI IGpuMemoryResource
	{
		virtual ~IGpuMemoryResource() = default;
		virtual RenderResourceHandle handle() const = 0;
		virtual size_t size() const = 0;
		virtual sakura::string format() const { return ""; }
	};

	template<typename _ResourceType>
	struct TypedRenderResourceHandle : public RenderResourceHandle
	{
		using ResourceType = _ResourceType;
		TypedRenderResourceHandle(const RenderGraphId id_);
		TypedRenderResourceHandle() = default;
		static_assert(std::is_base_of_v<IGpuMemoryResource, ResourceType>, "Resource Handle must be used with GPU Resources!");
	};
	template<typename _ObjectType>
	struct TypedRenderObjectHandle : public RenderObjectHandle
	{
		using ObjectType = _ObjectType;
		TypedRenderObjectHandle(const RenderGraphId id_);
		TypedRenderObjectHandle() = default;
		static_assert(std::is_base_of_v<IGpuObject, ObjectType>, "GPUObject Handle must be used with GPU Objects!");
	};

	struct Buffer
	{
		enum EUsage
		{
			Unknown,

			CopySrc,
			CopyDst,

			IndexBuffer,
			VertexBuffer,
			UniformBuffer,
			IndirectBuffer,
			StorageBuffer,
			Query,
			RayTracingAccelerateStructure
		};

		using Usages = uint32;
		
		struct RenderGraphAPI Descriptor
		{
			const Usages usages = EUsage::UniformBuffer;
			const EBufferCPUAccess access = EBufferCPUAccess::None;
			const size_t length = 0;
			const void* data = nullptr;
			
			Descriptor(Usages usage = EUsage::UniformBuffer, size_t length = 0, 
				const void* data = nullptr, const EBufferCPUAccess access = EBufferCPUAccess::None);
		};
	};
	using BufferDescriptor = Buffer::Descriptor;
	using EBufferUsage = Buffer::EUsage;
	using BufferUsages = Buffer::Usages;

	struct Texture
	{
		using Format = ETextureFormat;
		
		// cn: 纹理的一般性用途, 用于辅助系统隐式地推导它的Barrier和DescriptorSet/View.
		// en：Gneral purpose of texture, can assist the system to implicitly derive its Barrier and DescriptorSet/View.
		// jp: テクスチャの一般的な目的は、システムがバリアとディスクリプタセット/ビューを暗黙的に導出するのを支援できます.
		enum EUsage 
		{
			None = 0,
			CopySrc = 0x00000001,
			CopyDst = 0x00000002,
			Sampled = 0x00000004,
			Storage = 0x00000008,
			Attachment = 0x00000010,
		};
		using Usages = uint32;

		// cn: 纹理的维度, 用于辅助系统创建默认的ImageView, 以及对内存布局做出优化.
		// en: Dimension of the texture, can assist the system to create default ImageView, and optimize memory layout.
		// jp: テクスチャのディメンション. システムがデフォルトのImageViewを作成し、メモリレイアウトを最適化するのに役立ちます.
		enum class EDimension
		{
			Texture1D = 0,
			Texture2D = 1,
			Texture3D = 2
		};

		
		// cn: 一些特殊杂项标记, 用于辅助系统打开可选特性或进行更多的隐式推导. 在一些平台上, 正确地设置这些flags会带来性能提升.
		// en: Special miscellaneous flags assisting system to open optional features or perform more implicit deductions.
		//     On some platforms, setting these flags correctly will bring performance improvements.
		// jp: いくつかの特別なその他のフラグ、システムがオプション機能をエネーブル、より暗黙的な派生を実行したりするのを支援する。
		//     一部のプラットフォームでは、これらのフラグを正しく設定すると、パフォーマンスが向上します。
		enum Flag 
		{
			SparseBinding = 0x00000001,
			SparseResidency = 0x00000002,
			SparseAliased = 0x00000004,
			MutableFormat = 0x00000008,
			Cube = 0x00000010,
			T2DArray = 0x00000020,
			BlockButBitCompatible = 0x00000080,
			Alias = 0x00000400,

			// Some custome flags, notice to avoid native API's bitflags.
			
			ForceLinear = 0x10000000
		};
		using Flags = uint32;

		// cn: Texture在一片内(显)存上的的DataLayout, 指示起始位置和行列排布.
		// en: DataLayout of Texture on a slice of (video) memory, indicating the start position and row/column arrangement.
		struct DataLayout
		{
			uint64_t offset = 0;
			uint32_t bytes_per_raw = 0;
			uint32_t rows_per_image = 0;
		};

		enum class EAspect
		{
			All,
			StencilOnly,
			DepthOnly
		};
		
		struct Slice
		{
			EAspect aspect = EAspect::All;
			uint32_t mip_level = 1;
			pos3d origin = { 0, 0, 0 };
		};
		
		struct Descriptor
		{
			// cn: 纹理的一般性用途, 用于辅助系统隐式地推导它的Barrier和DescriptorSet/View.
			// en：Gneral purpose of texture, can assist the system to implicitly derive its Barrier and DescriptorSet/View.
			// jp: テクスチャの一般的な目的は、システムがバリアとディスクリプタセット/ビューを暗黙的に導出するのを支援できます.
			Usages usages = 0;

			// cn: 纹理的维度, 用于辅助系统创建默认的ImageView, 以及对内存布局做出优化.
			// en: Dimension of the texture, can assist the system to create default ImageView, and optimize memory layout.
			// jp: テクスチャのディメンション. システムがデフォルトのImageViewを作成し、メモリレイアウトを最適化するのに役立ちます.
			EDimension dimension = EDimension::Texture2D;

			ETextureFormat format;
			uint32 mip_levels = 1;
			extent3d size;

			// Advanced Features

			// cn: 一些特殊杂项标记, 用于辅助系统打开可选特性或进行更多的隐式推导. 在一些平台上, 正确地设置这些flags会带来性能提升.
			// en: Special miscellaneous flags assisting system to open optional features or perform more implicit deductions.
			//     On some platforms, setting these flags correctly will bring performance improvements.
			// jp: いくつかの特別なその他のフラグ、システムがオプション機能をエネーブル、より暗黙的な派生を実行したりするのを支援する。
			//     一部のプラットフォームでは、これらのフラグを正しく設定すると、パフォーマンスが向上します。
			Flags flags = 0;
			uint32 sample_count = 1;
			uint32 array_layers = 1;

			// cn: 跨队列操作需要Resource具有ESharingMode::Concurrent属性.
			// en: Cross-queue operation requires Resource to have ESharingMode::Concurrent attribute.
			// jp: クロスキュー操作では、リソースにESharingMode :: Concurrent属性が必要です.
			ESharingMode sharing_mode = ESharingMode::Exclusive;
		};

	};
	using TextureDescriptor = Texture::Descriptor;
	using TextureUsages = uint32;
	using TextureFlags = uint32;
	using TextureDataLayout = Texture::DataLayout;
	using TextureSlice = Texture::Slice;
	using ETextureDimension = Texture::EDimension;
	using ETextureFlag = Texture::Flag;
	using ETextureUsage = Texture::EUsage;
	using ETextureAspect = Texture::EAspect;


	struct RenderGraphAPI Sampler
	{
		enum class EAddressMode : uint8
		{
			Repeat = 0,
			MirroredRepeat = 1,
			ClampToEdge = 2,
			ClampToBorder = 3,
			MirrorClampToEdge = 4
		};
		
		struct Descriptor
		{
			EAddressMode address_mode_u = EAddressMode::MirroredRepeat;
			EAddressMode address_mode_v = EAddressMode::MirroredRepeat;
			EAddressMode address_mode_w = EAddressMode::MirroredRepeat;
			bool anisotropy = true;
			float max_anisotropy = 1.f;
			float max_lod = 1000.f;
			float min_lod = 0.f;
			EFilter mag_filter = EFilter::Linear;
			EFilter min_filter = EFilter::Linear;
		};

	};
	using SamplerDescriptor = Sampler::Descriptor;
	using ESamplerAddressMode = Sampler::EAddressMode;


	
	struct RenderGraphAPI IGpuShader : public IGpuMemoryResource
	{
		virtual EShaderFrequency frequency() const = 0;
		
		virtual sakura::string_view entry_name() const = 0;
		
		virtual EShaderCodeFormat code_format() const = 0;
	};
	struct RenderGraphAPI IGpuBuffer : public IGpuMemoryResource
	{
		virtual ~IGpuBuffer() = default;
		virtual BufferUsages usages() const = 0;
	};
	struct RenderGraphAPI IGpuTexture : public IGpuMemoryResource
	{
		virtual uint32 width() const = 0;
		
		virtual uint32 height() const = 0;
		
		virtual uint32 depth() const = 0;
		
		virtual ETextureFormat render_format() const = 0;

		virtual RenderResourceHandle handle() const = 0;
		
		virtual TextureUsages usages() const = 0;
	};
	struct RenderGraphAPI IGpuSampler : public IGpuMemoryResource
	{
		
	};
	using GpuShaderHandle = TypedRenderResourceHandle<IGpuShader>;
	using GpuBufferHandle = TypedRenderResourceHandle<IGpuBuffer>;
	using GpuTextureHandle = TypedRenderResourceHandle<IGpuTexture>;
	using GpuSamplerHandle = TypedRenderResourceHandle<IGpuSampler>;

	struct RenderGraphAPI ISwapChain : public IGpuObject
	{
		virtual uint8 buffer_count() const = 0;
		virtual extent2d extent() const = 0;
		virtual sakura::Window window() const = 0;
		virtual ETextureFormat render_format() const = 0;
		virtual bool present() = 0;
		//virtual RenderAttachmentHandle back_buffer() const = 0;
	};
	using SwapChainHandle = TypedRenderObjectHandle<ISwapChain>;
	
	struct RenderGraphAPI IRenderPipeline : public IGpuObject
	{

	};
	using RenderPipelineHandle = TypedRenderObjectHandle<IRenderPipeline>;
	
	struct RenderGraphAPI IComputePipeline : public IGpuObject
	{

	};
	using ComputePipelineHandle = TypedRenderObjectHandle<IComputePipeline>;
	
	struct RenderGraphAPI IFence : public IGpuObject
	{
		//virtual void wait() = 0;
		
		//virtual bool completed() const = 0;
	};
	using FenceHandle = TypedRenderObjectHandle<IFence>;
	using QueueIndex = size_t;


	
	struct RenderGraphAPI ShaderLayout
	{
		const GpuShaderHandle* shaders = nullptr;
		uint32 count = 0;
		ShaderLayout() = default;
		template<size_t N>
		constexpr explicit ShaderLayout(const GpuShaderHandle(&shaders)[N]);
	};

	struct RenderGraphAPI VertexLayout
	{
		enum class Frequency
		{
			PerVertexData,
			PerInstanceData
		};
		struct RenderGraphAPI Element
		{
			Element() = default;
			Element(string semantic_name, EVertexFormat format, uint32 offset) noexcept;
			sakura::string semantic_name = "";
			EVertexFormat format = EVertexFormat::FLOAT;
			uint32 offset = 0;
		};
		sakura::vector<Element> elements;
		Frequency freq = Frequency::PerVertexData;
		uint32 stride = 0;
		VertexLayout() = default;
		template<size_t N>
		VertexLayout(
			const VertexLayout::Element(&vertex_elements)[N], const VertexLayout::Frequency frequecy, uint32 stride);
	};


	struct RenderGraphAPI BindingLayout
	{
		enum EType
		{
			UniformBuffer = 0x00000000,
			StorageBuffer = 0x00000001,
			ReadonlyStorageBuffer = 0x00000002,
			Sampler = 0x00000003,
			ComparisonSampler = 0x00000004,
			SampledTexture = 0x00000005,
			MultisampledTexture = 0x00000006,
			ReadonlyStorageTexture = 0x00000007,
			WriteonlyStorageTexture = 0x00000008,
			Count = WriteonlyStorageTexture + 2,
			Invalid = 0x7FFFFFFF
		};
		struct RenderGraphAPI Slot
		{
			Slot(uint32 binding, EType binding_type, EShaderFrequencys visibility, uint32 count = 1) noexcept;
			Slot() = default;
			uint32 binding = 0;
			EType binding_type = EType::UniformBuffer;
			EShaderFrequencys visibility = EShaderFrequency::Invalid;
			uint32 count = 1;
		};
		struct RenderGraphAPI Set
		{
			Set(const Slot* slots, uint32 slots_count) noexcept;
			Set() = default;
			template<size_t N>
			explicit constexpr Set(const Slot(&)[N]);
			const sakura::vector<Slot> slots;
		};
		BindingLayout() = default;
		template<size_t N>
		explicit constexpr BindingLayout(const Set(&)[N]);
		const sakura::vector<BindingLayout::Set> tables;
	};
	using BindingSlot = BindingLayout::Slot;
	using BindingTable = BindingLayout::Set;
	
	struct RenderGraphAPI AttachmentLayout
	{
		struct RenderGraphAPI Slot
		{
			ETextureFormat format; // size = 16
			bool blend_enable = false;
			ELoadOp load_operation = ELoadOp::Count;
			ELoadOp stencil_load_operation = ELoadOp::DontCare;
			EStoreOp store_operation = EStoreOp::Count;
			EStoreOp stencil_store_operation = EStoreOp::DontCare;
			struct BlendSetting
			{
				ELoadOp load_operation = ELoadOp::Count;
				EStoreOp store_opeartion = EStoreOp::Count;
				EBlendOp operation = EBlendOp::Add;
				EBlendFactor src_factor = EBlendFactor::SrcAlpha;
				EBlendFactor dst_factor = EBlendFactor::OneMinusSrcAlpha;
			} alpha_blend, color_blend; // size = 32 * 2
			ColorMask write_mask = EColorMaskBits::All; // size = 8
			Slot(ETextureFormat format, ELoadOp loadOp, EStoreOp storeOp,
				ELoadOp stencilLoadOp = ELoadOp::DontCare, EStoreOp stencilStoreOp = EStoreOp::DontCare) noexcept;
			Slot() = default;
		};
		AttachmentLayout() = default;
		template<size_t N>
		explicit constexpr AttachmentLayout(const Slot(&)[N]);
		const sakura::vector<Slot> slots;
	};
	using AttachmentSlot = AttachmentLayout::Slot;
	using BlendSetting = AttachmentSlot::BlendSetting;

	enum class ECompareFunction : uint8
	{
		Undefined = 0,
		Never = 1,
		Less = 2,
		LessEqual = 3,
		Greater = 4,
		GreaterEqual = 5,
		Equal = 6,
		NotEqual = 7,
		Always = 8
	};

	struct RenderGraphAPI DepthStencil
	{
		struct RenderGraphAPI Descriptor
		{
			ETextureFormat format = ETextureFormat::Count;
			bool depth_write = false;
			ECompareFunction depth_compare = ECompareFunction::Less;
			uint32_t stencil_read_mask = 0;
			uint32_t stencil_write_mask = 0;
			
			ECompareFunction stencil_front_compare = ECompareFunction::Never;
			EStencilOp front_fail_op = EStencilOp::Keep;
			EStencilOp front_depth_fail_op = EStencilOp::Keep;
			EStencilOp front_pass_op = EStencilOp::Keep;

			ECompareFunction stencil_back_compare = ECompareFunction::Never;
			EStencilOp back_fail_op = EStencilOp::Keep;
			EStencilOp back_depth_fail_op = EStencilOp::Keep;
			EStencilOp back_pass_op = EStencilOp::Keep;

			Descriptor() = default;
			Descriptor(const ETextureFormat format, bool depth_write,
				ECompareFunction depth_compare = ECompareFunction::Less,
				uint32_t stencil_read_mask = 0, uint32_t stencil_write_mask = 0);
		};

		GpuTextureHandle ds_attachment;
		float clear_depth = 1.f;
		ELoadOp depth_load_op = ELoadOp::Clear;
		EStoreOp depth_store_op = EStoreOp::Store;
		uint32 clear_stencil = 0;
		ELoadOp stencil_load_op = ELoadOp::Clear;
		EStoreOp stencil_store_op = EStoreOp::Store;
	};
	using DepthStencilDescriptor = DepthStencil::Descriptor;

	struct RenderGraphAPI FenceDescriptor
	{
		const uint64 initial_value = 0;
		
		FenceDescriptor() = default;
		FenceDescriptor(const uint64 initialValue) noexcept;
	};
	
	enum class EPresentMode : uint8
	{
		Immediate = 0,
		FIFO = 1,
		Mailbox = 2,
		Count = 3
	};

	struct RenderGraphAPI SwapChainDescriptor
	{
		const EPresentMode present_mode = EPresentMode::Mailbox;
		const Window window = sakura::Window();
		const uint8 buffer_count = 2;
		const uint8 sample_count = 4;

		SwapChainDescriptor() = default;
		SwapChainDescriptor(const EPresentMode presentMode, const Window window, const uint8 bufferCount, const uint8 sample_count) noexcept;
	};
	
	struct RenderPipeline
	{
		struct RenderGraphAPI Descriptor
		{
			EPrimitiveTopology primitive_topology = EPrimitiveTopology::TriangleList;
			uint8 sample_count = 1;
			uint32 sample_mask = 0xFFFFFFFF;

			ECullMode cull_mode = ECullMode::Back;
			EPolygonMode polygon_mode = EPolygonMode::FILL;
			float lineWidth = 1.f;

			ShaderLayout shader_layout;
			const sakura::vector<VertexLayout> vertex_layout;
			BindingLayout binding_layout;
			AttachmentLayout attachment_layout;
			DepthStencilDescriptor depth_stencil;

			Descriptor() = default;
			explicit Descriptor(
				const ShaderLayout& shader_layout, const VertexLayout& vertex_layout,
				const BindingLayout& binding_layout, const AttachmentLayout& attachment_layout,
				const DepthStencilDescriptor depth_stencil = {},
				const ECullMode cull_mode = ECullMode::Back,
				const EPrimitiveTopology primitive_topology = EPrimitiveTopology::TriangleList,
				const EPolygonMode polygon_mode = EPolygonMode::FILL,
				uint8 sample_count = 1, uint32 sample_mask = 0xFFFFFFFF);

			template<size_t N>
			explicit Descriptor(
				const ShaderLayout & shader_layout, const VertexLayout(&vertex_layouts)[N],
				const BindingLayout binding_layout, const AttachmentLayout& attachment_layout,
				const DepthStencilDescriptor depth_stencil = {},
				const ECullMode cull_mode = ECullMode::Back,
				const EPrimitiveTopology primitive_topology = EPrimitiveTopology::TriangleList,
				const EPolygonMode polygon_mode = EPolygonMode::FILL,
				uint8 sample_count = 1, uint32 sample_mask = 0xFFFFFFFF);
		};
	};
	using RenderPipelineDescriptor = RenderPipeline::Descriptor;
	
	struct ComputePipeline
	{
		struct RenderGraphAPI Descriptor
		{
			
		};
		
	};
	using ComputePipelineDescriptor = ComputePipeline::Descriptor;

	
	struct RenderGraphAPI Attachment
	{
		struct RenderGraphAPI Slot
		{
			Slot() = default;
			Slot(GpuTextureHandle attachmentHandle,
				double4 clearColor = double4(), ELoadOp loadOp = ELoadOp::Clear, EStoreOp storeOp = EStoreOp::Store)
				:attachment(attachmentHandle), load_op(loadOp), store_op(storeOp), clear_color(clearColor)
			{
				  
			}
			Slot(SwapChainHandle attachmentHandle,
				double4 clearColor = double4(), ELoadOp loadOp = ELoadOp::Clear, EStoreOp storeOp = EStoreOp::Store)
				:attachment(attachmentHandle), load_op(loadOp), store_op(storeOp), clear_color(clearColor)
			{
				  
			}
			sakura::variant<GpuTextureHandle, SwapChainHandle> attachment;
			ELoadOp load_op = ELoadOp::Clear;
			EStoreOp store_op = EStoreOp::Store;
			double4 clear_color = { 0, 0, 0, 0 };
		};
		sakura::vector<Slot> slots;
		
		template<size_t N>
		Attachment(const Slot(&slots)[N]) noexcept;
		Attachment() = default;
 	};


	struct RenderGraphAPI Binding
	{
		struct Buffer
		{
			uint32 size = 0;
			GpuBufferHandle buffer = RenderGraphId::UNINITIALIZED;

			FORCEINLINE bool operator==(const Buffer& other) const
			{
				return std::tie(size, buffer)
					== std::tie(other.size, other.buffer);
			}
		};

		struct Sampler
		{
			GpuSamplerHandle sampler = RenderGraphId::UNINITIALIZED;

			FORCEINLINE bool operator==(const Sampler& other) const
			{
				return std::tie(sampler) == std::tie(other.sampler);
			}
		};

		struct SampledTexture
		{
			GpuTextureHandle texture = RenderGraphId::UNINITIALIZED;
			bool follow_default = true;

			ETextureFormat format = ETextureFormat::Count;
			ETextureDimension dimension = ETextureDimension::Texture2D;
			ETextureAspect aspect = ETextureAspect::All;
			uint32_t base_mip_level = 0;
			uint32_t mip_level_count = 0;
			uint32_t base_array_layer = 0;
			uint32_t array_layer_count = 1;

			FORCEINLINE bool operator==(const SampledTexture& other) const
			{
				return std::tie(texture, follow_default, format, dimension, aspect,
					base_mip_level, mip_level_count, base_array_layer, array_layer_count) ==
					std::tie(other.texture, other.follow_default, other.format, other.dimension, other.aspect, other.base_mip_level, other.mip_level_count, other.base_array_layer, other.array_layer_count);
			}
		};

		FORCEINLINE auto as_buffer() const
		{
			return std::get_if<Buffer>(&content);
		}

		FORCEINLINE auto as_sampler() const
		{
			return std::get_if<Sampler>(&content);
		}

		FORCEINLINE auto as_sampled_texture() const
		{
			return std::get_if<SampledTexture>(&content);
		}

		FORCEINLINE uint32 set() const
		{
			return target_set;
		}

		FORCEINLINE uint32 bind() const
		{
			return target_binding;
		}

		FORCEINLINE uint32 offset() const
		{
			return offset_;
		}
		
		FORCEINLINE bool operator==(const Binding& other) const
		{
			return content == other.content && target_binding == other.target_binding 
				&& target_set == other.target_set && other.offset_ == offset_;
		}

		FORCEINLINE bool operator!=(const Binding& other) const
		{
			return !(*this == other);
		}

		FORCEINLINE void operator=(const Binding& other)
		{
			target_set = other.set();
			target_binding = other.bind();
			content = other.content;
			offset_ = other.offset();
		}
		Binding() = default;
		Binding(uint32_t set, uint32_t binding, GpuBufferHandle buffer = RenderGraphId::UNINITIALIZED,
			uint32 slot_index = 0, uint32 size = 0, uint32 offset = 0);
		Binding(uint32_t set, uint32_t binding, GpuTextureHandle texture);
		Binding(uint32_t set, uint32_t binding, GpuSamplerHandle texture);
	protected:
		uint32 offset_ = 0;
		uint32 target_set = 0;
		uint32 target_binding = 0;
		std::variant<Buffer, Sampler, SampledTexture> content = {};
	};

	// Shader Description struct
	struct RenderGraphAPI Shader
	{
		struct RenderGraphAPI Descriptor
		{
			sakura::string name = "";
			sakura::string entry = "";
			EShaderFrequency frequency = EShaderFrequency::Invalid;
			EShaderLanguage shader_language = EShaderLanguage::SPIRV;

			const sakura::span<const std::byte> code;
			const char* src = nullptr;

			Descriptor() = default;
			explicit Descriptor(const sakura::string & name,
				const sakura::string & entry, const EShaderFrequency frequency,
				const sakura::span<const std::byte> code = {}, const EShaderLanguage sl = EShaderLanguage::SPIRV) noexcept;
			
			explicit Descriptor(const sakura::string & name,
				const sakura::string & entry, const EShaderFrequency frequency,
				const char* src = nullptr, const EShaderLanguage sl = EShaderLanguage::HLSL) noexcept;
		};
	};
	using ShaderDescriptor = Shader::Descriptor;


	// cn: 渲染帧, 一般盛装有一帧之内的各种渲染状态.
	// en: Rendered frames generally contain various rendering states within one frame.
	// jp: レンダリングされたフレームには、通常、フレーム内にさまざまなレンダリング状態が含まれます。
	struct RenderGraphAPI IRenderFrame : public IFrame
	{
		virtual ~IRenderFrame() = default;
	};
}

#include "RenderGraph/RenderGraphCore.inl"





