#pragma once

#include <Windows.h>
#include <wrl.h>
#include <Audioclient.h>

#include "EngineType.h"
#include "Platform/PlatformMacro.h"
#include "Platform/Generic/GenericAudio.h"

namespace GameEngine
{
    namespace Platform
    {
        class WindowsAudio : public GenericAudio
        {
        public:
            PLATFORM_API WindowsAudio ();
            PLATFORM_API virtual ~WindowsAudio ();

            PLATFORM_API virtual bool Initialize (uint32 desiredBufferFrames) override;
            PLATFORM_API virtual void Shutdown () override;

            PLATFORM_API virtual void LockAudioThread () override;
            PLATFORM_API virtual void UnlockAudioThread () override;

            friend int32 AudioThreadFunction (void* thread, int32 threadId, void* data);

        protected:
            Microsoft::WRL::ComPtr<IAudioClient> m_audioClient;
            Microsoft::WRL::ComPtr<IAudioRenderClient> m_audioRenderClient;
        };
    }
}