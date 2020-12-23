#pragma once
#include "imgui/imgui.h"
#include "System/Window.h"
#include "RuntimeCore/RuntimeCore.h"

namespace sakura::imgui
{
	ImGuiAPI void bind_window(Window window);

	ImGuiAPI void new_frame(Window window, float delta_time);
}