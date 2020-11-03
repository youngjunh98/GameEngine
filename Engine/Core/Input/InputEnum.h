#ifndef INCLUDE_INPUT_ENUM
#define INCLUDE_INPUT_ENUM

namespace GameEngine
{
	enum class EKeyCode
	{
		None,


		/* Normal Keys */

		Escape,
		Enter,
		Space,
		Backspace,
		Tab,
		CapsLock,
		LeftShift,
		RightShift,
		LeftControl,
		RightControl,
		LeftAlt,
		RightAlt,

		PrintScreen,
		ScrollLock,
		Pause,

		Insert,
		Delete,
		Home,
		End,
		PageUp,
		PageDown,

		Alpha0,
		Alpha1,
		Alpha2,
		Alpha3,
		Alpha4,
		Alpha5,
		Alpha6,
		Alpha7,
		Alpha8,
		Alpha9,

		A,
		B,
		C,
		D,
		E,
		F,
		G,
		H,
		I,
		J,
		K,
		L,
		M,
		N,
		O,
		P,
		Q,
		R,
		S,
		T,
		U,
		V,
		W,
		X,
		Y,
		Z,

		BackQuote,
		Minus,
		Plus,
		BackSlash,
		LeftBracket,
		RightBracket,
		Semicolon,
		Quote,
		Comma,
		Dot,
		Slash,


		/* Function Keys */

		F1,
		F2,
		F3,
		F4,
		F5,
		F6,
		F7,
		F8,
		F9,
		F10,
		F11,
		F12,


		/* Arrow Keys */

		LeftArrow,
		RightArrow,
		UpArrow,
		DownArrow,
		
		
		/* Numpad Keys */

		Numpad0,
		Numpad1,
		Numpad2,
		Numpad3,
		Numpad4,
		Numpad5,
		Numpad6,
		Numpad7,
		Numpad8,
		Numpad9,

		NumLock,
		NumpadDivide,
		NumpadMultiply,
		NumpadMinus,
		NumpadPlus,
		NumpaEnter,
		NumpadDot,
		NumpadEquals,
	};

	enum class EMouseButton
	{
		Left,
		Middle,
		Right
	};
}

#endif