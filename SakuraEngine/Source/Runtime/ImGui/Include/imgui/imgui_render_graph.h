#pragma once
#include "RenderGraph/RenderGraph.h"

namespace sakura::graphics
{
    ImGuiAPI void initialize_imgui(graphics::RenderGraph& render_graph, graphics::IRenderDevice& device);
}
