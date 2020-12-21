/*
 * @Author: your name
 * @Date: 2020-07-30 14:20:24
 * @LastEditTime: 2020-08-21 20:02:16
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \SakuraEngineV3\SakuraEngine\Runtime\RenderGraph\public\RenderGraph.h
 */ 
#pragma once
#include "IRenderDevice.h" 
#include "Module.h"
#include "RenderGraphCore.h"
#include "RuntimeCore/Resource.h"

namespace sakura::graphics
{
    class render_graph_texture;
    class RGHeap;
}

#define constructor_rg_resource_handle(element, funcname)\
[[nodiscard]] element##Handle element##Unsafe()\
{\
	memory_resources_.emplace_back(RenderGraphId::uhalf::max/*generation*/);\
	return element##Handle({RenderGraphId::uhalf::max/*generation*/,\
	    static_cast<RenderGraphId::uhalf_t>(memory_resources_.size())});\
}\
template<typename... Args>\
[[nodiscard]] element##Handle element##Unsafe(const sakura::string& name, Args&&... args)\
{\
	std::forward<Args...>(args...);\
	auto hdl = element##Unsafe();\
	named_handles_.insert_or_assign(name, hdl);\
	return hdl;\
}\
[[nodiscard]] element##Handle element##Unsafe(const sakura::string& name)\
{\
	auto hdl = element##Unsafe();\
	named_handles_.insert_or_assign(name, hdl);\
	return hdl;\
}\
[[nodiscard]] element##Handle element()\
{\
	memory_resources_.emplace_back(0/*generation*/);\
	return element##Handle({0/*generation*/,\
	static_cast<RenderGraphId::uhalf_t>(memory_resources_.size())});\
}\
template<typename... Args>\
[[nodiscard]] element##Handle element(const sakura::string& name, Args&&... args)\
{\
	std::forward<Args...>(args...);\
	auto hdl = element();\
	named_handles_.insert_or_assign(name, hdl);\
	return hdl;\
}\
[[nodiscard]] element##Handle element(const sakura::string& name)\
{\
	auto hdl = element();\
	named_handles_.insert_or_assign(name, hdl);\
	return hdl;\
}




#define constructor_rg_object_handle(element, funcname)\
[[nodiscard]] element##Handle element()\
{\
	gpu_objects_.emplace_back(0/*generation*/);\
	return element##Handle({0/*generation*/,\
	    static_cast<RenderGraphId::uhalf_t>(gpu_objects_.size())});\
}\
template<typename... Args>\
[[nodiscard]] element##Handle element(Args&&... args)\
{\
    auto hdl = element();\
	void(std::forward<Args...>(args...));\
	return hdl;\
}

namespace sakura::graphics
{
    struct RenderPass;
	
    class RenderGraphAPI RenderGraph
    {
    public:
        struct RenderGraphAPI Builder
        {
            friend class RenderGraph;
            friend struct RenderPass;
        	
            Builder(const uint32 generation);
        	
            void write(const Attachment& attachment);
            void pipeline(const RenderPipelineHandle& ppl);

            bool apply();
        	
            RenderPipelineHandle pipeline() const;
            Attachment attachment() const;
        protected:
            Attachment _attachment;
            RenderPipelineHandle _pipeline;
            const uint32 generation = 0;
        };
    	
        struct RenderGraphAPI Blackboard
        {
        	
        };
    	
		RenderGraph();
        ~RenderGraph();
		[[nodiscard]] IRenderDevice* get_device(const sakura::string_view name) const;
        void emplace_device(IRenderDevice* device);

        constructor_rg_resource_handle(RenderShader, shader)
        constructor_rg_resource_handle(RenderBuffer, buffer)
        constructor_rg_resource_handle(RenderTexture, texture)
        constructor_rg_resource_handle(RenderAttachment, render_attachment)

        constructor_rg_object_handle(RenderPipeline, render_pipeline)
        constructor_rg_object_handle(SwapChain, swap_chain)
        constructor_rg_object_handle(Fence, fence)

    	template<typename... Args>
        RenderPassHandle create_render_pass(Args&&... args);
        template<typename T, typename... Args>
        RenderPassHandle create_render_pass(Args&&... args);
    	
        RenderPass* render_pass(const RenderPassHandle handle);
        RenderGraph::Builder& builder(const RenderPassHandle handle);

        const RenderResourceHandle query(const sakura::string& name) const;
    	template<typename T>
        const T query(const sakura::string& name) const
    	{
            static_assert(std::is_base_of_v<RenderResourceHandle, T>, "T is not derived from RenderResourceHandle!");
            auto& hdl = query(name);
            return *static_cast<const T*>(&hdl);
    	}

    private:
        RenderDeviceGroupProxy devices_;
        sakura::unordered_map<sakura::string, RenderResourceHandle> named_handles_;
    	sakura::vector<uint32> memory_resources_;/*generations*/
        sakura::vector<uint32> gpu_objects_;/*generations*/
        sakura::vector<sakura::pair<RenderPass*, Builder>> passes;
    };

    struct RenderGraphAPI RenderPass
    {
	    virtual ~RenderPass();
	    virtual const RenderCommandBuffer& execute(RenderCommandBuffer& command_buffer,
            const RenderGraph& rg, IRenderDevice& device) noexcept = 0;
        virtual bool construct(RenderGraph::Builder& rg) noexcept = 0;
        RenderPassHandle handle() const
        {
            return handle_;
        }
    protected:
        RenderPass(const RenderPassHandle __handle, uint8 cycleCount = 3);
        RenderPass() = delete;
        const RenderPassHandle handle_;
    };

    struct RenderGraphAPI RenderPassLambda final : public RenderPass
    {
        using Evaluator = sakura::function<const RenderCommandBuffer&(IRenderDevice& device,
            const RenderGraph& rg, RenderCommandBuffer& buffer)>;
        using Constructor = sakura::function<Evaluator(RenderGraph::Builder& builder)>;
        RenderPassLambda(const RenderPassHandle __handle, const Constructor& constructor);

        bool construct(RenderGraph::Builder& builder) noexcept override;
        const RenderCommandBuffer& execute(RenderCommandBuffer& command_buffer, const RenderGraph& rg, 
            IRenderDevice& device) noexcept override;
    protected:
        Constructor constructor_;
        mutable Evaluator evaluator_;
    };

    template <typename ... Args>
    RenderPassHandle RenderGraph::create_render_pass(Args&&... args)
    {
        auto newBuilder = RenderGraph::Builder(0);
        auto newHandle = RenderPassHandle(
            RenderGraphId(newBuilder.generation,passes.size())
        );
        passes.emplace_back(sakura::make_pair(
            new RenderPassLambda(newHandle, std::forward<Args>(args)...),
            newBuilder
        ));
        return newHandle;
    }

    template <typename T, typename ... Args>
    RenderPassHandle RenderGraph::create_render_pass(Args&&... args)
    {
        auto newBuilder = RenderGraph::Builder(0);
        auto newHandle = RenderPassHandle(
            RenderGraphId(newBuilder.generation, static_cast<uint32>(passes.size()))
        );
        passes.emplace_back(sakura::make_pair(
            new T(newHandle, std::forward<Args>(args)...),
            newBuilder
        ));
        return newHandle;
    }
	
    class RenderGraphAPI RenderGraphModule : public IModule
    {
    public:
        static RenderGraphModule* create();
	    bool StartUp() override;
	    bool Terminate() override;
    protected:
        static bool render_graph_module_registered_;
    };
}
