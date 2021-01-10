#include "System/Messages.h"

namespace sakura::windows
{
	class WinMessages final : public OSMessages
	{
		struct Result
		{
			LRESULT lres = 0;
			bool handled = false;
		};
		struct WinMessage
		{
			HWND hWnd;
			UINT message;
			WPARAM wParam;
			LPARAM lParam;
			Result* result;

			void handled() noexcept {
				result->handled = true;
			}

			template<UINT WM>
			static auto is() noexcept {
				return [](WinMessage m) {
					return m.message == WM;
				};
			}
		};
	public:
		std::tuple<bool, LRESULT> message(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	public:
		rxcpp::observable<WinMessage> messages_win() {
			return subject_win_.get_observable();
		}

		template<UINT WM>
		rxcpp::observable<WinMessage> messages_win() {
			return messages_win().filter(WinMessage::is<WM>());
		}

		FORCEINLINE WinMessages(const Window window);
		FORCEINLINE ~WinMessages() override;

		FORCEINLINE rxcpp::observable<WinMessage> win_messages()
		{
			return subject_win_.get_observable();
		}
	private:
		void onKeyboard();
		void onPresent();
		rxcpp::subjects::subject<WinMessage> subject_win_;
		rxcpp::subscriber<WinMessage> subscriber_win_;
	};

	FORCEINLINE std::tuple<bool, LRESULT> WinMessages::message(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) 
	{
		Result result;
		auto m = WinMessage{ hWnd, message, wParam, lParam, &result };
		RXCPP_TRY{
			subscriber_win_.on_next(m);
		} RXCPP_CATCH(...) {
			subscriber_win_.on_error(std::current_exception());
		}
		return std::make_tuple(result.handled, result.lres);
	}

	FORCEINLINE void WinMessages::onKeyboard()
	{
		messages_win<WM_KEYDOWN>().
			subscribe([this](auto m)
				{
					os_subscriber_.on_next(OSMessage{ SM_KEYDOWN });
					kb_subscriber_.on_next(OSKeyboardMessage{ SM_KEYDOWN, (input::EKeyCode)m.wParam });
				});
		messages_win<WM_SYSKEYDOWN>().
			subscribe([this](auto m)
				{
					os_subscriber_.on_next(OSMessage{ SM_SYS_KEYDOWN });
					kb_subscriber_.on_next(OSKeyboardMessage{ SM_SYS_KEYDOWN, (input::EKeyCode)m.wParam });
				});

		messages_win<WM_KEYUP>().
			subscribe([this](auto m)
				{
					os_subscriber_.on_next(OSMessage{ SM_KEYUP });
					kb_subscriber_.on_next(OSKeyboardMessage{ SM_KEYUP, (input::EKeyCode)m.wParam });
				});
		messages_win<WM_SYSKEYUP>().
			subscribe([this](auto m)
				{
					os_subscriber_.on_next(OSMessage{ SM_SYS_KEYUP });
					kb_subscriber_.on_next(OSKeyboardMessage{ SM_SYS_KEYUP, (input::EKeyCode)m.wParam });
				});
	}

	FORCEINLINE void WinMessages::onPresent()
	{
		messages_win<WM_PAINT>().
			subscribe([this](auto m) 
		{
			m.handled(); // skip DefWindowProc
			PAINTSTRUCT ps;
			BeginPaint(static_cast<HWND>(window_.handle()), &ps);
			EndPaint(static_cast<HWND>(window_.handle()), &ps);

			os_subscriber_.on_next(OSMessage{ WM_PAINT });
		});
	}

	FORCEINLINE WinMessages::WinMessages(const Window window)
		: OSMessages(window), subscriber_win_(subject_win_.get_subscriber())
	{
		onPresent();
		onKeyboard();
	}

	FORCEINLINE WinMessages::~WinMessages()
	{
		subscriber_win_.on_completed();
	}
}