#include "imgui/sakura_imgui.h"

#include "System/Input.h"
#include "RuntimeCore/RuntimeCore.h"

#include "RenderGraph/RenderGraph.h"
#include "RenderGraphWebGPU/RenderGraphWebGPU.h"
#include "RenderGraphVulkan/RenderGraphVulkan.h"

using namespace sakura;
using namespace sakura::graphics;

inline sakura::Window main_window;
inline RenderGraph render_graph;
inline IRenderDevice* render_device = nullptr;
inline SwapChainHandle swap_chain = render_graph.SwapChain("DefaultSwapChain");

const bool useVk = false;
struct Timer
{
    void start_up()
    {
        tmpt = std::chrono::system_clock::now();
    }

    double end()
    {
        auto dur = std::chrono::system_clock::now() - tmpt;
        auto delta_time =
            static_cast<double>(dur.count()) / static_cast<double>(decltype(dur)::period::den);
        tmpt = std::chrono::system_clock::now();
        return delta_time;
    }
    std::chrono::system_clock::time_point tmpt;
};

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
        render_device = new webgpu::RenderDevice(deviceConfig);
        assert(render_device != nullptr && "ERROR: Failed to create Dawn device!");
    }
    else
    {
        deviceConfig.name = "VulkanDevice";
        render_device = new vk::RenderDevice(deviceConfig);
        assert(render_device != nullptr && "ERROR: Failed to create Vulkan device!");
    }
    // Create Swap Chains.
    render_device->create_swap_chain(swap_chain, SwapChainDesc(EPresentMode::Mailbox, main_window, 3));
	
    sakura::imgui::initialize(main_window);
    sakura::imgui::initialize_gfx(render_graph, *render_device);
    
    RenderCommandBuffer command_buffer("ImGuiRender", 4096);
    RenderPassHandle pass = render_graph.create_render_pass<imgui::RenderPassImGui>(swap_chain, render_graph);
    Timer timer;
    double deltaTime = 0;
    double added = 0;
	while (sakura::Core::yield())
    {
        sakura::imgui::new_frame(main_window, 1.f / 60.f);
        timer.start_up();
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
		
        if (added >= 1.0 / 60.0)
        {
            added = 0;
            main_window.set_title(fmt::format(L"SakuraEngine: {:.2f} FPS", 1.0 / deltaTime).c_str());
        }
		
        {
            // Render
            RenderPass* pass_ptr = render_graph.render_pass(pass);
            pass_ptr->construct(render_graph.builder(pass), *render_device);
            command_buffer.reset();
            pass_ptr->execute(command_buffer, render_graph, *render_device);
            render_device->execute(command_buffer, pass_ptr->handle());
            render_device->present(swap_chain);
        }

        deltaTime = timer.end();

        added += deltaTime;
    }
}