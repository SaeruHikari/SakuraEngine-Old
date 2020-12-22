#include "System/Input.h"
#include "Platform/Windows/Windows.h"

namespace sakura
{
	bool input::key_down(EKeyCode key_code)
	{
		return GetAsyncKeyState((uint32)key_code) & 0x8000;
	}

	bool input::key_down(EMouseKey key_code)
	{
		int key;
		switch (key_code)
		{
		case EMouseKey::LB:
			key = VK_LBUTTON;
			break;
		case EMouseKey::RB:
			key = VK_RBUTTON;
			break;
		case EMouseKey::MB:
			key = VK_MBUTTON;
			break;
		case EMouseKey::X1B:
			key = VK_XBUTTON1;
			break;
		case EMouseKey::X2B:
			key = VK_XBUTTON2;
			break;
		default:
			return false;
		}
		return (GetAsyncKeyState(key) & 0x8000) ? true : false;
	}

	bool input::set_cursor_pos(int32 x, int32 y)
	{
		return ::SetCursorPos(x, y);
	}

	bool input::set_cursor(EMouseCursor cursor)
	{
		LPTSTR win32_cursor = IDC_ARROW;
		switch (cursor)
		{
			case EMouseCursor::Arrow:        win32_cursor = IDC_ARROW; break;
			case EMouseCursor::TextInput:    win32_cursor = IDC_IBEAM; break;
			case EMouseCursor::ResizeAll:    win32_cursor = IDC_SIZEALL; break;
			case EMouseCursor::ResizeEW:     win32_cursor = IDC_SIZEWE; break;
			case EMouseCursor::ResizeNS:     win32_cursor = IDC_SIZENS; break;
			case EMouseCursor::ResizeNESW:   win32_cursor = IDC_SIZENESW; break;
			case EMouseCursor::ResizeNWSE:   win32_cursor = IDC_SIZENWSE; break;
			case EMouseCursor::Hand:         win32_cursor = IDC_HAND; break;
			case EMouseCursor::NotAllowed:   win32_cursor = IDC_NO; break;
			case EMouseCursor::Hide:
				return ::SetCursor(NULL);
		}
		return ::SetCursor(::LoadCursor(NULL, win32_cursor));
	}

	pos2d input::cursor_pos()
	{
		POINT p;
		::GetCursorPos(&p);
		return pos2d{ static_cast<uint32>(p.x), static_cast<uint32>(p.y) };
	}
}
