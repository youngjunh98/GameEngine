#include "GenericAudio.h"

namespace GameEngine
{
    AudioTrack::AudioTrack (const float* sampleData, int32 channelCount, int32 sampleFrameCount, float sampleRate) :
        m_sampleData (sampleData), m_channelCount (channelCount), m_sampleFrameCount (sampleFrameCount),
        m_sampleRate (sampleRate), m_framesPlayed (0.0f)
    {
    }

    AudioTrack::~AudioTrack () 
    {
        m_sampleData = nullptr;
        m_channelCount = 0;
        m_sampleFrameCount = 0;
        m_sampleRate = 0.0f;
        m_framesPlayed = 0.0f;
    }

    bool AudioTrack::ReadNextSamples (byte* buffer, int32 channelCount, int32 sampleFrameCount, float sampleRate, uint32 sampleBitSize)
    {
        float clipSampleRate = m_sampleRate;
        float pitch = clipSampleRate / sampleRate;

        int32 clipChannelCount = m_channelCount;
        int32 clipFrameCount = m_sampleFrameCount;
        const float* clipSampleData = m_sampleData;

		for (int32 copyFrame = 0; copyFrame < sampleFrameCount; copyFrame++)
		{
			int32 currentFrame = static_cast<int32> (m_framesPlayed);

			if (currentFrame >= clipFrameCount)
			{
				return true;
			}

			for (int32 copyChannel = 0; copyChannel < channelCount; copyChannel++)
			{
				int32 copyDestinationFrame = copyFrame * channelCount + copyChannel;
				int32 copySourceFrame = currentFrame * clipChannelCount + copyChannel;
                float sample = clipSampleData[copySourceFrame];

                if (sampleBitSize == 8)
                {
                    auto* copyDestination = reinterpret_cast<int8*> (buffer);
                    copyDestination[copyDestinationFrame] += static_cast<int8> (sample * 128.0f);
                }
                else if (sampleBitSize == 16)
                {
                    auto* copyDestination = reinterpret_cast<int16*> (buffer);
                    copyDestination[copyDestinationFrame] += static_cast<int16> (sample * 32768.0f);
                }
                else if (sampleBitSize == 32)
                {
                    auto* copyDestination = reinterpret_cast<float*> (buffer);
                    copyDestination[copyDestinationFrame] += sample;
                }				
			}

			m_framesPlayed += pitch;
		}

        return false;
    }

    namespace Platform
    {
        GenericAudio::GenericAudio () : m_bKeepRunning (false), m_bufferFrameCount (0),
            m_channelCount (0), m_sampleRate (0.0f), m_sampleFrameBytes (0)
        {
        }

        GenericAudio::~GenericAudio ()
        {
        }

        void GenericAudio::Shutdown ()
        {
            m_bufferFrameCount = 0;
            m_channelCount = 0;
            m_sampleRate = 0.0f;
            m_sampleFrameBytes = 0;

            m_tracks.clear ();
        }

        void GenericAudio::AddTrackNoLock (const AudioTrack& track)
        {
            LockAudioThread ();
            m_tracks.push_back (track);
            UnlockAudioThread ();
        }
    }
}