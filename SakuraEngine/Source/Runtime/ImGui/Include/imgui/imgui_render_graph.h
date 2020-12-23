#pragma once
#include "RenderGraph/RenderGraph.h"
#include "imgui/imgui.h"

namespace sakura::imgui
{
    ImGuiAPI void initialize_gfx(graphics::RenderGraph& render_graph, graphics::IRenderDevice& device);

    using namespace sakura::graphics;
	
    class ImGuiAPI RenderPassImGui : public graphics::RenderPass
    {
    public:
        RenderPassImGui(const RenderPassHandle handle, const SwapChainHandle swap_chain, RenderGraph& render_graph, sakura::uint8 cycleCount = 3)
            :RenderPass(handle, cycleCount), render_graph(render_graph), swap_chain(swap_chain) {}
        const RenderCommandBuffer& execute(RenderCommandBuffer& command_buffer, const RenderGraph& rg, IRenderDevice& device) noexcept override;
        bool construct(RenderGraph::Builder& builder, IRenderDevice& device) noexcept override;
        RenderGraph& render_graph;
        SwapChainHandle swap_chain = GenerationalId::UNINITIALIZED;
    };
}



namespace sakura
{
    // Helper structure to hold the data needed by one rendering context into one OS window
// (Used by example's main.cpp. Used by multi-viewport features. Probably NOT used by your own engine/app.)
    struct ImGuiWindow
    {
        int                 width;
        int                 height;
        Window              window;
        // Current frame being rendered to (0 <= FrameIndex < FrameInFlightCount)
        uint32_t            frame_index;
        // Number of simultaneous in-flight frames (returned by vkGetSwapchainImagesKHR, usually derived from min_image_count)
        uint32_t            image_count;
    };

    // Reusable buffers used for rendering 1 current in-flight frame, for ImGui_ImplVulkan_RenderDrawData()
    // [Please zero-clear before use!]
    struct ImGuiFrameRenderBuffers
    {
        size_t        vertex_buffer_size;
        size_t        index_buffer_size;
        graphics::GpuBufferHandle            vertex_buffer;
        graphics::GpuBufferHandle            index_buffer;
    };

    // [Please zero-clear before use!]
    struct ImGuiWindowRenderBuffers
    {
        uint32_t            index;
        uint32_t            count;
        ImGuiFrameRenderBuffers* frame_render_buffers;
    };

    // For multi-viewport support:
    // Helper structure we store in the void* RenderUserData field of each ImGuiViewport to easily retrieve our backend data.
    struct ImGuiViewportDataVulkan
    {
        bool                       window_owned;
        ImGuiWindow                window;             // Used by secondary viewports only
        ImGuiWindowRenderBuffers   render_buffers;      // Used by all viewports

        ImGuiViewportDataVulkan() { window_owned = false; memset(&render_buffers, 0, sizeof(render_buffers)); }
        ~ImGuiViewportDataVulkan() { }
    };

    const char imgui_vertex_shader[] =
           "[[vk::binding(0, 0)]] cbuffer vertexBuffer : register(b0) \
            {\
              float4x4 ProjectionMatrix; \
            };\
            struct VS_INPUT\
            {\
              float2 pos : POSITION;\
              float2 uv  : TEXCOORD0;\
              float4 col : COLOR0;\
            };\
            \
            struct PS_INPUT\
            {\
              float4 pos : SV_POSITION;\
              float4 col : COLOR0;\
              float2 uv  : TEXCOORD0;\
            };\
            \
            PS_INPUT main(VS_INPUT input)\
            {\
              PS_INPUT output;\
              output.pos = mul(float4(input.pos.xy, 0.f, 1.f), ProjectionMatrix);\
              output.col = input.col;\
              output.uv  = input.uv;\
              return output;\
            }";

    const char imgui_pixel_shader[] =
           "struct PS_INPUT\
            {\
              float4 pos : SV_POSITION;\
              float4 col : COLOR0;\
              float2 uv  : TEXCOORD0;\
            };\
            [[vk::binding(0, 1)]] SamplerState sampler0 : register(s0);\
            [[vk::binding(1, 1)]] Texture2D texture0 : register(t0);\
            \
            float4 main(PS_INPUT input) : SV_Target\
            {\
              float4 out_col = input.col * texture0.Sample(sampler0, input.uv); \
              return out_col; \
            }";
}