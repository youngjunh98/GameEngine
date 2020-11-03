#pragma once

#include "Core/CoreType.h"
#include "Platform/PlatformMacro.h"

namespace GameEngine
{
	class PLATFORM_API GenericPlatformTimer
	{
	public:
		GenericPlatformTimer () :
			m_time (0.0f),
			m_deltaTime (0.0f),
			m_accumulatedTime (0.0f),
			m_fixedDeltaTime (0.0f)
		{}
		virtual ~GenericPlatformTimer () = 0 {}

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