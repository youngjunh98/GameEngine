#include <cstring>

#include "Sound/SoundClip.h"

namespace GameEngine
{
	SoundClip::SoundClip ()
	{
	}

	SoundClip::~SoundClip ()
	{
	}

	int32 SoundClip::FrameCount () const
	{
		return m_frameCount;
	}

	int32 SoundClip::ChannelCount () const
	{
		return m_channelCount;
	}

	float SoundClip::SampleRate () const
	{
		return m_sampleRate;
	}

	float SoundClip::Length () const
	{
		float length = 0.0f;

		if (m_sampleRate <= 0)
		{
			length = static_cast<float> (m_frameCount) / m_sampleRate;
		}

		return length;
	}

	//void SoundClip::ReadNextSamples (float* buffer, int32 sampleCount, int32 channelCount, float sampleRate)
	//{
	//	float pitch = m_sampleRate / sampleRate;

	//	for (int32 sampleIndex = 0; sampleIndex < sampleCount; sampleIndex++)
	//	{
	//		int32 sampleOffset = static_cast<int32> (m_playSamples) * m_channelCount;
	//		int32 bufferOffset = sampleIndex * channelCount;

	//		for (int32 channel = 0; channel < channelCount; channel++)
	//		{
	//			float sample = 0.0f;
	//			int32 sampleDataIndex = sampleOffset + channel;

	//			if (sampleDataIndex < m_sampleData.size () && channel < m_channelCount)
	//			{
	//				sample = m_sampleData[sampleDataIndex];
	//			}

	//			buffer[bufferOffset + channel] = sample;
	//		}

	//		m_playSamples += pitch;
	//	}
	//}

	const float* SoundClip::GetSampleData () const
	{
		return m_sampleData.data ();
	}

	void SoundClip::SetSampleData (const std::vector<float>& data, int32 channelCount, float sampleRate)
	{
		m_sampleData = data;
		m_frameCount = static_cast<int32> (data.size ()) / channelCount;
		m_sampleRate = sampleRate;
		m_channelCount = channelCount;
	}
}