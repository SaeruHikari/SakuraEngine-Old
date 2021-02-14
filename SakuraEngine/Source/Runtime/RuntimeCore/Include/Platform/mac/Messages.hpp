#pragma once
#include "System/Messages.h"

namespace sakura::macos
{
    class MacMessages final : public OSMessages
	{
		struct Result
		{
			int lres = 0;
			bool handled = false;
		};
		struct MacMessage
		{
			void* hWnd;
			int message;
			int wParam;
			int lParam;
			Result* result;

			void handled() noexcept {
				result->handled = true;
			}

			template<int WM>
			static auto is() noexcept {
				return [](MacMessage m) {
					return m.message == WM;
				};
			}
		};
	public:
		rxcpp::observable<MacMessage> messages_mac() {
			return subject_win_.get_observable();
		}

		template<int WM>
		rxcpp::observable<MacMessage> messages_mac() {
			return messages_mac().filter(MacMessage::is<WM>());
		}

		FORCEINLINE MacMessages(const Window window)
            :OSMessages(window), subscriber_win_(subject_win_.get_subscriber())
        {

        }
		FORCEINLINE ~MacMessages() override
        {

        }

		FORCEINLINE rxcpp::observable<MacMessage> win_messages()
		{
			return subject_win_.get_observable();
		}
	private:
		rxcpp::subjects::subject<MacMessage> subject_win_;
		rxcpp::subscriber<MacMessage> subscriber_win_;
	};
}