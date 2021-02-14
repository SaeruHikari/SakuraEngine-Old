#include "RuntimeCore/RuntimeCore.h"
#include <System/Log.h>

int main()
{
    using namespace sakura;
	sakura::Core::initialize(sakura::Core::Parameters());
	sakura::info("game thread id: {}", std::hash<std::thread::id>()(sakura::Core::get_main_thread_id()));
	std::cout << "S" << std::endl;

	sakura::Window::desc windDesc;
	windDesc.height = 1080;
	windDesc.width = 1920;
	windDesc.name = "Sakura Engine";
	auto main_window = sakura::Window::create(windDesc);
    
	std::cout << "E" << std::endl;

	// Game & Rendering Logic
	while (sakura::Core::yield())
	{
		
	}
    return 0;
}