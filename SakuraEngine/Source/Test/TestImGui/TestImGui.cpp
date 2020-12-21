#include "imgui/imgui.h"
#include "imgui/imgui_render_graph.h"
#include "RuntimeCore/RuntimeCore.h"
#include "RenderGraph/RenderGraph.h"

using namespace sakura;
using namespace sakura::graphics;

inline sakura::Window main_window;
inline RenderGraph render_graph;
inline RenderDeviceGroupProxy device_group = RenderDeviceGroupProxy(render_graph);
inline SwapChainHandle swap_chain = render_graph.SwapChain("DefaultSwapChain");

int main(int, char**)
{
    sakura::Core::initialize(sakura::Core::Parameters());
    sakura::info("game thread id: {}", std::hash<std::thread::id>()(sakura::Core::get_main_thread_id()));

    sakura::Window::desc windDesc;
    windDesc.height = 1080;
    windDesc.width = 1920;
    windDesc.name = "Sakura Engine";
    main_window = sakura::Window::create(windDesc);

    initialize_imgui(render_graph, device_group);

    


    
}