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
		devices_.emplace_back(device);
		return;
	}

	RenderPass* RenderGraph::render_pass(const RenderPassHandle handle)
	{
		return passes.at(handle.id().index()).first;
	}

	RenderGraph::Builder& RenderGraph::builder(const RenderPassHandle handle)
	{
		return passes.at(handle.id().index()).second;
	}

	const RenderResourceHandle RenderGraph::query(const sakura::string& name) const
	{
		auto res = named_handles_.find(name);
		if (res != named_handles_.end())
			return res->second;
		return RenderResourceHandle();
	}

    RenderPass::~RenderPass()
    {
		
    }

    RenderPass::RenderPass(const RenderPassHandle __handle, uint8 cycleCount)
		: handle_(__handle)
	{

	}

	RenderPassLambda::RenderPassLambda(const RenderPassHandle __handle, const Constructor& _constructor)
		:RenderPass(__handle), constructor_(_constructor)
	{

	}

	bool RenderPassLambda::construct(RenderGraph::Builder& builder, IRenderDevice& device) noexcept
	{
		evaluator_ = constructor_(builder, device);
		return true;
	}

	const RenderCommandBuffer& RenderPassLambda::execute(RenderCommandBuffer& command_buffer, 
		const RenderGraph& rg, IRenderDevice& device) noexcept
	{
		return evaluator_(device, rg, command_buffer);
	}

	RenderGraphAPI IRenderDevice* RenderGraph::get_device(const sakura::string_view name) const
	{
		for(auto i = 0u; i < devices_.size(); i++)
		{
			if (name == devices_[i]->get_name())
				return devices_[i];
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

	void RenderGraph::Builder::pipeline(const RenderPipelineHandle& ppl)
	{
		_pipeline = ppl;
	}

	bool RenderGraph::Builder::apply()
	{
		return true;
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
		:devices_()
	{
		
	}

	RenderGraph::~RenderGraph()
	{
		for (size_t i = 0u; i < passes.size(); i++)
		{
			delete passes.at(i).first;
		}
		for (auto i = 0u; i < devices_.size(); i++)
		{
			delete devices_[i];
		}
	}
	
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
