#include "Engine/Core/Input/Input.h"
#include "Engine/Platform/Platform.h"

namespace GameEngine
{
	bool Input::GetKey (EKeyCode keyCode)
	{
		return Platform::GetGenericInput ().GetKey (keyCode);
	}

	bool Input::GetKeyUp (EKeyCode keyCode)
	{
		return Platform::GetGenericInput ().GetKeyUp (keyCode);
	}

	bool Input::GetKeyDown (EKeyCode keyCode)
	{
		return Platform::GetGenericInput ().GetKeyDown (keyCode);
	}

	bool Input::GetMouseButton (EMouseButton mouseButton)
	{
		return Platform::GetGenericInput ().GetMouseButton (mouseButton);
	}

	bool Input::GetMouseButtonUp (EMouseButton mouseButton)
	{
		return Platform::GetGenericInput ().GetMouseButtonUp (mouseButton);
	}

	bool Input::GetMouseButtonDown (EMouseButton mouseButton)
	{
		return Platform::GetGenericInput ().GetMouseButtonDown (mouseButton);
	}

	Vector2 Input::GetMousePosition ()
	{
		auto& platformInput = Platform::GetGenericInput ();
		return Vector2 (platformInput.GetMousePositionX (), platformInput.GetMousePositionY ());
	}

	Vector2 Input::GetMouseDelta ()
	{
		auto& platformInput = Platform::GetGenericInput ();
		return Vector2 (platformInput.GetMouseDeltaX (), platformInput.GetMouseDeltaY ());
	}

	float Input::GetMouseWheelScrollDelta ()
	{
		return Platform::GetGenericInput ().GetMouseWheelScrollDelta ();
	}
}
