#include "Platform/Generic/GenericPlatformTimer.h"

namespace GameEngine
{
	void GenericPlatformTimer::Reset (float fixedDeltaTime)
	{
		m_time = 0.0f;
		m_deltaTime = 0.0f;

		m_accumulatedTime = 0.0f;
		m_fixedDeltaTime = fixedDeltaTime;
	}

	float GenericPlatformTimer::GetTime () const
	{
		return m_time;
	}

	float GenericPlatformTimer::GetDeltaTime () const
	{
		return m_deltaTime;
	}

	float GenericPlatformTimer::GetFixedDeltaTime () const
	{
		return m_fixedDeltaTime;
	}

	float GenericPlatformTimer::GetAccumulatedTime () const
	{
		return m_accumulatedTime;
	}

	void GenericPlatformTimer::SetAccumulatedTime (float accumulatedTime)
	{
		m_accumulatedTime = accumulatedTime;
	}

	void GenericPlatformTimer::UpdateInternal (float time, float deltaTime)
	{
		m_time = time;
		m_deltaTime = deltaTime > 0.0f ? deltaTime : 0.0f;
	}
}