/*
 * @Author: your name
 * @Date: 2020-07-30 14:30:13
 * @LastEditTime: 2020-08-21 20:09:48
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \SakuraEngineV3\SakuraEngine\Source\Runtime\RenderGraph\private\RenderGraph.cpp
 */ 
#include <RenderGraph/RenderGraph.h>
#include "System/Log.h"


using namespace sakura::graphics;

namespace sakura::graphics
{
	RenderGraphAPI void RenderGraph::emplace_device(IRenderDevice* device)
	{
		if (get_device(device->get_name()))
		{
			auto err = string("a device named ").append(device->get_name().data()).append("already exists!");
			assert(0 && err.c_str());
		}
		devices.emplace(device);
		return;
	}

	RenderPass* RenderGraph::render_pass(const RenderPassHandle handle)
	{
		return passes[handle.id().index()].first;
	}

	RenderGraph::Builder& RenderGraph::builder(const RenderPassHandle handle)
	{
		return passes[handle.id().index()].second;
	}

	const RenderResourceHandle RenderGraph::blackboard(const sakura::string& name) const
	{
		auto res = _blackboard.find(name);
		if (res != _blackboard.end())
			return res->second;
		return RenderResourceHandle();
	}

    RenderPass::~RenderPass()
    {
		
    }

    bool RenderPass::reset()
    {
		command_buffer().reset();
		return true;
    }

    RenderPass::RenderPass(const RenderPassHandle __handle, uint8 cycleCount, size_t bufferSize)
		: handle_(__handle), command_buffers_("", bufferSize)
	{

	}

	RenderPassLambda::RenderPassLambda(const RenderPassHandle __handle, const Constructor& _constructor)
		:RenderPass(__handle), constructor_(_constructor)
	{

	}

	bool RenderPassLambda::construct(RenderGraph::Builder& builder) noexcept
	{
		evaluator_ = constructor_(builder);
		return true;
	}

	const RenderCommandBuffer& RenderPassLambda::execute(const RenderGraph& rg,
		const RenderGraph::Builder& builder, IRenderDevice& device) noexcept
	{
		return evaluator_(device, rg, command_buffer());
	}

	RenderGraphAPI IRenderDevice* RenderGraph::get_device(const sakura::string_view name) const
	{
		for(auto i = 0u; i < devices.count(); i++)
		{
			if (name == devices[i]->get_name())
				return devices[i];
		}
		return nullptr;
	}

	RenderGraph::Builder::Builder(const uint32 _generation)
		:generation(_generation)
	{
	}

	void RenderGraph::Builder::write(const Attachment& attachment)
	{
		_attachment = attachment;
	}

	void RenderGraph::Builder::write(const RenderPipelineHandle& ppl)
	{
		_pipeline = ppl;
	}

	RenderPipelineHandle RenderGraph::Builder::pipeline() const
	{
		return _pipeline;
	}

	Attachment RenderGraph::Builder::attachment() const
	{
		return _attachment;
	}

	RenderGraph::RenderGraph()
		:devices(*this)
	{
	}

	RenderGraph::~RenderGraph()
	{
		for (size_t i = 0u; i < passes.size(); i++)
		{
			delete passes[i].first;
		}
		for (auto i = 0u; i < devices.count(); i++)
		{
			delete devices[i];
		}
	}


	RenderGraphAPI RenderGraphHandle::RenderGraphHandle(RenderGraphId id)
		:_id(id) {	}

	RenderGraphAPI RenderGraphHandle::RenderGraphHandle()
		: _id(RenderGraphId::UNINITIALIZED) {	}
	
	RenderGraphAPI RenderPassHandle::RenderPassHandle(RenderGraphId id)
		: _id(id) {	}

	RenderGraphAPI RenderPassHandle::RenderPassHandle()
		: _id(RenderGraphId::UNINITIALIZED) {	}

	RenderGraphAPI RenderResourceHandle::RenderResourceHandle(RenderGraphId id)
		: _id(id)	{	}

	RenderGraphAPI RenderResourceHandle::RenderResourceHandle()
		: _id(RenderGraphId::UNINITIALIZED) {	}

	bool isMeshPipelineStage(const EShaderFrequency freq)
	{
		return freq <= EShaderFrequency::MeshShader && freq >= EShaderFrequency::TaskShader;
	}

	bool isRayTracingStage(const EShaderFrequency freq)
	{
		return freq <= EShaderFrequency::RayGenerationShader && freq >= EShaderFrequency::AnyHitShader;
	}

	bool isRenderPipelineStage(const EShaderFrequency freq)
	{
		return freq <= EShaderFrequency::MeshShader && freq >= EShaderFrequency::VertexShader;
	}

	Binding::Slot::Slot(RenderBufferHandle _buffer, 
		uint32 _slot_index, uint32 _size, uint32 _offset)
		:slot_index(_slot_index), size(_size), offset(_offset), buffer(_buffer)	{	}

	ShaderDesc::ShaderDesc(const sakura::string& _name, const sakura::string& _entry,
		const EShaderFrequency _frequency, const sakura::span<const std::byte> _code) noexcept
		:code(_code), name(_name), entry(_entry), frequency(_frequency)	{	}

	VertexLayout::Element::Element(string _semantic_name, EVertexFormat _format, uint32 _offset) noexcept
		:semantic_name(_semantic_name), format(_format), offset(_offset) {	}

	BindingLayout::Slot::Slot(uint32 _binding, EType _binding_type, EShaderFrequency _visibility, uint32 _count) noexcept
		:binding(_binding), binding_type(_binding_type), visibility(_visibility), count(_count) {	}

	BindingLayout::Set::Set(const Slot* _slots, uint32 _slots_count) noexcept
		:slots(_slots, _slots + _slots_count) {	}

	AttachmentLayout::Slot::Slot(ETextureFormat _format) noexcept
		:format(_format) {	}

	FenceDesc::FenceDesc(const uint64 initialValue) noexcept
		: initial_value(initialValue) {	}
	
	SwapChainDesc::SwapChainDesc(const EPresentMode presentMode,
		const Window _window, const uint8 bufferCount) noexcept
		:present_mode(presentMode), window(_window), buffer_count(bufferCount) {	}

	RenderPipelineDesc::RenderPipelineDesc(const ShaderLayout& _shader_layout, const VertexLayout& _vertex_layout,
		const BindingLayout& _binding_layout, const AttachmentLayout& _attachment_layout,
		EPrimitiveTopology _primitive_topology, uint8 _sample_count, uint32 _sample_mask)
		: primitive_topology(_primitive_topology), sample_count(_sample_count), sample_mask(_sample_mask),
		shader_layout(_shader_layout), vertex_layout({ _vertex_layout }), binding_layout(_binding_layout),
			attachment_layout(_attachment_layout) {	}

	BufferDesc::BufferDesc(EBufferUsage _usage, size_t _length,
		const void* _data, const EBufferCPUAccess _access, EBufferOptions _options)
		:usage(_usage), options(_options), access(_access), length(_length), data(_data) {	}


	
	bool RenderGraphModule::render_graph_module_registered_
		= IModule::Registry::regist("RenderGraph", &RenderGraphModule::create);
	RenderGraphModule * RenderGraphModule::create()
	{
		return new RenderGraphModule();
	}
	bool RenderGraphModule::StartUp()
	{
		return true;
	}

	bool RenderGraphModule::Terminate()
	{
		return true;
	}
}
