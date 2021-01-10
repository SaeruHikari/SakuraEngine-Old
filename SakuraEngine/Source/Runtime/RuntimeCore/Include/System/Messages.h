#pragma once
#include "rxcpp/rx.hpp"
#include "Input.h"

namespace sakura
{
	using MessageCode = uint32_t;
	static constexpr MessageCode SM_KEYDOWN = 0x0100;
	static constexpr MessageCode SM_SYS_KEYDOWN = 0x0104;

	static constexpr MessageCode SM_KEYUP = 0x0101;
	static constexpr MessageCode SM_SYS_KEYUP = 0x0105;
	static constexpr MessageCode SM_REPAINT = 0x000F;

	struct RuntimeCoreAPI OSMessage
	{
		MessageCode message;
	};
	// en: keyboard rx event.
	struct RuntimeCoreAPI OSKeyboardMessage
	{
		MessageCode message;
		input::EKeyCode key_code;
	};

	struct RuntimeCoreAPI OSMessages
	{
		OSMessages(const Window window);
		virtual ~OSMessages();

		template<typename MsgT>
		rxcpp::observable<MsgT> messages() 
		{
			if constexpr (std::is_same_v<MsgT, OSMessage>)
			{
				return os_subject_.get_observable();
			}
			else if constexpr (std::is_same_v<MsgT, OSKeyboardMessage>)
			{
				return kb_subject_.get_observable();
			}
			else
			{
				static_assert(false, "Such OSMessageType does not exist!");
			}
		}
	protected:
		Window window_;
		rxcpp::subjects::subject<OSMessage> os_subject_;
		rxcpp::subscriber<OSMessage> os_subscriber_;

		rxcpp::subjects::subject<OSKeyboardMessage> kb_subject_;
		rxcpp::subscriber<OSKeyboardMessage> kb_subscriber_;
	};


	OSMessages::OSMessages(const Window window)
		: kb_subscriber_(kb_subject_.get_subscriber()), os_subscriber_(os_subject_.get_subscriber()), window_(window)
	{

	}

	OSMessages::~OSMessages()
	{
		kb_subscriber_.on_completed();
		os_subscriber_.on_completed();
	}
}



