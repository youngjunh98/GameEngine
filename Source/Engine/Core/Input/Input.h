#ifndef INCLUDE_INPUT
#define INCLUDE_INPUT

#include "Engine/Core/CoreMacro.h"
#include "Engine/Core/Input/InputEnum.h"
#include "Engine/Core/Math/Math.h"

namespace GameEngine
{
	class ENGINE_CORE_API Input
	{
	public:
		static bool GetKey (EKeyCode keyCode);
		static bool GetKeyUp (EKeyCode keyCode);
		static bool GetKeyDown (EKeyCode keyCode);

		static bool GetMouseButton (EMouseButton mouseButton);
		static bool GetMouseButtonUp (EMouseButton mouseButton);
		static bool GetMouseButtonDown (EMouseButton mouseButton);

		static Vector2 GetMousePosition ();
		static Vector2 GetMouseDelta ();

		static float GetMouseWheelScrollDelta ();
	};
}

#endif