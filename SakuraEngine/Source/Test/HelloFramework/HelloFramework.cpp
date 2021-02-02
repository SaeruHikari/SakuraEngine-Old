#include "RuntimeCore/RuntimeCore.h"
int main()
{
    using namespace sakura;
	sakura::Core::initialize(sakura::Core::Parameters());
	sakura::info("game thread id: {}", std::hash<std::thread::id>()(sakura::Core::get_main_thread_id()));

	sakura::Window::desc windDesc;
	windDesc.height = 1080;
	windDesc.width = 1920;
	windDesc.name = "Sakura Engine";
	auto main_window = sakura::Window::create(windDesc);
    
	auto rx_os = sakura::Core::find_messenger(main_window.handle());
	rx_os->messages<OSMessage>().subscribe([](auto m)
		{

		});

	// Game & Rendering Logic
	while (sakura::Core::yield())
	{
		
	}
    return 0;
}