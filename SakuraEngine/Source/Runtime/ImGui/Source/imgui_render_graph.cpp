#include "imgui/imgui_render_graph.h"

#include <vulkan/vulkan_core.h>



#include "Math/Math.hpp"
#include "Math/Matrix.h"
#include "ShaderCompiler/ShaderCompiler.h"
#include "System/Input.h"

using namespace sakura;
using namespace sakura::graphics;

static RenderPipelineHandle imgui_render_pipeline;
static GpuTextureHandle imgui_fonts_texture = GenerationalId::UNINITIALIZED;
static GpuSamplerHandle imgui_fonts_sampler = GenerationalId::UNINITIALIZED;
static GpuShaderHandle imgui_vs = GenerationalId::UNINITIALIZED;
static GpuShaderHandle imgui_ps = GenerationalId::UNINITIALIZED;
static sakura::vector<std::byte> imgui_vs_spirv;
static sakura::vector<std::byte> imgui_ps_spirv;

static GpuBufferHandle imgui_vertex_buffer = GenerationalId::UNINITIALIZED;
static GpuBufferHandle imgui_index_buffer = GenerationalId::UNINITIALIZED;
static GpuBufferHandle imgui_projection_matrix = GenerationalId::UNINITIALIZED;
static float4x4 projection_matrix;

static Attachment attachment;


namespace sakura::imgui
{
    ImGuiAPI ImGuiContext* imgui_context = nullptr;
    void imgui_create_fonts(graphics::IRenderDevice& device);
    void imgui_create_pipeline(graphics::IRenderDevice& device);
	
    static void imgui_setup_render_state(graphics::RenderCommandBuffer& command_buffer, ImDrawData* draw_data, int fb_width, int fb_height)
    {
        command_buffer.enqueue<RenderCommandBeginRenderPass>(imgui_render_pipeline, attachment);
        command_buffer.enqueue<RenderCommandUpdateBinding>(
            Binding(0, 0, imgui_projection_matrix, 0, sizeof(sakura::float4x4), 0));
        command_buffer.enqueue<RenderCommandUpdateBinding>(Binding(1, 0, imgui_fonts_sampler));
        command_buffer.enqueue<RenderCommandUpdateBinding>(Binding(2, 0, imgui_fonts_texture));
    }
	
    const RenderCommandBuffer& RenderPassImGui::execute(RenderCommandBuffer& command_buffer, const RenderGraph& rg,
        IRenderDevice& device) noexcept
    {
        ImDrawData* draw_data = ImGui::GetDrawData();

        // Avoid rendering when minimized, scale coordinates for retina displays (screen coordinates != framebuffer coordinates)
        int fb_width = (int)(draw_data->DisplaySize.x * draw_data->FramebufferScale.x);
        int fb_height = (int)(draw_data->DisplaySize.y * draw_data->FramebufferScale.y);
        if (fb_width <= 0 || fb_height <= 0)
            return command_buffer;
        // TODO: FrameBuffers.

        if (draw_data->TotalVtxCount > 0)
        {
            imgui_setup_render_state(command_buffer, draw_data, fb_width, fb_height);

            // Will project scissor/clipping rectangles into framebuffer space
            ImVec2 clip_off = draw_data->DisplayPos;         // (0,0) unless using multi-viewports
            ImVec2 clip_scale = draw_data->FramebufferScale; // (1,1) unless using retina display which are often (2,2)

            // Render command lists
            // (Because we merged all buffers into a single one, we maintain our own offset into them)
            int global_vtx_offset = 0;
            int global_idx_offset = 0;
            for (int n = 0; n < draw_data->CmdListsCount; n++)
            {
                const ImDrawList* cmd_list = draw_data->CmdLists[n];
                for (int cmd_i = 0; cmd_i < cmd_list->CmdBuffer.Size; cmd_i++)
                {
                    const ImDrawCmd* pcmd = &cmd_list->CmdBuffer[cmd_i];
                    if (pcmd->UserCallback != NULL)
                    {
                        // TODO: Support this.
                        // User callback, registered via ImDrawList::AddCallback()
                        // (ImDrawCallback_ResetRenderState is a special callback value used by the user to request the renderer to reset render state.)
                        if (pcmd->UserCallback == ImDrawCallback_ResetRenderState)
                           imgui_setup_render_state(command_buffer, draw_data, fb_width, fb_height);
                        //else
                        //    pcmd->UserCallback(cmd_list, pcmd);
                    }
                    else
                    {
                        // Project scissor/clipping rectangles into framebuffer space
                        ImVec4 clip_rect;
                        clip_rect.x = (pcmd->ClipRect.x - clip_off.x) * clip_scale.x;
                        clip_rect.y = (pcmd->ClipRect.y - clip_off.y) * clip_scale.y;
                        clip_rect.z = (pcmd->ClipRect.z - clip_off.x) * clip_scale.x;
                        clip_rect.w = (pcmd->ClipRect.w - clip_off.y) * clip_scale.y;

                        if (clip_rect.x < fb_width && clip_rect.y < fb_height && clip_rect.z >= 0.0f && clip_rect.w >= 0.0f)
                        {
                            // Negative offsets are illegal for vkCmdSetScissor
                            if (clip_rect.x < 0.0f)
                                clip_rect.x = 0.0f;
                            if (clip_rect.y < 0.0f)
                                clip_rect.y = 0.0f;

                            // Apply scissor/clipping rectangle
                            pos2d scissor_offset = {};
                            sakura::extent2d scissor_extent = {};
                            scissor_offset.x = (int32_t)(clip_rect.x);
                            scissor_offset.y = (int32_t)(clip_rect.y);
                            scissor_extent.width = (uint32_t)(clip_rect.z - clip_rect.x);
                            scissor_extent.height = (uint32_t)(clip_rect.w - clip_rect.y);
                            //vkCmdSetScissor(command_buffer, 0, 1, &scissor);
                            // Draw
                            command_buffer.enqueue<RenderCommandSetVB>(imgui_vertex_buffer);
                            command_buffer.enqueue<RenderCommandSetIB>(imgui_index_buffer, EIndexFormat::UINT16);

                            command_buffer.enqueue<RenderCommandDrawInstancedWithArgs>(
                                pcmd->ElemCount, 1, pcmd->IdxOffset + global_idx_offset, pcmd->VtxOffset + global_vtx_offset, 0
                            );
                        }
                    }
                }
                global_idx_offset += cmd_list->IdxBuffer.Size;
                global_vtx_offset += cmd_list->VtxBuffer.Size;
            }
            command_buffer.enqueue<RenderCommandEndRenderPass>();
        }
        return command_buffer;
    }

    std::size_t calc_align(std::size_t n, std::size_t align)
    {
        return ((n + align - 1) / align) * align;
    }
	
    bool RenderPassImGui::construct(RenderGraph::Builder& builder, IRenderDevice& device) noexcept
    {
        ImDrawData* draw_data = ImGui::GetDrawData();

        if(imgui_render_pipeline == GenerationalId::UNINITIALIZED)
        {
            imgui_render_pipeline = render_graph.RenderPipeline("ImGuiRenderPipeline");
            RenderPipelineDescriptor pipelineDesc = RenderPipelineDescriptor(
                ShaderLayout({
                    // Create Actual ShaderResources on Device.
                    device.create_shader(imgui_vs, ShaderDescriptor("ImGuiVertexShader", "main", EShaderFrequency::VertexShader, imgui_vs_spirv)),
                    device.create_shader(imgui_ps, ShaderDescriptor("ImGuiPiexelShader", "main", EShaderFrequency::PixelShader, imgui_ps_spirv))
                    }),
                VertexLayout(
                    {
                        VertexLayout::Element("POSITION", EVertexFormat::FLOAT2, IM_OFFSETOF(ImDrawVert, pos)),
                        VertexLayout::Element("TEXCOORD0", EVertexFormat::FLOAT2, IM_OFFSETOF(ImDrawVert, uv)),
                        VertexLayout::Element("COLOR0", EVertexFormat::R8G8B8A8_UNORM, IM_OFFSETOF(ImDrawVert, col)),
                    }, VertexLayout::Frequency::PerVertexData, sizeof(ImDrawVert)
                    ),
                BindingLayout(
                    {
                        BindingLayout::Set(
                        {
                            BindingLayout::Slot(0, BindingLayout::UniformBuffer, EShaderFrequency::VertexShader),
                        }),
                        BindingLayout::Set(
                        {
                            BindingLayout::Slot(0, BindingLayout::Sampler, EShaderFrequency::PixelShader),
                        }),
                    	BindingLayout::Set(
                        {
                            BindingLayout::Slot(0, BindingLayout::SampledTexture, EShaderFrequency::PixelShader),
                        }),
                    }),
                    AttachmentLayout(
                        { AttachmentLayout::Slot(device.get<ISwapChain>(swap_chain)->render_format(), ELoadOp::Clear, EStoreOp::Store) }
                    ),
                ECullMode::Back, EPrimitiveTopology::TriangleList, EPolygonMode::FILL, 1, 0xFFFFFFFF
            );
            // Create Render pipeline.
            device.create_render_pipeline(imgui_render_pipeline, pipelineDesc);
        }
    	
        if (draw_data->TotalVtxCount > 0)
        {
            // Create or resize the vertex/index buffers
            size_t vertex_size = draw_data->TotalVtxCount * sizeof(ImDrawVert);
            size_t index_size = draw_data->TotalIdxCount * sizeof(ImDrawIdx);

            device.destroy(imgui_vertex_buffer);
            device.destroy(imgui_index_buffer);
        	// TODO: refactor this!
            imgui_vertex_buffer = render_graph.GpuBuffer();
            imgui_index_buffer = render_graph.GpuBuffer();
        	
            // Upload vertex/index data into a single contiguous GPU buffer
            std::vector<ImDrawVert> vtx; 
            std::vector<ImDrawIdx> idx;
            vtx.resize(calc_align(vertex_size / sizeof(ImDrawVert), 4));
        	idx.resize(calc_align(index_size / sizeof(ImDrawIdx), 4));
            ImDrawVert* vtx_dst = (ImDrawVert*)vtx.data();
            ImDrawIdx* idx_dst = (ImDrawIdx*)idx.data();
            for (int n = 0; n < draw_data->CmdListsCount; n++)
            {
                const ImDrawList* cmd_list = draw_data->CmdLists[n];
                memcpy(vtx_dst, cmd_list->VtxBuffer.Data, cmd_list->VtxBuffer.Size * sizeof(ImDrawVert));
                memcpy(idx_dst, cmd_list->IdxBuffer.Data, cmd_list->IdxBuffer.Size * sizeof(ImDrawIdx));
                vtx_dst += cmd_list->VtxBuffer.Size;
                idx_dst += cmd_list->IdxBuffer.Size;
            }
            device.create_buffer(imgui_vertex_buffer, 
                BufferDescriptor(EBufferUsage::CopyDst | EBufferUsage::VertexBuffer, calc_align(vertex_size, 4), vtx.data(), EBufferCPUAccess::None));
            device.create_buffer(imgui_index_buffer, 
                BufferDescriptor(EBufferUsage::CopyDst | EBufferUsage::IndexBuffer, calc_align(index_size, 4), idx.data(), EBufferCPUAccess::None));
        }

    	
        // Update Projection Matrix.
        bool clip_origin_lower_left = true;
#if defined(GL_CLIP_ORIGIN) && !defined(__APPLE__)
        GLenum current_clip_origin = 0; glGetIntegerv(GL_CLIP_ORIGIN, (GLint*)&current_clip_origin);
        if (current_clip_origin == GL_UPPER_LEFT)
            clip_origin_lower_left = false;
#endif
        float L = draw_data->DisplayPos.x;
        float R = draw_data->DisplayPos.x + draw_data->DisplaySize.x;
        float T = draw_data->DisplayPos.y;
        float B = draw_data->DisplayPos.y + draw_data->DisplaySize.y;
        if (!clip_origin_lower_left) { float tmp = T; T = B; B = tmp; } // Swap top and bottom if origin is upper left
        const float ortho_projection[] =
        {
             2.0f / (R - L),   0.0f,         0.0f,   0.0f ,
             0.0f,         2.0f / (T - B),   0.0f,   0.0f ,
             0.0f,         0.0f,        -1.0f,   0.0f ,
             (R + L) / (L - R),  (T + B) / (B - T),  0.0f,   1.0f ,
        };
        const float4x4 projection = math::transpose(ortho_projection);
        device.update_buffer(imgui_projection_matrix, 0, (void*)&projection, sizeof(float4x4));

    	
        attachment = Attachment({
            Attachment::Slot(swap_chain, sakura::double4(), ELoadOp::DontCare, EStoreOp::Store)
        });
        return true;
    }




	
    static void imgui_render_window(ImGuiViewport* viewport, void*)
    {
        if (!(viewport->Flags & ImGuiViewportFlags_NoRendererClear))
        {
            ImVec4 clear_color = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);

        }
        //ImGui_ImplOpenGL3_RenderDrawData(viewport->DrawData);
    }

	
    void initialize_gfx(graphics::RenderGraph& render_graph, graphics::IRenderDevice& device)
    {
        // Upload Fonts
        imgui_fonts_texture = render_graph.GpuTexture("ImGuiFontsTexture");
        imgui_fonts_sampler = render_graph.GpuSampler("ImGuiFontsSampler");
        imgui_vs = render_graph.GpuShader("ImGuiVertexShader");
        imgui_ps = render_graph.GpuShader("ImGuiPixelShader");
        imgui_projection_matrix = render_graph.GpuBuffer("ImGuiProjectionMatrix");

        BufferDescriptor pmDesc =
            BufferDescriptor(EBufferUsage::CopyDst | EBufferUsage::UniformBuffer, sizeof(float4x4), &projection_matrix, EBufferCPUAccess::None);
        device.create_buffer(imgui_projection_matrix, pmDesc);
    	
        ImGuiIO& io = ImGui::GetIO(); (void)io;

        io.BackendRendererName = "imgui_render_graph";
        // We can honor the ImDrawCmd::VtxOffset field, allowing for large meshes.
        io.BackendFlags |= ImGuiBackendFlags_RendererHasVtxOffset;
        // We can create multi-viewports on the Renderer side (optional)
        io.BackendFlags |= ImGuiBackendFlags_RendererHasViewports;
    	
        // Our render function expect RendererUserData to be storing the window render buffer we need (for the main viewport we won't use ->Window)
        ImGuiViewport* main_viewport = ImGui::GetMainViewport();
        //main_viewport->RendererUserData = IM_NEW(ImGuiViewportDataVulkan)();

        imgui_create_fonts(device);
        imgui_create_pipeline(device);
    	
        //if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        //    ImGui_ImplVulkan_InitPlatformInterface();
        ImGuiPlatformIO& platform_io = ImGui::GetPlatformIO();
        platform_io.Renderer_RenderWindow = imgui_render_window;
    }

	
    static void imgui_create_fonts(graphics::IRenderDevice& device)
    {
        ImGuiIO& io = ImGui::GetIO();

        unsigned char* pixels;
        int width, height;
        io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);
        size_t upload_size = width * height * 4 * sizeof(char);

        // Create & Initialize fonts texture.
        {
            TextureDescriptor texDesc = {};
            texDesc.size = { static_cast<uint32>(width), static_cast<uint32>(height), 1 };
            texDesc.dimension = ETextureDimension::Texture2D;
            texDesc.format = ETextureFormat::R8G8B8A8_UNORM;
            texDesc.mip_levels = 1;
            texDesc.sharing_mode = ESharingMode::Concurrent;
            texDesc.sample_count = 1;
            texDesc.array_layers = 1;
            texDesc.usages = ETextureUsage::CopyDst | ETextureUsage::Sampled;
            device.create_texture(imgui_fonts_texture, texDesc);

            TextureSlice slice = {};
            slice.origin = { 0, 0, 0 };
            slice.aspect = Texture::EAspect::All;
            slice.mip_level = 0;
            TextureDataLayout layout = {};
            layout.bytes_per_raw = width * 4 * sizeof(char);
            layout.offset = 0;
            layout.rows_per_image = height;
            device.write_texture(imgui_fonts_texture, pixels, upload_size, slice,
                layout, { static_cast<uint32>(width), static_cast<uint32>(height), 1 });
        }

        // Create font sampler.
        {
            SamplerDescriptor samplerDesc = {};
            samplerDesc.address_mode_u = ESamplerAddressMode::Repeat;
            samplerDesc.address_mode_v = ESamplerAddressMode::Repeat;
            samplerDesc.address_mode_w = ESamplerAddressMode::Repeat;
            samplerDesc.anisotropy = true;
            samplerDesc.mag_filter = EFilter::Nearest;
            samplerDesc.min_filter = EFilter::Nearest;
            samplerDesc.max_anisotropy = 1.f;
            samplerDesc.max_lod = 1000.f;
            samplerDesc.min_lod = 0.f;
            device.create_sampler(imgui_fonts_sampler, samplerDesc);
        }

        // Set to io.
        io.Fonts->TexID = (ImTextureID)(intptr_t)&imgui_fonts_texture;
    }


    void imgui_create_pipeline(graphics::IRenderDevice& device)
    {
        sakura::development::ShaderCompilerVariables vars;
        vars.target_language = sakura::development::EShaderLanguage::Spirv;

        vars.entry = u8"main";
        vars.name = u8"ImGuiVS";
        vars.freq = sakura::graphics::EShaderFrequency::VertexShader;
        imgui_vs_spirv =
            sakura::development::compile_hlsl(imgui_vertex_shader, vars);

        vars.entry = u8"main";
        vars.name = u8"ImGuiPS";
        vars.freq = sakura::graphics::EShaderFrequency::PixelShader;
        imgui_ps_spirv =
            sakura::development::compile_hlsl(imgui_pixel_shader, vars);
    	

    }
}
