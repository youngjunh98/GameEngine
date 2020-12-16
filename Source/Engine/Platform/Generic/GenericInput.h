#pragma once

#include "Type.h"
#include "Engine/Platform/PlatformMacro.h"
#include "Engine/Core/Input/InputEnum.h"

namespace GameEngine
{
	namespace Platform
	{
		class ENGINE_PLATFORM_API GenericInput
		{
		public:
			GenericInput ();
			virtual ~GenericInput () = 0;

			virtual bool Initialize () = 0;
			virtual void Shutdown () = 0;

			void Update ();

			bool GetKey (EKeyCode keyCode) const;
			bool GetKeyUp (EKeyCode keyCode) const;
			bool GetKeyDown (EKeyCode keyCode) const;

			bool GetMouseButton (EMouseButton mouseButton) const;
			bool GetMouseButtonUp (EMouseButton mouseButton) const;
			bool GetMouseButtonDown (EMouseButton mouseButton) const;

			float GetMousePositionX () const;
			float GetMousePositionY () const;

			float GetMouseDeltaX () const;
			float GetMouseDeltaY () const;

			float GetMouseWheelScrollDelta () const;

		protected:
			void KeyDown (EKeyCode keyCode);
			void KeyUp (EKeyCode keyCode);

			void MouseButtonDown (EMouseButton mouseButton);
			void MouseButtonUp (EMouseButton mouseButton);

			void MousePosition (float x, float y);
			void MouseDelta (float deltaX, float deltaY);

			void MouseWheelScrollDelta (float scrollDeta);

		private:
			enum class State
			{
				Up,
				Down
			};

			State m_keyStates[256];
			State m_prevKeyStates[256];

			State m_mouseButtonStates[3];
			State m_prevMouseButtonStates[3];

			float m_mousePositionX;
			float m_mousePositionY;

			float m_mouseDeltaX;
			float m_mouseDeltaY;
			
			float m_mouseWheelScrollDelta;
		};
	}
}
