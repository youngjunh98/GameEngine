#pragma once

#include "Type.h"
#include "Platform/PlatformMacro.h"

namespace GameEngine
{
	namespace Platform
	{
		class PLATFORM_API GenericTimer
		{
		public:
			GenericTimer ();
			virtual ~GenericTimer () = 0;

			virtual void Reset (float fixedDeltaTime);
			virtual void Tick () = 0;
			
			float GetTime () const;
			float GetDeltaTime () const;
			float GetFixedDeltaTime () const;

			float GetAccumulatedTime () const;
			void SetAccumulatedTime (float accumulatedTime);

		protected:
			void UpdateInternal (float time, float deltaTime);

		private:
			float m_time;
			float m_deltaTime;
			float m_fixedDeltaTime;
			float m_accumulatedTime;
		};
	}
}