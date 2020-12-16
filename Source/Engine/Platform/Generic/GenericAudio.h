#pragma once

#include <vector>

#include "Type.h"
#include "Engine/Platform/PlatformMacro.h"

namespace GameEngine
{
    class ENGINE_PLATFORM_API AudioTrack
    {
    public:
        AudioTrack (const float* sampleData, int32 channelCount, int32 sampleFrameCount, float sampleRate);
        virtual ~AudioTrack ();

        bool ReadNextSamples (byte* buffer, int32 channelCount, int32 sampleFrameCount, float sampleRate, uint32 sampleBitSize);
        
    private:
        const float* m_sampleData;
        int32 m_channelCount;
        int32 m_sampleFrameCount;
        float m_sampleRate;
        float m_framesPlayed;
    };
    
    namespace Platform
    {
        class GenericAudio
        {
        public:
            ENGINE_PLATFORM_API GenericAudio ();
            ENGINE_PLATFORM_API virtual ~GenericAudio () = 0;

            ENGINE_PLATFORM_API virtual bool Initialize (uint32 desiredBufferFrames) = 0;
            ENGINE_PLATFORM_API virtual void Shutdown ();

            ENGINE_PLATFORM_API virtual void LockAudioThread () = 0;
            ENGINE_PLATFORM_API virtual void UnlockAudioThread () = 0;

            ENGINE_PLATFORM_API void AddTrackNoLock (const AudioTrack& track);

        protected:
            bool m_bKeepRunning;
            uint32 m_bufferFrameCount;
            int32 m_channelCount;
            float m_sampleRate;
            uint32 m_sampleFrameBytes;

            std::vector<AudioTrack> m_tracks;
        };
    }
}