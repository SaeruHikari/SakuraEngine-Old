#pragma once
#include "rxcpp/rx.hpp"
#include "Input.h"

namespace sakura
{
	using MessageCode = uint32_t;
	static constexpr MessageCode SM_KEYDOWN = 0x0100;
	static constexpr MessageCode SM_KEYUP = 0x0101;
	// en: keyboard rx event.
	struct KeyboardMessage
	{
		input::EKeyCode key_code;
	};

	class OSMessages
	{
		// en: OS leading message
		struct OSMessage
		{
			MessageCode code;
		};
	public:
		FORCEINLINE OSMessages()
			: subscriber_(subject_.get_subscriber())
		{

		}

		FORCEINLINE ~OSMessages()
		{
			subscriber_.on_completed();
		}

		FORCEINLINE rxcpp::observable<OSMessage> messages()
		{
			return subject_.get_observable();
		}
	private:
		rxcpp::subjects::subject<OSMessage> subject_;
		rxcpp::subscriber<OSMessage> subscriber_;
	};
}

#ifdef SAKURA_TARGET_PLATFORM_WIN
#include "Platform/Windows/Messages.inl"
#elif defined(SAKURA_TARGET_PLATFORM_PLAYSTATION)
#include "Platform/PlayStation/Messages.inl"
#elif defined(SAKURA_TARGET_PLATFORM_EMSCRIPTEN)
#include "Platform/Web/Messages.inl"
#elif defined(SAKURA_TARGET_PLATFORM_MACOS)
static_assert(0, "Implement This!");
#endif


