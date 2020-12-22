#include "imgui/imgui_render_graph.h"

using namespace sakura;
using namespace sakura::graphics;

static RenderPipelineHandle imgui_render_pipeline;
static RenderTextureHandle imgui_fonts_texture;

namespace sakura
{
    ImGuiAPI ImGuiContext* imgui_context = nullptr;

    void initialize_imgui(graphics::RenderGraph& render_graph, graphics::IRenderDevice& device)
    {
        // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
        //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
        //io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoTaskBarIcons;
        //io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoMerge;

        // Setup Dear ImGui style
        ImGui::StyleColorsDark();
        //ImGui::StyleColorsClassic();
        // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
        ImGuiStyle& style = ImGui::GetStyle();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            style.WindowRounding = 0.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }

        io.BackendRendererName = "imgui_render_graph";
        // We can honor the ImDrawCmd::VtxOffset field, allowing for large meshes.
        io.BackendFlags |= ImGuiBackendFlags_RendererHasVtxOffset;
        // We can create multi-viewports on the Renderer side (optional)
        io.BackendFlags |= ImGuiBackendFlags_RendererHasViewports;

        // Upload Fonts
        imgui_fonts_texture = render_graph.RenderTexture("ImGuiFonts");

    }


    void create_imgui_fonts(graphics::IRenderDevice& device, graphics::RenderCommandBuffer& buffer)
    {
        ImGuiIO& io = ImGui::GetIO();

        unsigned char* pixels;
        int width, height;
        io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);
        size_t upload_size = width * height * 4 * sizeof(char);

        RenderCommandBuffer buffer("ImGuiUploadFont", 1024);

        TextureDesc texDesc = {};
        texDesc.size = { width, height, 1 };
        texDesc.dimension = ETextureDimension::Texture2D;
        texDesc.format = ETextureFormat::R8G8B8A8_UNORM;
        texDesc.mip_levels = 1;
        texDesc.sharing_mode = ESharingMode::Concurrent;
        texDesc.sample_count = 1;
        texDesc.array_layers = 1;
        device.create_texture(imgui_fonts_texture, texDesc);

    }

}
