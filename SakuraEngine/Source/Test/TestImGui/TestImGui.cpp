#include "imgui/sakura_imgui.h"

#include "RuntimeCore/RuntimeCore.h"

#include "RenderGraph/RenderGraph.h"
#include "RenderGraphWebGPU/RenderGraphWebGPU.h"
#include "RenderGraphVulkan/RenderGraphVulkan.h"

using namespace sakura;
using namespace sakura::graphics;

inline sakura::Window main_window;
inline RenderGraph render_graph;
inline RenderDeviceGroupProxy device_group = RenderDeviceGroupProxy(render_graph);
inline SwapChainHandle swap_chain = render_graph.SwapChain("DefaultSwapChain");

const bool useVk = false;

int main(int, char**)
{
    sakura::Core::initialize(sakura::Core::Parameters());
    sakura::info("game thread id: {}", std::hash<std::thread::id>()(sakura::Core::get_main_thread_id()));

    sakura::Window::desc windDesc;
    windDesc.height = 1080;
    windDesc.width = 1920;
    windDesc.name = "Sakura Engine";
    main_window = sakura::Window::create(windDesc);

    DeviceConfiguration deviceConfig;
    if (!useVk)
    {
        deviceConfig.name = "DawnDevice";
        render_graph.emplace_device(new webgpu::RenderDevice(deviceConfig));
        IRenderDevice* dawnDevice = render_graph.get_device("DawnDevice");
        assert(dawnDevice != nullptr && "ERROR: Failed to create Dawn device!");
        device_group.emplace(dawnDevice);
    }
    else
    {
        deviceConfig.name = "VulkanDevice";
        render_graph.emplace_device(new vk::RenderDevice(deviceConfig));
        IRenderDevice* vulaknDevice = render_graph.get_device("VulkanDevice");
        assert(vulaknDevice != nullptr && "ERROR: Failed to create Vulkan device!");
        device_group.emplace(vulaknDevice);
    }

    imgui_initialize();
    imgui_bind_window(main_window);
    imgui_initialize_gfx(render_graph, device_group);
   
    RenderCommandBuffer command_buffer("ImGuiRender", 4096);
    while (sakura::Core::yield())
    {
        sakura::imgui_new_frame(main_window, 1.f / 60.f);

        {
            static float f = 0.0f;
            static int counter = 0;

            ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

            ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)

            ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f

            if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
                counter++;
            ImGui::SameLine();
            ImGui::Text("counter = %d", counter);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::End();

            ImGui::Render();
        }

        sakura::imgui_fetch_commands(command_buffer);
    }
    
}