#pragma once

#include <vector>

#include "Engine/Core/CoreMinimal.h"
#include "Engine/Core/Object/Object.h"

namespace GameEngine
{
	class AudioClip : public Object
	{
	public:
		REGISTER_OBJECT_HEADER (AudioClip)

		AudioClip ();
		virtual ~AudioClip ();

		int32 ChannelCount () const;
		int32 SampleFrameCount () const;
		float SampleRate () const;
		float Length () const;

		const float* GetSampleData () const;
		void SetSampleData (const std::vector<float>& data, int32 channelCount, float sampleRate);

	private:
		int32 m_channelCount;
		int32 m_sampleFrameCount;
		float m_sampleRate;
		std::vector<float> m_sampleData;
	};
}
