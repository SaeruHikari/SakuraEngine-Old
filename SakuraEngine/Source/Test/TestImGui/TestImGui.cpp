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
    
    graphics::initialize_imgui(render_graph, device_group);
}