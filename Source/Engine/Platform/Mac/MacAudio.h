#pragma once

#include "Type.h"
#include "Engine/Platform/PlatformMacro.h"
#include "Engine/Platform/Generic/GenericAudio.h"

namespace GameEngine
{
    namespace Platform
    {
        class MacAudio : public GenericAudio
        {
        public:
            ENGINE_PLATFORM_API MacAudio ();
            ENGINE_PLATFORM_API virtual ~MacAudio ();

            ENGINE_PLATFORM_API virtual bool Initialize (uint32 desiredBufferFrames) override;
            ENGINE_PLATFORM_API virtual void Shutdown () override;

            ENGINE_PLATFORM_API virtual void LockAudioThread () override;
            ENGINE_PLATFORM_API virtual void UnlockAudioThread () override;

        protected:
        };
    }
}
