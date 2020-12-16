#include "Engine/Platform/Generic/GenericTimer.h"

namespace GameEngine
{
	namespace Platform
	{
		GenericTimer::GenericTimer () : m_time (0.0f), m_deltaTime (0.0f),
			m_accumulatedTime (0.0f), m_fixedDeltaTime (0.0f)
		{}

		GenericTimer::~GenericTimer ()
		{}

		void GenericTimer::Reset (float fixedDeltaTime)
		{
			m_time = 0.0f;
			m_deltaTime = 0.0f;

			m_accumulatedTime = 0.0f;
			m_fixedDeltaTime = fixedDeltaTime;
		}

		float GenericTimer::GetTime () const
		{
			return m_time;
		}

		float GenericTimer::GetDeltaTime () const
		{
			return m_deltaTime;
		}

		float GenericTimer::GetFixedDeltaTime () const
		{
			return m_fixedDeltaTime;
		}

		float GenericTimer::GetAccumulatedTime () const
		{
			return m_accumulatedTime;
		}

		void GenericTimer::SetAccumulatedTime (float accumulatedTime)
		{
			m_accumulatedTime = accumulatedTime;
		}

		void GenericTimer::UpdateInternal (float time, float deltaTime)
		{
			m_time = time;
			m_deltaTime = deltaTime > 0.0f ? deltaTime : 0.0f;
		}
	}
}