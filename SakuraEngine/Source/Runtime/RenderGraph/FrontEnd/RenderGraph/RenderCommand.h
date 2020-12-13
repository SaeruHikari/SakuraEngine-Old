#pragma once
#include "RenderGraph/RenderGraphCore.h"

namespace sakura::graphics
{
    enum class ERenderCommandType : uint32_t
    {
        set_scissor_rect,
        draw,
        draw_indirect,

        dispatch,
        dispatch_indirect,

        update_binding,

        copy_buffer,
        copy_texture,

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

    enum class ERenderQueueType : uint8_t
    {
        None = 0x00,
        Copy = 0x01,
        Compute = 0x02,
        Graphics = 0x04,
        All = Copy | Compute | Graphics
    };

    struct RenderCommand
    {
        virtual ~RenderCommand() = default;
        virtual ERenderCommandType type() const = 0;
    };

    template<ERenderCommandType CommandType, ERenderQueueType QueueType>
    struct RenderCommandTyped : public RenderCommand
    {
        static const ERenderCommandType command_type = CommandType;
        static const ERenderQueueType queue_type = QueueType;
        ERenderCommandType type() const final override { return CommandType; }
        RenderCommandTyped() { }
    };

    struct RenderCommandDispatch
        : public RenderCommandTyped<ERenderCommandType::dispatch, ERenderQueueType::Compute>
    {

    };

    struct RenderCommandBeginRenderPass final
        : public RenderCommandTyped<ERenderCommandType::begin_render_pass, ERenderQueueType::Graphics>
    {
        explicit RenderCommandBeginRenderPass(
            RenderPipelineHandle pipeline, const Attachment& attachment);
        RenderPipelineHandle pipeline = RenderGraphId::UNINITIALIZED;
        Attachment attachments;
    };

    inline RenderCommandBeginRenderPass::RenderCommandBeginRenderPass(
        RenderPipelineHandle ppl, const Attachment& att)
        :pipeline(ppl), attachments(att)
    {

    }

    struct RenderCommandEndRenderPass final
        : public RenderCommandTyped<ERenderCommandType::end_render_pass, ERenderQueueType::Graphics>
    {
        RenderCommandEndRenderPass() = default;
    };

    struct RenderCommandUpdateBinding final
        : public RenderCommandTyped<ERenderCommandType::update_binding, ERenderQueueType::Graphics>
    {
        RenderCommandUpdateBinding(const Binding& binding) noexcept;

        sakura::variant<Binding> binder = Binding();
    };

    inline RenderCommandUpdateBinding::RenderCommandUpdateBinding(const Binding& binding) noexcept
        :binder(binding)
    {

    }

    struct RenderCommandFence final
        : public RenderCommandTyped<ERenderCommandType::fence, ERenderQueueType::All>
    {
    	
    };

    struct RenderCommandSetScissorRect final
        : public RenderCommandTyped<ERenderCommandType::set_scissor_rect, ERenderQueueType::Graphics>
    {
        RenderCommandSetScissorRect(uint32 x, uint32 y, uint32 width, uint32 height) noexcept;

        uint32 x;
        uint32 y;
        uint32 width;
        uint32 height;
    };

    inline RenderCommandSetScissorRect::RenderCommandSetScissorRect(
        uint32 _x, uint32 _y, uint32 _width, uint32 _height) noexcept
        :x(_x), y(_y), width(_width), height(_height)
    {

    }

    struct RenderCommandDraw final
        : public RenderCommandTyped<ERenderCommandType::draw, ERenderQueueType::Graphics>
    {
        struct VB
        {
            size_t offset = 0;
            size_t stride = 0;
            RenderBufferHandle vertex_buffer;
            VB() = default;
            explicit VB(RenderBufferHandle vertex_buffer, size_t offset = 0, size_t stride = 0);
        };
        const sakura::vector<VB> vbs;
        struct IB
        {
            size_t offset = 0;
            size_t stride = 0;
            EIndexFormat format = EIndexFormat::UINT16;
            RenderBufferHandle index_buffer;
            size_t index_count = 0;
            IB() = default;
            explicit IB(RenderBufferHandle index_buffer, size_t indexCount,
                EIndexFormat format = EIndexFormat::UINT16, size_t offset = 0, size_t stride = 0);
        }ib;
        uint32 instance_count = 1;
        uint32 first_index = 0;
        uint32 base_vertex = 0;
        uint32 first_instance = 0;

        explicit RenderCommandDraw(const VB& vbs, const IB& ib,
            uint32 instanceCount = 1, uint32 firstIndex = 0, uint32 baseVertex = 0, uint32 firstInstance = 0);
        explicit RenderCommandDraw(const sakura::initializer_list<VB>& vbs, const IB& ib,
            uint32 instanceCount = 1, uint32 firstIndex = 0, uint32 baseVertex = 0, uint32 firstInstance = 0);
        template<size_t N>
        explicit RenderCommandDraw(const VB(&vbs)[N], const IB& ib,
            uint32 instanceCount = 1, uint32 firstIndex = 0, uint32 baseVertex = 0, uint32 firstInstance = 0);
    };

    inline RenderCommandDraw::VB::VB(
        RenderBufferHandle vb, size_t _offset, size_t _stride)
        :offset(_offset), stride(_stride), vertex_buffer(vb)
    {

    }
    inline RenderCommandDraw::IB::IB(RenderBufferHandle ib, size_t indexCount,
        EIndexFormat _format, size_t _offset, size_t _stride)
        : offset(_offset), stride(_stride), format(_format), index_buffer(ib), index_count(indexCount)
    {

    }

    inline RenderCommandDraw::RenderCommandDraw(const VB& _vbs, const IB& _ib,
        uint32 instanceCount, uint32 firstIndex, uint32 baseVertex, uint32 firstInstance)
        : vbs({_vbs}), ib(_ib),
        instance_count(instanceCount), first_index(firstIndex), base_vertex(baseVertex), first_instance(firstInstance)
    {

    }

    template <size_t N>
    RenderCommandDraw::RenderCommandDraw(
        const VB(& _vbs)[N], const IB& _ib,
        uint32 instanceCount, uint32 firstIndex, uint32 baseVertex, uint32 firstInstance)
	        :vbs(_vbs, _vbs + N), ib(_ib),
			instance_count(instanceCount), first_index(firstIndex), base_vertex(baseVertex), first_instance(firstInstance)
    {
    	
    }

    inline RenderCommandDraw::RenderCommandDraw(const sakura::initializer_list<VB>& _vbs, const IB& _ib,
        uint32 instanceCount, uint32 firstIndex, uint32 baseVertex, uint32 firstInstance)
	    :vbs(_vbs), ib(_ib),
        instance_count(instanceCount), first_index(firstIndex), base_vertex(baseVertex), first_instance(firstInstance)
    {
        
    }
}


namespace fmt
{
    template<> struct formatter<sakura::graphics::RenderCommandBeginRenderPass> : fmt::formatter<sakura::string_view>
    {
        using RenderCommandBeginRenderPass = sakura::graphics::RenderCommandBeginRenderPass;
        template<typename FormatContext>
        auto format(const RenderCommandBeginRenderPass& cmd, FormatContext& ctx)
        {
            using namespace sakura::graphics;
            using namespace sakura::graphics::detail;
        	
            constexpr sakura::string_view totalTitle = " BeginRenderPass ";
            constexpr sakura::string_view pplTitle = "Pipeline";
            constexpr size_t widthLeft = sakura::max(pplTitle.size(), size_t(0));
            int widthRight = 20;
            return fmt::format_to(
                ctx.out(),
                "***{4:*^{2}}***\n"
                "* {5: <{0}}   {6: >{1}} *\n"
                "***{3:*^{2}}***\n",
                widthLeft, widthRight, widthRight + widthLeft + 1,
                "", //3 : none
                totalTitle,//4: TotalTitle
                pplTitle, cmd.pipeline// 5, 6 pipeline
            );
        }
    };
}