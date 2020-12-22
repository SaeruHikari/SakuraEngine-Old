#include "imgui/imgui_render_graph.h"

#include <vulkan/vulkan_core.h>

using namespace sakura;
using namespace sakura::graphics;

static RenderPipelineHandle imgui_render_pipeline;
static RenderTextureHandle imgui_fonts_texture;

namespace sakura
{
    ImGuiAPI ImGuiContext* imgui_context = nullptr;

    void imgui_initialize_gfx(graphics::RenderGraph& render_graph, graphics::IRenderDevice& device)
    {
        ImGuiIO& io = ImGui::GetIO(); (void)io;
		// Window
        // Our mouse update function expect PlatformHandle to be filled for the main viewport
        //ImGuiViewport* main_viewport = ;

        // Keyboard mapping. ImGui will use those indices to peek into the io.KeysDown[] array.

    	
    	// Render
    	
        io.BackendRendererName = "imgui_render_graph";
        // We can honor the ImDrawCmd::VtxOffset field, allowing for large meshes.
        io.BackendFlags |= ImGuiBackendFlags_RendererHasVtxOffset;
        // We can create multi-viewports on the Renderer side (optional)
        io.BackendFlags |= ImGuiBackendFlags_RendererHasViewports;

        // Upload Fonts
        imgui_fonts_texture = render_graph.RenderTexture("ImGuiFonts");
        // Our render function expect RendererUserData to be storing the window render buffer we need (for the main viewport we won't use ->Window)
        ImGuiViewport* main_viewport = ImGui::GetMainViewport();
        main_viewport->RendererUserData = IM_NEW(ImGuiViewportDataVulkan)();

        imgui_create_fonts(device);
    	
        //if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        //    ImGui_ImplVulkan_InitPlatformInterface();
        ImGuiPlatformIO& platform_io = ImGui::GetPlatformIO();
        //platform_io.Renderer_RenderWindow = ImGui_ImplOpenGL3_RenderWindow;
    }


    void imgui_create_fonts(graphics::IRenderDevice& device)
    {
        ImGuiIO& io = ImGui::GetIO();

        unsigned char* pixels;
        int width, height;
        io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);
        size_t upload_size = width * height * 4 * sizeof(char);

        TextureDesc texDesc = {};
        texDesc.size = { static_cast<uint32>(width), static_cast<uint32>(height), 1 };
        texDesc.dimension = ETextureDimension::Texture2D;
        texDesc.format = ETextureFormat::R8G8B8A8_UNORM;
        texDesc.mip_levels = 1;
        texDesc.sharing_mode = ESharingMode::Concurrent;
        texDesc.sample_count = 1;
        texDesc.array_layers = 1;
        device.create_texture(imgui_fonts_texture, texDesc);

        graphics::RenderCommandBuffer buffer("ImGuiFontsCreate");
    	
    	
    	io.Fonts->TexID = (ImTextureID)(intptr_t)&imgui_fonts_texture;
    }




    // per-frame
	 
    void imgui_new_frame(Window window, float delta_time)
    {
        ImGuiIO& io = ImGui::GetIO();

        auto extent = window.extent();
        io.DisplaySize = {static_cast<float>(extent.width), static_cast<float>(extent.height)};

    	// !! no Scalable Drawing Support.
        io.DisplayFramebufferScale = ImVec2(1, 1);

        io.DeltaTime = delta_time;

    	// Update MousePosAndButtons
    	// Update MouseCursor
    	// Update Gamepads.

        ImGui::NewFrame();
    }

    void imgui_fetch_commands(graphics::RenderCommandBuffer& command_buffer)
    {
        ImDrawData* draw_data = ImGui::GetDrawData();

        // Avoid rendering when minimized, scale coordinates for retina displays (screen coordinates != framebuffer coordinates)
        int fb_width = (int)(draw_data->DisplaySize.x * draw_data->FramebufferScale.x);
        int fb_height = (int)(draw_data->DisplaySize.y * draw_data->FramebufferScale.y);
        if (fb_width <= 0 || fb_height <= 0)
            return;

        if (draw_data->TotalVtxCount > 0)
        {
            // Create or resize the vertex/index buffers
            size_t vertex_size = draw_data->TotalVtxCount * sizeof(ImDrawVert);
            size_t index_size = draw_data->TotalIdxCount * sizeof(ImDrawIdx);

            // Upload vertex/index data into a single contiguous GPU buffer
            ImDrawVert* vtx_dst = NULL;
            ImDrawIdx* idx_dst = NULL;
            for (int n = 0; n < draw_data->CmdListsCount; n++)
            {
                const ImDrawList* cmd_list = draw_data->CmdLists[n];
                memcpy(vtx_dst, cmd_list->VtxBuffer.Data, cmd_list->VtxBuffer.Size * sizeof(ImDrawVert));
                memcpy(idx_dst, cmd_list->IdxBuffer.Data, cmd_list->IdxBuffer.Size * sizeof(ImDrawIdx));
                vtx_dst += cmd_list->VtxBuffer.Size;
                idx_dst += cmd_list->IdxBuffer.Size;
            }
            
        }
    }
}
