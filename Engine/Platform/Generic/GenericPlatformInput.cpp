#include <cstring>

#include "Platform/Generic/GenericPlatformInput.h"

namespace GameEngine
{
	GenericPlatformInput::GenericPlatformInput () :
		m_mousePositionX (0.0f),
		m_mousePositionY (0.0f),
		m_mouseDeltaX (0.0f),
		m_mouseDeltaY (0.0f),
		m_mouseWheelScrollDelta (0.0f)
	{}

	GenericPlatformInput::~GenericPlatformInput ()
	{}

	void GenericPlatformInput::Update ()
	{
		static_assert (sizeof (m_keyStates) == sizeof (m_prevKeyStates), "Size of key states and previous states are different!");
		static_assert (sizeof (m_mouseButtonStates) == sizeof (m_prevMouseButtonStates), "Size of mouse button states and previous states are different!");

		std::memcpy (m_prevKeyStates, m_keyStates, sizeof (m_prevKeyStates));
		std::memcpy (m_prevMouseButtonStates, m_mouseButtonStates, sizeof (m_prevMouseButtonStates));

		m_mouseDeltaX = 0.0f;
		m_mouseDeltaY = 0.0f;

		m_mouseWheelScrollDelta = 0.0f;
	}

	bool GenericPlatformInput::GetKey (EKeyCode keyCode) const
	{
		if (keyCode == EKeyCode::None)
		{
			return false;
		}

		int32 index = static_cast<int32> (keyCode);

		return m_keyStates[index] == State::Down;
	}

	bool GenericPlatformInput::GetKeyUp (EKeyCode keyCode) const
	{
		if (keyCode == EKeyCode::None)
		{
			return false;
		}

		int32 index = static_cast<int32> (keyCode);

		return m_prevKeyStates[index] == State::Down && m_keyStates[index] == State::Up;
	}

	bool GenericPlatformInput::GetKeyDown (EKeyCode keyCode) const
	{
		if (keyCode == EKeyCode::None)
		{
			return false;
		}

		uint32 index = static_cast<uint32> (keyCode);

		return m_prevKeyStates[index] == State::Up && m_keyStates[index] == State::Down;
	}

	bool GenericPlatformInput::GetMouseButton (EMouseButton mouseButton) const
	{
		if (mouseButton == EMouseButton::Left)
		{
			return m_mouseButtonStates[0] == State::Down;
		}
		else if (mouseButton == EMouseButton::Middle)
		{
			return m_mouseButtonStates[1] == State::Down;
		}
		else if (mouseButton == EMouseButton::Right)
		{
			return m_mouseButtonStates[2] == State::Down;
		}

		return false;
	}

	bool GenericPlatformInput::GetMouseButtonUp (EMouseButton mouseButton) const
	{
		if (mouseButton == EMouseButton::Left)
		{
			return m_prevMouseButtonStates[0] == State::Down && m_mouseButtonStates[0] == State::Up;
		}
		else if (mouseButton == EMouseButton::Middle)
		{
			return m_prevMouseButtonStates[1] == State::Down && m_mouseButtonStates[1] == State::Up;
		}
		else if (mouseButton == EMouseButton::Right)
		{
			return m_prevMouseButtonStates[2] == State::Down && m_mouseButtonStates[2] == State::Up;
		}

		return false;
	}

	bool GenericPlatformInput::GetMouseButtonDown (EMouseButton mouseButton) const
	{
		if (mouseButton == EMouseButton::Left)
		{
			return m_prevMouseButtonStates[0] == State::Up && m_mouseButtonStates[0] == State::Down;
		}
		else if (mouseButton == EMouseButton::Middle)
		{
			return m_prevMouseButtonStates[1] == State::Up && m_mouseButtonStates[1] == State::Down;
		}
		else if (mouseButton == EMouseButton::Right)
		{
			return m_prevMouseButtonStates[2] == State::Up && m_mouseButtonStates[2] == State::Down;
		}

		return false;
	}

	float GenericPlatformInput::GetMousePositionX () const
	{
		return m_mousePositionX;
	}

	float GenericPlatformInput::GetMousePositionY () const
	{
		return m_mousePositionY;
	}

	float GenericPlatformInput::GetMouseDeltaX () const
	{
		return m_mouseDeltaX;
	}

	float GenericPlatformInput::GetMouseDeltaY () const
	{
		return m_mouseDeltaY;
	}

	float GenericPlatformInput::GetMouseWheelScrollDelta () const
	{
		return m_mouseWheelScrollDelta;
	}

	void GenericPlatformInput::KeyDown (EKeyCode keyCode)
	{
		if (keyCode == EKeyCode::None)
		{
			return;
		}

		int32 index = static_cast<int32> (keyCode);
		m_keyStates[index] = State::Down;
	}

	void GenericPlatformInput::KeyUp (EKeyCode keyCode)
	{
		if (keyCode == EKeyCode::None)
		{
			return;
		}

		int32 index = static_cast<int32> (keyCode);
		m_keyStates[index] = State::Up;
	}

	void GenericPlatformInput::MouseButtonDown (EMouseButton mouseButton)
	{
		if (mouseButton == EMouseButton::Left)
		{
			m_mouseButtonStates[0] = State::Down;
		}
		else if (mouseButton == EMouseButton::Middle)
		{
			m_mouseButtonStates[1] = State::Down;
		}
		else if (mouseButton == EMouseButton::Right)
		{
			m_mouseButtonStates[2] = State::Down;
		}
	}

	void GenericPlatformInput::MouseButtonUp (EMouseButton mouseButton)
	{
		if (mouseButton == EMouseButton::Left)
		{
			m_mouseButtonStates[0] = State::Up;
		}
		else if (mouseButton == EMouseButton::Middle)
		{
			m_mouseButtonStates[1] = State::Up;
		}
		else if (mouseButton == EMouseButton::Right)
		{
			m_mouseButtonStates[2] = State::Up;
		}
	}

	void GenericPlatformInput::MousePosition (float x, float y)
	{
		m_mousePositionX = x;
		m_mousePositionY = y;
	}

	void GenericPlatformInput::MouseDelta (float deltaX, float deltaY)
	{
		m_mouseDeltaX = deltaX;
		m_mouseDeltaY = deltaY;
	}

	void GenericPlatformInput::MouseWheelScrollDelta (float scrollDeta)
	{
		m_mouseWheelScrollDelta = scrollDeta;
	}
}
