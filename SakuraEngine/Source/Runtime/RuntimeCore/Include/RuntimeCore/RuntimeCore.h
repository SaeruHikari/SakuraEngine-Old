#pragma once

#include "Containers/pair.hpp"
#include "Module.h"

#include "System/Memory.h"
#include "Base/GUID.h"
#include "Base/GenerationalId.h"
#ifdef SAKURA_TARGET_PLATFORM_WIN
#include "Platform/windows/Messages.hpp"
namespace sakura
{
	using ActualOSMessages = sakura::windows::WinMessages;
}
#elif defined(SAKURA_TARGET_PLATFORM_PLAYSTATION)
#include "Platform/prospero/Messages.hpp"
namespace sakura
{
	using ActualOSMessages = sakura::playstation::PSMessages;
}
#elif defined(SAKURA_TARGET_PLATFORM_EMSCRIPTEN)
#include "Platform/web/Messages.hpp"
namespace sakura
{
	using ActualOSMessages = sakura::emscripten::WebMessages;
}
#elif defined(SAKURA_TARGET_PLATFORM_MACOS)
#include "Platform/mac/Messages.hpp"
namespace sakura
{
	using ActualOSMessages = sakura::macos::MacMessages;
}
#endif

namespace sakura
{
	enum class EBuildMode 
	{
		Debug,
		Release,
#ifdef _DEBUG
		Current = Debug,
#else
		Current = Release,
#endif
		Count
	};

	class RuntimeCoreAPI Core
	{
	public:
		struct RuntimeCoreAPI Parameters
		{
			EBuildMode build_mode = EBuildMode::Current;
			sakura::u8string project_dir;
		};
		using handle = void*;
		static void initialize(const Parameters& params) noexcept;
		/**
		 * \return \c true if core is still running (i.e. did not quit)
		*/
		static bool yield() noexcept;
		static void finalize() noexcept;
		static handle current_app_handle() noexcept;
		[[nodiscard]] static std::thread::id get_main_thread_id() noexcept;

		// en: called by default.
		static OSMessages* bind(Window window) noexcept;
		static void unbind(const void* window) noexcept;
		static OSMessages* find_messenger(const void* window) noexcept;
	private:
		static std::thread::id main_thread_id_;
		static std::unordered_map<const void*, std::unique_ptr<ActualOSMessages>> msg_buses_;
	};
	typedef Core CoreContext;

	struct RuntimeCoreAPI IFrame
	{
		virtual ~IFrame() = default;
		virtual const void* raw() const = 0;
		virtual void* raw() = 0;
		virtual bool valid(const size_t currentFrame) const = 0;
	};
}

struct RuntimeCoreAPI RuntimeCoreModule
{
    static void StartUp();
}; 