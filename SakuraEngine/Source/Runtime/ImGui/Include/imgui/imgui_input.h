#pragma once
#include "imgui/imgui.h"
#include "System/Window.h"
#include "RuntimeCore/RuntimeCore.h"

namespace sakura
{
	ImGuiAPI void imgui_bind_window(Window window);

	ImGuiAPI void imgui_new_frame(Window window, float delta_time);
}