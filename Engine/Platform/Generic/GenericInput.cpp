#include <cstring>

#include "Platform/Generic/GenericInput.h"

namespace GameEngine
{
	namespace Platform
	{
		GenericInput::GenericInput () : m_keyStates { State::Up }, m_prevKeyStates { State::Up },
			m_mouseButtonStates { State::Up }, m_prevMouseButtonStates { State::Up },
			m_mousePositionX (0.0f), m_mousePositionY (0.0f),
			m_mouseDeltaX (0.0f), m_mouseDeltaY (0.0f),
			m_mouseWheelScrollDelta (0.0f)
		{
		}

		GenericInput::~GenericInput ()
		{
		}

		void GenericInput::Update ()
		{
			static_assert (sizeof (m_keyStates) == sizeof (m_prevKeyStates), "Size of key states and previous states are different!");
			static_assert (sizeof (m_mouseButtonStates) == sizeof (m_prevMouseButtonStates), "Size of mouse button states and previous states are different!");

			std::memcpy (m_prevKeyStates, m_keyStates, sizeof (m_prevKeyStates));
			std::memcpy (m_prevMouseButtonStates, m_mouseButtonStates, sizeof (m_prevMouseButtonStates));

			m_mouseDeltaX = 0.0f;
			m_mouseDeltaY = 0.0f;

			m_mouseWheelScrollDelta = 0.0f;
		}

		bool GenericInput::GetKey (EKeyCode keyCode) const
		{
			return keyCode == EKeyCode::None ? false : m_keyStates[static_cast<int> (keyCode)] == State::Down;
		}

		bool GenericInput::GetKeyUp (EKeyCode keyCode) const
		{
			if (keyCode == EKeyCode::None)
			{
				return false;
			}

			auto index = static_cast<uint32> (keyCode);
			return m_prevKeyStates[index] == State::Down && m_keyStates[index] == State::Up;
		}

		bool GenericInput::GetKeyDown (EKeyCode keyCode) const
		{
			if (keyCode == EKeyCode::None)
			{
				return false;
			}

			auto index = static_cast<uint32> (keyCode);
			return m_prevKeyStates[index] == State::Up && m_keyStates[index] == State::Down;
		}

		bool GenericInput::GetMouseButton (EMouseButton mouseButton) const
		{
			State state = State::Up;

			if (mouseButton == EMouseButton::Left)
			{
				state = m_mouseButtonStates[0];
			}
			else if (mouseButton == EMouseButton::Middle)
			{
				state = m_mouseButtonStates[1];
			}
			else if (mouseButton == EMouseButton::Right)
			{
				state = m_mouseButtonStates[2];
			}

			return state == State::Down;
		}

		bool GenericInput::GetMouseButtonUp (EMouseButton mouseButton) const
		{
			State prevState = State::Up;
			State state = State::Up;

			if (mouseButton == EMouseButton::Left)
			{
				prevState = m_prevMouseButtonStates[0];
				state = m_mouseButtonStates[0];
			}
			else if (mouseButton == EMouseButton::Middle)
			{
				prevState = m_prevMouseButtonStates[1];
				state = m_mouseButtonStates[1];
			}
			else if (mouseButton == EMouseButton::Right)
			{
				prevState = m_prevMouseButtonStates[2];
				state = m_mouseButtonStates[2];
			}

			return prevState == State::Down && state == State::Up;
		}

		bool GenericInput::GetMouseButtonDown (EMouseButton mouseButton) const
		{
			State prevState = State::Up;
			State state = State::Up;

			if (mouseButton == EMouseButton::Left)
			{
				prevState = m_prevMouseButtonStates[0];
				state = m_mouseButtonStates[0];
			}
			else if (mouseButton == EMouseButton::Middle)
			{
				prevState = m_prevMouseButtonStates[1];
				state = m_mouseButtonStates[1];
			}
			else if (mouseButton == EMouseButton::Right)
			{
				prevState = m_prevMouseButtonStates[2];
				state = m_mouseButtonStates[2];
			}

			return prevState == State::Up && state == State::Down;
		}

		float GenericInput::GetMousePositionX () const
		{
			return m_mousePositionX;
		}

		float GenericInput::GetMousePositionY () const
		{
			return m_mousePositionY;
		}

		float GenericInput::GetMouseDeltaX () const
		{
			return m_mouseDeltaX;
		}

		float GenericInput::GetMouseDeltaY () const
		{
			return m_mouseDeltaY;
		}

		float GenericInput::GetMouseWheelScrollDelta () const
		{
			return m_mouseWheelScrollDelta;
		}

		void GenericInput::KeyDown (EKeyCode keyCode)
		{
			if (keyCode == EKeyCode::None)
			{
				return;
			}

			auto index = static_cast<uint32> (keyCode);
			m_keyStates[index] = State::Down;
		}

		void GenericInput::KeyUp (EKeyCode keyCode)
		{
			if (keyCode == EKeyCode::None)
			{
				return;
			}

			auto index = static_cast<uint32> (keyCode);
			m_keyStates[index] = State::Up;
		}

		void GenericInput::MouseButtonDown (EMouseButton mouseButton)
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

		void GenericInput::MouseButtonUp (EMouseButton mouseButton)
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

		void GenericInput::MousePosition (float x, float y)
		{
			m_mousePositionX = x;
			m_mousePositionY = y;
		}

		void GenericInput::MouseDelta (float deltaX, float deltaY)
		{
			m_mouseDeltaX = deltaX;
			m_mouseDeltaY = deltaY;
		}

		void GenericInput::MouseWheelScrollDelta (float scrollDeta)
		{
			m_mouseWheelScrollDelta = scrollDeta;
		}
	}
}
