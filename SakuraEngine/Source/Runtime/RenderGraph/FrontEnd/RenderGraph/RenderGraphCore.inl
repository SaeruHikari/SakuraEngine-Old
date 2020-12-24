namespace sakura::graphics
{
	FORCEINLINE bool operator==(const RenderObjectHandle& lhs, const RenderObjectHandle& rhs) {
		return size_t(lhs.id()) == size_t(rhs.id());
	}
	
	FORCEINLINE bool operator==(const RenderPassHandle& lhs, const RenderPassHandle& rhs) {
		return size_t(lhs.id()) == size_t(rhs.id());
	}
	
	FORCEINLINE bool operator==(const RenderResourceHandle& lhs, const RenderResourceHandle& rhs) {
		return size_t(lhs.id()) == size_t(rhs.id());
	}
	
	template<typename _ResourceType>
	FORCEINLINE TypedRenderResourceHandle<_ResourceType>::TypedRenderResourceHandle(const RenderGraphId id_)
		:RenderResourceHandle(id_)
	{

	}

	template<typename _ObjectType>
	FORCEINLINE TypedRenderObjectHandle<_ObjectType>::TypedRenderObjectHandle(const RenderGraphId id_)
		:RenderObjectHandle(id_)
	{

	}

	template <size_t N>
	FORCEINLINE constexpr ShaderLayout::ShaderLayout(const GpuShaderHandle(&_shaders)[N])
		:shaders(_shaders), count(N)
	{

	}

	template <size_t N>
	FORCEINLINE VertexLayout::VertexLayout(
		const VertexLayout::Element(&vertex_elements)[N], const VertexLayout::Frequency frequecy, uint32 _stride)
		:elements(vertex_elements, vertex_elements + N), freq(frequecy), stride(_stride)
	{

	}

	template <size_t N>
	FORCEINLINE constexpr BindingLayout::Set::Set(const Slot(&_slots)[N])
		:slots(_slots, _slots + N)
	{

	}
	template <size_t N>
	FORCEINLINE constexpr BindingLayout::BindingLayout(const Set(&_tables)[N])
		: tables(_tables, _tables + N)
	{

	}

	template <size_t N>
	FORCEINLINE constexpr AttachmentLayout::AttachmentLayout(const Slot(&attachments)[N])
		:slots(attachments, attachments + N)
	{

	}

	template <size_t N>
	FORCEINLINE RenderPipeline::Descriptor::Descriptor(
		const ShaderLayout& _shader_layout, const VertexLayout(&_vertex_layouts)[N],
		const BindingLayout _binding_layout, const AttachmentLayout& _attachment_layout,
		const ECullMode _cull_mode,
		const EPrimitiveTopology _primitive_topology, const EPolygonMode _polygon_mode,
		uint8 _sample_count, uint32 _sample_mask)
		: primitive_topology(_primitive_topology), sample_count(_sample_count), sample_mask(_sample_mask),
		shader_layout(_shader_layout), vertex_layout(_vertex_layouts, _vertex_layouts + N), binding_layout(_binding_layout),
		attachment_layout(_attachment_layout), polygon_mode(_polygon_mode), cull_mode(_cull_mode)
	{
		
	}
	
	template <size_t N>
	FORCEINLINE Attachment::Attachment(const Slot(&_slots)[N]) noexcept
		:slots(_slots, _slots + N)
	{

	}

	template <size_t N>
	FORCEINLINE Binding::Set::Set(const Slot(&_slots)[N]) noexcept
		: slots(_slots, _slots + N), dynamic_offsets(N, 0)
	{

	}

	template <typename I, size_t N>
	FORCEINLINE Binding::Set::Set(const Slot(&_slots)[N], const I(&offsets)[N]) noexcept
		:slots(_slots, _slots + N), dynamic_offsets(offsets, offsets + N)
	{

	}

	template <size_t N>
	FORCEINLINE Binding::Binding(const Set(&_sets)[N]) noexcept
		:sets(_sets, _sets + N)
	{

	}

	FORCEINLINE RenderObjectHandle::RenderObjectHandle(RenderGraphId id)
		: id_(id) {	}

	FORCEINLINE RenderObjectHandle::RenderObjectHandle()
		: id_(RenderGraphId::UNINITIALIZED) {	}

	FORCEINLINE RenderPassHandle::RenderPassHandle(RenderGraphId id)
		: id_(id) {	}

	FORCEINLINE RenderPassHandle::RenderPassHandle()
		: id_(RenderGraphId::UNINITIALIZED) {	}

	FORCEINLINE RenderResourceHandle::RenderResourceHandle(RenderGraphId id)
		: id_(id) {	}

	FORCEINLINE RenderResourceHandle::RenderResourceHandle()
		: id_(RenderGraphId::UNINITIALIZED) {	}

	FORCEINLINE bool isMeshPipelineStage(const EShaderFrequency freq)
	{
		return freq <= EShaderFrequency::MeshShader && freq >= EShaderFrequency::TaskShader;
	}

	FORCEINLINE bool isRayTracingStage(const EShaderFrequency freq)
	{
		return freq <= EShaderFrequency::RayGenerationShader && freq >= EShaderFrequency::AnyHitShader;
	}

	FORCEINLINE bool isRenderPipelineStage(const EShaderFrequency freq)
	{
		return freq <= EShaderFrequency::MeshShader && freq >= EShaderFrequency::VertexShader;
	}

	FORCEINLINE Binding::Slot::Slot(GpuBufferHandle _buffer,
		uint32 _slot_index, uint32 _size, uint32 _offset)
		: content(Binding::Slot::Buffer{ _slot_index, _size, _offset, _buffer }) {	}

	FORCEINLINE Binding::Slot::Slot(GpuTextureHandle texture, uint32 slot_index)
		: content(Binding::Slot::SampledTexture(texture, slot_index)) {		}


	FORCEINLINE Binding::Slot::Slot(GpuSamplerHandle sampler, uint32 slot_index)
		: content(Binding::Slot::Sampler{ slot_index, sampler }) {		}


	FORCEINLINE Shader::Descriptor::Descriptor(const sakura::string& _name, const sakura::string& _entry,
		const EShaderFrequency _frequency, const sakura::span<const std::byte> _code) noexcept
		: code(_code), name(_name), entry(_entry), frequency(_frequency) {	}

	FORCEINLINE VertexLayout::Element::Element(string _semantic_name, EVertexFormat _format, uint32 _offset) noexcept
		:semantic_name(_semantic_name), format(_format), offset(_offset) {	}

	FORCEINLINE BindingLayout::Slot::Slot(uint32 _binding, EType _binding_type, EShaderFrequency _visibility, uint32 _count) noexcept
		:binding(_binding), binding_type(_binding_type), visibility(_visibility), count(_count) {	}

	FORCEINLINE BindingLayout::Set::Set(const Slot* _slots, uint32 _slots_count) noexcept
		:slots(_slots, _slots + _slots_count) {	}

	FORCEINLINE AttachmentLayout::Slot::Slot(ETextureFormat _format, ELoadOp loadOp, EStoreOp storeOp,
		ELoadOp stencilLoadOp, EStoreOp stencilStoreOp) noexcept
		:format(_format), load_operation(loadOp), store_operation(storeOp),
		stencil_load_operation(stencilLoadOp), stencil_store_operation(stencilStoreOp) {	}

	FORCEINLINE FenceDescriptor::FenceDescriptor(const uint64 initialValue) noexcept
		: initial_value(initialValue) {	}

	FORCEINLINE SwapChainDescriptor::SwapChainDescriptor(const EPresentMode presentMode,
		const Window _window, const uint8 bufferCount) noexcept
		:present_mode(presentMode), window(_window), buffer_count(bufferCount) {	}

	FORCEINLINE RenderPipeline::Descriptor::Descriptor(const ShaderLayout& _shader_layout, const VertexLayout& _vertex_layout,
		const BindingLayout& _binding_layout, const AttachmentLayout& _attachment_layout,
		const ECullMode _cull_mode,
		const EPrimitiveTopology _primitive_topology, const EPolygonMode _polygon_mode, uint8 _sample_count, uint32 _sample_mask)
		: primitive_topology(_primitive_topology), sample_count(_sample_count), sample_mask(_sample_mask),
		shader_layout(_shader_layout), vertex_layout({ _vertex_layout }), binding_layout(_binding_layout),
		attachment_layout(_attachment_layout), polygon_mode(_polygon_mode), cull_mode(_cull_mode) {	}

	FORCEINLINE Buffer::Descriptor::Descriptor(BufferUsages _usage, size_t _length,
		const void* _data, const EBufferCPUAccess _access)
		: usages(_usage), access(_access), length(_length), data(_data) {	}
}


namespace sakura::graphics
{
	namespace detail
	{
		constexpr auto ShaderFrequencyNameLut = sakura::map_c<sakura::graphics::EShaderFrequency, sakura::string_view>(
			{
				{ sakura::graphics::EShaderFrequency::None, "None" },
				{ sakura::graphics::EShaderFrequency::VertexShader, "VertexShader" },
				{ sakura::graphics::EShaderFrequency::PixelShader, "PixelShader" },
				{ sakura::graphics::EShaderFrequency::ComputeShader, "ComputeShader" },

				{ sakura::graphics::EShaderFrequency::TaskShader, "TaskShader" },
				{ sakura::graphics::EShaderFrequency::MeshShader, "MeshShader" },

				{ sakura::graphics::EShaderFrequency::AnyHitShader, "AnyHitShader" },
				{ sakura::graphics::EShaderFrequency::CallableShader, "CallableShader" },
				{ sakura::graphics::EShaderFrequency::ClosestHitShader, "ClosestHitShader" },
				{ sakura::graphics::EShaderFrequency::IntersectionShader, "IntersectionShader" },
				{ sakura::graphics::EShaderFrequency::MissShader, "MissShader" },
				{ sakura::graphics::EShaderFrequency::RayGenerationShader, "RayGenerationShader" },
			}
		);
		static_assert(ShaderFrequencyNameLut.size() == static_cast<int>(EShaderFrequency::Count),
			"Found ShaderFrequency Not Registed Into LUT!");

		constexpr auto LoadOpNameLut = sakura::map_c<sakura::graphics::ELoadOp, sakura::string_view>(
			{
				{ sakura::graphics::ELoadOp::Clear, "Clear" },
				{ sakura::graphics::ELoadOp::Load, "Load" },
				{ sakura::graphics::ELoadOp::DontCare, "DontCare" },
			}
		);
		static_assert(LoadOpNameLut.size() == static_cast<int>(ELoadOp::Count),
			"Found LoadOp Not Registed Into LUT!");

		constexpr auto StoreOpNameLut = sakura::map_c<sakura::graphics::EStoreOp, sakura::string_view>(
			{
				{ sakura::graphics::EStoreOp::Clear, "Clear" },
				{ sakura::graphics::EStoreOp::Store, "Store" },
				{ sakura::graphics::EStoreOp::DontCare, "DontCare" },
			}
		);
		static_assert(StoreOpNameLut.size() == static_cast<int>(EStoreOp::Count),
			"Found StoreOp Not Registed Into LUT!");


		constexpr auto BindingTypeNameLut = sakura::map_c<sakura::graphics::BindingLayout::EType, sakura::string_view>(
			{
				{ sakura::graphics::BindingLayout::EType::Invalid, "Invalid" },
				{ sakura::graphics::BindingLayout::EType::UniformBuffer, "UniformBuffer" },
				{ sakura::graphics::BindingLayout::EType::StorageBuffer, "StorageBuffer" },
				{ sakura::graphics::BindingLayout::EType::ReadonlyStorageBuffer, "ReadonlyStorageBuffer" },

				{ sakura::graphics::BindingLayout::EType::Sampler, "Sampler" },
				{ sakura::graphics::BindingLayout::EType::ComparisonSampler, "ComparisonSampler" },

				{ sakura::graphics::BindingLayout::EType::SampledTexture, "SampledTexture" },
				{ sakura::graphics::BindingLayout::EType::MultisampledTexture, "MultisampledTexture" },
				{ sakura::graphics::BindingLayout::EType::ReadonlyStorageTexture, "ReadonlyStorageTexture" },
				{ sakura::graphics::BindingLayout::EType::WriteonlyStorageTexture, "WriteonlyStorageTexture" },
			}
		);
		static_assert(BindingTypeNameLut.size() == static_cast<int>(BindingLayout::EType::Count),
			"Found BindingType Not Registed Into LUT!");

		constexpr auto ShaderFormatNameLut = sakura::map_c<sakura::graphics::EShaderCodeFormat, sakura::string_view>(
			{
				{ sakura::graphics::EShaderCodeFormat::PSSL2, "PSSL2" },
				{ sakura::graphics::EShaderCodeFormat::DXBC, "DXBC" },
				{ sakura::graphics::EShaderCodeFormat::DXIL, "DXIL" },
				{ sakura::graphics::EShaderCodeFormat::GLSL, "GLSL" },
				{ sakura::graphics::EShaderCodeFormat::Spirv, "Spirv" },
			}
		);
		static_assert(ShaderFormatNameLut.size() == static_cast<int>(EShaderCodeFormat::Count),
			"Found ShaderFrequency Not Registed Into LUT!");

		constexpr auto BlendOpNameLut = sakura::map_c<sakura::graphics::EBlendOp, sakura::string_view>(
			{
				{ sakura::graphics::EBlendOp::Add, "Add" },
				{ sakura::graphics::EBlendOp::Subtract, "Subtract" },
				{ sakura::graphics::EBlendOp::ReverseSubtract, "ReverseSubtract" },
				{ sakura::graphics::EBlendOp::Min, "Min" },
				{ sakura::graphics::EBlendOp::Max, "Max" },
			}
		);
		constexpr auto BlendFactorNameLut = sakura::map_c<sakura::graphics::EBlendFactor, sakura::string_view>(
			{
				{ sakura::graphics::EBlendFactor::Zero, "Zero" },
				{ sakura::graphics::EBlendFactor::One, "One" },
				{ sakura::graphics::EBlendFactor::SrcColor, "SrcColor" },
				{ sakura::graphics::EBlendFactor::OneMinusSrcColor, "OneMinusSrcColor" },
				{ sakura::graphics::EBlendFactor::SrcAlpha, "SrcAlpha" },
				{ sakura::graphics::EBlendFactor::OneMinusSrcAlpha, "OneMinusSrcAlpha" },
				{ sakura::graphics::EBlendFactor::DstColor, "DstColor" },
				{ sakura::graphics::EBlendFactor::OneMinusDstColor, "OneMinusDstColor" },
				{ sakura::graphics::EBlendFactor::DstAlpha, "DstAlpha" },
				{ sakura::graphics::EBlendFactor::OneMinusDstAlpha, "OneMinusDstAlpha" },
				{ sakura::graphics::EBlendFactor::SrcAlphaSaturated, "SrcAlphaSaturated" },
				{ sakura::graphics::EBlendFactor::BlendColor, "BlendColor" },
				{ sakura::graphics::EBlendFactor::OneMinusBlendColor, "OneMinusBlendColor" },
			}
		);
		constexpr auto ColorMaskNameLut = sakura::map_c<sakura::graphics::EColorMaskBits, sakura::string_view>(
			{
				{ sakura::graphics::EColorMaskBits::None, "None" },
				{ sakura::graphics::EColorMaskBits::R, "R" },
				{ sakura::graphics::EColorMaskBits::G, "G" },
				{ sakura::graphics::EColorMaskBits::B, "B" },
				{ sakura::graphics::EColorMaskBits::A, "A" },
				{ sakura::graphics::EColorMaskBits::All, "All" },
			}
		);

	}
}

namespace std
{
	template<> struct hash<sakura::graphics::RenderResourceHandle>
	{
		sakura::size_t operator()(sakura::graphics::RenderResourceHandle handle) const
		{
			return std::hash<sakura::uint64>()(handle.id().storage());
		}
	};

	template<> struct hash<sakura::graphics::RenderObjectHandle>
	{
		sakura::size_t operator()(sakura::graphics::RenderObjectHandle handle) const
		{
			return std::hash<sakura::uint64>()(handle.id().storage());
		}
	};

	template<typename ResourceType> struct hash<sakura::graphics::TypedRenderResourceHandle<ResourceType>>
	{
		sakura::size_t operator()(sakura::graphics::TypedRenderResourceHandle<ResourceType> handle) const
		{
			return std::hash<sakura::uint64>()(handle.id().storage());
		}
	};

	template<typename ObjectType> struct hash<sakura::graphics::TypedRenderObjectHandle<ObjectType>>
	{
		sakura::size_t operator()(sakura::graphics::TypedRenderObjectHandle<ObjectType> handle) const
		{
			return std::hash<sakura::uint64>()(handle.id().storage());
		}
	};
}

namespace fmt
{
	template<> struct formatter<sakura::graphics::BindingLayout::EType> : fmt::formatter<sakura::string_view>
	{
		template<typename FormatContext>
		auto format(sakura::graphics::BindingLayout::EType type, FormatContext& ctx)
		{
			using namespace sakura::graphics;
			using namespace sakura::graphics::detail;
			sakura::string_view bindingTypeName = "Binding::EType:Undefined";
			if (BindingTypeNameLut.find(type) != BindingTypeNameLut.end())
				bindingTypeName = BindingTypeNameLut.find(type)->second;
			return fmt::formatter<sakura::string_view>::format(bindingTypeName, ctx);
		}
	};
	template<> struct formatter<sakura::graphics::EShaderCodeFormat> : fmt::formatter<sakura::string_view>
	{
		template<typename FormatContext>
		auto format(sakura::graphics::EShaderCodeFormat fmt, FormatContext& ctx)
		{
			using namespace sakura::graphics;
			using namespace sakura::graphics::detail;
			sakura::string_view fmtName = "ShaderCodeFormat:Undefined";
			if (ShaderFormatNameLut.find(fmt) != ShaderFormatNameLut.end())
				fmtName = ShaderFormatNameLut.find(fmt)->second;
			return fmt::formatter<sakura::string_view>::format(fmtName, ctx);
		}
	};
	template<> struct formatter<sakura::graphics::EShaderFrequency> : fmt::formatter<sakura::string_view>
	{
		template<typename FormatContext>
		auto format(sakura::graphics::EShaderFrequency freq, FormatContext& ctx)
		{
			using namespace sakura::graphics;
			using namespace sakura::graphics::detail;
			sakura::string_view freqName = "ShaderFrequency:Undefined";
			if (ShaderFrequencyNameLut.find(freq) != ShaderFrequencyNameLut.end())
				freqName = ShaderFrequencyNameLut.find(freq)->second;
			return fmt::formatter<sakura::string_view>::format(freqName, ctx);
		}
	};
	template<> struct formatter<sakura::graphics::EBlendOp> : fmt::formatter<sakura::string_view>
	{
		template<typename FormatContext>
		auto format(sakura::graphics::EBlendOp op, FormatContext& ctx)
		{
			using namespace sakura::graphics;
			using namespace sakura::graphics::detail;
			sakura::string_view opName = "BlendOp:Undefined";
			if (BlendOpNameLut.find(op) != BlendOpNameLut.end())
				opName = BlendOpNameLut.find(op)->second;
			return fmt::formatter<sakura::string_view>::format(opName, ctx);
		}
	};
	template<> struct formatter<sakura::graphics::EBlendFactor> : fmt::formatter<sakura::string_view>
	{
		template<typename FormatContext>
		auto format(sakura::graphics::EBlendFactor factor, FormatContext& ctx)
		{
			using namespace sakura::graphics;
			using namespace sakura::graphics::detail;
			sakura::string_view factorName = "EBlendFactor:Undefined";
			if (BlendFactorNameLut.find(factor) != BlendFactorNameLut.end())
				factorName = BlendFactorNameLut.find(factor)->second;
			return fmt::formatter<sakura::string_view>::format(factorName, ctx);
		}
	};
	template<> struct formatter<sakura::graphics::EColorMaskBits> : fmt::formatter<sakura::string_view>
	{
		template<typename FormatContext>
		auto format(sakura::graphics::EColorMaskBits mask, FormatContext& ctx)
		{
			using namespace sakura::graphics;
			using namespace sakura::graphics::detail;
			sakura::string_view maskName = "EColorMaskBits:Undefined";
			if (ColorMaskNameLut.find(mask) != ColorMaskNameLut.end())
				maskName = ColorMaskNameLut.find(mask)->second;
			return fmt::formatter<sakura::string_view>::format(maskName, ctx);
		}
	};

	template<> struct formatter<sakura::graphics::RenderResourceHandle> : fmt::formatter<sakura::graphics::RenderGraphId>
	{
		template<typename FormatContext>
		auto format(sakura::graphics::RenderResourceHandle handle, FormatContext& ctx)
		{
			return fmt::formatter< sakura::graphics::RenderGraphId>::format(handle.id(), ctx);
		}
	};
	template<> struct formatter<sakura::graphics::RenderObjectHandle> : fmt::formatter<sakura::graphics::RenderGraphId>
	{
		template<typename FormatContext>
		auto format(sakura::graphics::RenderObjectHandle handle, FormatContext& ctx)
		{
			return fmt::formatter< sakura::graphics::RenderGraphId>::format(handle.id(), ctx);
		}
	};

	template<typename ResourceType> struct formatter<sakura::graphics::TypedRenderResourceHandle<ResourceType>>
		: fmt::formatter<sakura::graphics::RenderGraphId>
	{
		template<typename FormatContext>
		auto format(sakura::graphics::TypedRenderResourceHandle<ResourceType> handle, FormatContext& ctx)
		{
			return fmt::formatter< sakura::graphics::RenderGraphId>::format(handle.id(), ctx);
		}
	};
		template<typename ObjectType> struct formatter<sakura::graphics::TypedRenderObjectHandle<ObjectType>>
			: fmt::formatter<sakura::graphics::RenderGraphId>
		{
			template<typename FormatContext>
			auto format(sakura::graphics::TypedRenderObjectHandle<ObjectType> handle, FormatContext& ctx)
			{
				return fmt::formatter< sakura::graphics::RenderGraphId>::format(handle.id(), ctx);
			}
		};


			template<> struct formatter<sakura::graphics::ShaderDescriptor> : fmt::formatter<sakura::vfs::path>
			{
				template<typename FormatContext>
				auto format(sakura::graphics::ShaderDescriptor desc, FormatContext& ctx)
				{
					using namespace sakura::graphics::detail;
					constexpr sakura::string_view pathTitle = "Shader Name";
					constexpr sakura::string_view freqTitle = "Shader Frequency";
					constexpr sakura::string_view entryTitle = "Entry Point";
					constexpr size_t widthLeft
						= sakura::max(pathTitle.size(), freqTitle.size(), entryTitle.size());
					size_t widthRight = sakura::max(
						desc.name.size(), desc.entry.size(),
						ShaderFrequencyNameLut.find(desc.frequency)->second.size()
					);
					return fmt::format_to(
						ctx.out(),
						"***{4:*^{2}}***\n"
						"* {5: <{0}}   {6: >{1}} *\n"
						"* {7: <{0}}   {8: >{1}} *\n"
						"* {9: <{0}}   {10: >{1}} *\n"
						"***{3:*^{2}}***\n",
						widthLeft, widthRight, widthRight + widthLeft + 1,
						"", //3 : none
						" ShaderDescriptor ",//4 : TotalTitle
						pathTitle, desc.name, //5, 6
						freqTitle, desc.frequency,//7, 8
						entryTitle, desc.entry//9, 10
					);
				}
			};

			template<> struct formatter<sakura::graphics::TextureDescriptor> : fmt::formatter<sakura::string_view>
			{
				template<typename FormatContext>
				auto format(sakura::graphics::TextureDescriptor desc, FormatContext& ctx)
				{
					constexpr sakura::string_view widthTitle = "Texture Width";
					constexpr sakura::string_view heightTitle = "Texture Height";
					constexpr sakura::string_view depthTitle = "Texture Depth";
					constexpr int widthLeft = sakura::max(widthTitle.size(), heightTitle.size(), depthTitle.size());
					int widthRight = 30;
					return fmt::format_to(
						ctx.out(),
						"***{4:*^{2}}***\n"
						"* {5: <{0}}   {6: >{1}} *\n"
						"* {7: <{0}}   {8: >{1}} *\n"
						"* {9: <{0}}   {10: >{1}} *\n"
						"***{3:*^{2}}***\n",
						widthLeft, widthRight, widthRight + widthLeft + 1,
						"", //3 : none
						" Texture Desc ",//4 : TotalTitle
						widthTitle, desc.size.width,//5, 6
						heightTitle, desc.size.height, //7,8
						depthTitle, desc.size.depth //9,10
					);
				}
			};
			//RenderAttachmentDesc
			template<> struct formatter<sakura::graphics::Attachment::Slot> : fmt::formatter<sakura::graphics::TextureDescriptor>
			{
				template<typename FormatContext>
				auto format(sakura::graphics::Attachment::Slot desc, FormatContext& ctx)
				{
					constexpr sakura::string_view widthTitle = " Attachment Desc ";
					constexpr int widthLeft = sakura::max(widthTitle.size(), size_t(30));
					int widthRight = 30;
					return fmt::format_to(
						ctx.out(),
						"***{4:*^{2}}***\n"
						"{5: ^{2}}\n",
						widthLeft, widthRight, widthRight + widthLeft + 1,
						"", //3 : none
						widthTitle, 0//4: RTTitle, 5: TexDesc
					);
				}
			};
			template<> struct formatter<sakura::graphics::Attachment> : fmt::formatter<sakura::graphics::TextureDescriptor>
			{
				template<typename FormatContext>
				auto format(sakura::graphics::Attachment desc, FormatContext& ctx)
				{
					constexpr sakura::string_view totalTitle = " Attachment Desc ";
					constexpr size_t widthLeft = sakura::max(size_t(0), size_t(10));
					size_t widthRight = 10;
					return fmt::format_to(
						ctx.out(),
						"***{4:*^{2}}***\n"
						,
						widthLeft, widthRight, widthRight + widthLeft + 1,
						"", //3 : none
						totalTitle//4: RTTitle
					);
				}
			};

}