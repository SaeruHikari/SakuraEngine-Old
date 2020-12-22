#pragma once
#include "Base/Definations.h"

namespace sakura::input
{
	enum class EKeyCode : uint32
	{
		Invalid = 0x00,
		Backspace = 0x08,
		Tab = 0x09,
		Clear = 0x0C,
		Enter = 0x0D,
		Shift = 0x10,
		Ctrl = 0x11,
		Alt = 0x12,
		Pause = 0x13,
		CapsLock = 0x14,
		Esc = 0x1B,
		SpaceBar = 0x20,
		PageUp = 0x21,
		PageDown = 0x22,
		End = 0x23,
		Home = 0x24,
		Left = 0x25,
		Up = 0x26,
		Right = 0x27,
		Down = 0x28,
		Select = 0x29,
		Print = 0x2A,
		Execute = 0x2B,
		Print_screen = 0x2C,
		Insert = 0x2D,
		Del = 0x2E,
		Help = 0x2F,
		Zero = 0x30,
		One = 0x31,
		Two = 0x32,
		Three = 0x33,
		Four = 0x34,
		Five = 0x35,
		Six = 0x36,
		Seven = 0x37,
		Eight = 0x38,
		Nine = 0x39,
		A = 0x41,
		B = 0x42,
		C = 0x43,
		D = 0x44,
		E = 0x45,
		F = 0x46,
		G = 0x47,
		H = 0x48,
		I = 0x49,
		J = 0x4A,
		K = 0x4B,
		L = 0x4C,
		M = 0x4D,
		N = 0x4E,
		O = 0x4F,
		P = 0x50,
		Q = 0x51,
		R = 0x52,
		S = 0x53,
		T = 0x54,
		U = 0x55,
		V = 0x56,
		W = 0x57,
		X = 0x58,
		Y = 0x59,
		Z = 0x5A,
		LSystem = 0x5B,
		RSystem = 0x5C,
		App = 0x5D,
		Sleep = 0x5F,
		Numpad0 = 0x60,
		Numpad1 = 0x61,
		Numpad2 = 0x62,
		Numpad3 = 0x63,
		Numpad4 = 0x64,
		Numpad5 = 0x65,
		Numpad6 = 0x66,
		Numpad7 = 0x67,
		Numpad8 = 0x68,
		Numpad9 = 0x69,
		Multiply = 0x6A,
		Add = 0x6B,
		Separator = 0x6C,
		Subtract = 0x6D,
		Decimal = 0x6E,
		Divide = 0x6F,
		F1 = 0x70,
		F2 = 0x71,
		F3 = 0x72,
		F4 = 0x73,
		F5 = 0x74,
		F6 = 0x75,
		F7 = 0x76,
		F8 = 0x77,
		F9 = 0x78,
		F10 = 0x79,
		F11 = 0x7A,
		F12 = 0x7B,
		F13 = 0x7C,
		F14 = 0x7D,
		F15 = 0x7E,
		F16 = 0x7F,
		F17 = 0x80,
		F18 = 0x81,
		F19 = 0x82,
		F20 = 0x83,
		F21 = 0x84,
		F22 = 0x85,
		F23 = 0x86,
		F24 = 0x87,
		Num_lock = 0x90,
		Scroll_lock = 0x91,
		LShift = 0xA0,
		RShift = 0xA1,
		LCtrl = 0xA2,
		RCtrl = 0xA3,
		LAlt = 0xA4,
		RAlt = 0xA5,
		Semicolon = 0xBA,	// ;: key on US standard keyboard
		Plus = 0xBB,		// =+ key
		Comma = 0xBC,		// ,< key
		Minus = 0xBD,		// -_ key
		Dot = 0xBE,			// .> key
		Slash = 0xBF,		// /? key on US standard keyboard
		Wave = 0xC0,		// ~` key on US standard keyboard
		LBranket = 0xDB,	// [{ key on US standard keyboard
		Backslash = 0xDC,	// \| key on US standard keyboard
		RBranket = 0xDD,	// ]} key on US standard keyboard
		Quote = 0xDE,		// '" key on US standard keyboard
	};

	enum class EMouseKey : uint32
	{
		None = 0x00,
		LB = 0x01,
		RB = 0x02,
		MB = 0x04,
		X1B = 0x08,
		X2B = 0x10,
	};


	enum class EControllerAxis : uint32
	{
		LX = 0,	// Left thumb pad X axis.
		LY = 1,	// Left thumb pad Y axis.
		RX = 2,	// Right thumb pad X axis.
		RY = 3,	// Right thumb pad Y axis.
		LT = 4,	// Left trigger.
		RT = 5,	// Right trigger.
	};

	enum class EControllerButton : uint32
	{
		LThumb = 0,	// Left thumb button.
		RThumb = 1,	// Right thumb button.
		LUp = 2,	// Left up button.
		LDown = 3,	// Left down button.
		LLeft = 4,	// Left left button.
		LRight = 5,	// Left right button.
		RUp = 6,	// Right up button.
		RDown = 7,	// Right down button.
		RLeft = 8,	// Right left button.
		RRight = 9,	// Right right button.
		LB = 10,	// Left shoulder button.
		RB = 11,	// Right shoulder button.
		LT = 12,	// Left trigger button.
		RT = 13,	// Right trigger button.
		LSpecial = 14,	// Left special button.
		RSpecial = 15	// Right special button.
	};

	//! Identify the vibration motor to set in `set_vibration`.
	enum class EControllerVibrationMotor : uint32
	{
		Left = 1,
		Right = 2,
	};

	enum class EMouseCursor
	{
		Hide,
		Arrow,
		TextInput,
		ResizeAll,
		ResizeEW,
		ResizeNS,
		ResizeNESW,
		ResizeNWSE,
		Hand,
		NotAllowed
	};
	
	RuntimeCoreAPI bool key_down(EKeyCode key_code);
	RuntimeCoreAPI bool key_down(EMouseKey key_code);
	RuntimeCoreAPI bool set_cursor_pos(int32 x, int32 y);
	RuntimeCoreAPI bool set_cursor(EMouseCursor cursor);
	RuntimeCoreAPI pos2d cursor_pos();
}