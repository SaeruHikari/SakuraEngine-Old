#pragma once
#include "RenderGraph/RenderGraph.h"
#include "imgui/imgui.h"

namespace sakura
{
    ImGuiAPI void initialize_imgui(graphics::RenderGraph& render_graph, graphics::IRenderDevice& device);

    ImGuiAPI void create_imgui_fonts(graphics::RenderCommandBuffer& buffer);
}
