#include "imgui/imgui_render_graph.h"

using namespace sakura;
using namespace sakura::graphics;

static RenderPipelineHandle imgui_render_pipeline;

ImGuiAPI void initialize_imgui(graphics::RenderGraph& render_graph, graphics::IRenderDevice& device)
{
    // Setup backend capabilities flags
    ImGuiIO& io = ImGui::GetIO();
    io.BackendRendererName = "imgui_render_graph";
    // We can honor the ImDrawCmd::VtxOffset field, allowing for large meshes.
    io.BackendFlags |= ImGuiBackendFlags_RendererHasVtxOffset;
    // We can create multi-viewports on the Renderer side (optional)
    io.BackendFlags |= ImGuiBackendFlags_RendererHasViewports;

    // Upload Fonts
}

ImGuiAPI void graphics::create_imgui_fonts(RenderCommandBuffer& buffer)
{
    ImGuiIO& io = ImGui::GetIO();

    unsigned char* pixels;
    int width, height;
    io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);
    size_t upload_size = width * height * 4 * sizeof(char);

	
}
