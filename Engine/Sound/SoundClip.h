#ifndef INCLUDE_SOUND_CLIP
#define INCLUDE_SOUND_CLIP

#include <vector>

#include "Core/CoreMinimal.h"
#include "Object.h"

namespace GameEngine
{
	class SoundClip : public Object
	{
	public:
		SoundClip ();
		virtual ~SoundClip ();

		int32 FrameCount () const;
		int32 ChannelCount () const;
		float SampleRate () const;
		float Length () const;

		//void ReadNextSamples (float* buffer, int32 sampleCount, int32 channelCount, float sampleRate);

		const float* GetSampleData () const;
		void SetSampleData (const std::vector<float>& data, int32 channelCount, float sampleRate);

	private:
		int32 m_frameCount;
		int32 m_channelCount;
		float m_sampleRate;
		std::vector<float> m_sampleData;
	};
}

#endif