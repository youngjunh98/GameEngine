#pragma once

#include <Windows.h>
#include <wrl.h>
#include <Audioclient.h>

#include "Type.h"
#include "Engine/Platform/PlatformMacro.h"
#include "Engine/Platform/Generic/GenericAudio.h"

namespace GameEngine
{
    namespace Platform
    {
        class WindowsAudio : public GenericAudio
        {
        public:
            ENGINE_PLATFORM_API WindowsAudio ();
            ENGINE_PLATFORM_API virtual ~WindowsAudio ();

            ENGINE_PLATFORM_API virtual bool Initialize (uint32 desiredBufferFrames) override;
            ENGINE_PLATFORM_API virtual void Shutdown () override;

            ENGINE_PLATFORM_API virtual void LockAudioThread () override;
            ENGINE_PLATFORM_API virtual void UnlockAudioThread () override;

            friend int32 AudioThreadFunction (void* thread, int32 threadId, void* data);

        protected:
            Microsoft::WRL::ComPtr<IAudioClient> m_audioClient;
            Microsoft::WRL::ComPtr<IAudioRenderClient> m_audioRenderClient;
        };
    }
}