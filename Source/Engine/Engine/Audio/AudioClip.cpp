#include "AudioClip.h"

namespace GameEngine
{
	REGISTER_OBJECT_CPP (AudioClip)

	AudioClip::AudioClip () : m_channelCount (0), m_sampleFrameCount (0), m_sampleRate(0.0f)
	{
	}

	AudioClip::~AudioClip ()
	{
		m_channelCount = 0;
		m_sampleFrameCount = 0;
		m_sampleRate = 0.0f;
		m_sampleData.clear ();
	}

	int32 AudioClip::ChannelCount () const
	{
		return m_channelCount;
	}

	int32 AudioClip::SampleFrameCount () const
	{
		return m_sampleFrameCount;
	}

	float AudioClip::SampleRate () const
	{
		return m_sampleRate;
	}

	float AudioClip::Length () const
	{
		float length = 0.0f;

		if (m_sampleRate <= 0)
		{
			length = static_cast<float> (m_sampleFrameCount) / m_sampleRate;
		}

		return length;
	}

	const float* AudioClip::GetSampleData () const
	{
		return m_sampleData.data ();
	}

	void AudioClip::SetSampleData (const std::vector<float>& data, int32 channelCount, float sampleRate)
	{
		m_sampleData = data;
		m_sampleFrameCount = static_cast<int32> (data.size ()) / channelCount;
		m_sampleRate = sampleRate;
		m_channelCount = channelCount;
	}
}