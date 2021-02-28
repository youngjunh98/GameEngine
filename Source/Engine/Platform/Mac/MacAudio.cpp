#include "MacAudio.h"

namespace GameEngine
{
    namespace Platform
    {
        MacAudio::MacAudio()
        {
        }

        MacAudio::~MacAudio()
        {
        }

        bool MacAudio::Initialize (uint32 desiredBufferFrames)
        {
            return true;
        }

        void MacAudio::Shutdown ()
        {
            GenericAudio::Shutdown ();
        }

        void MacAudio::LockAudioThread ()
        {
        }

        void MacAudio::UnlockAudioThread ()
        {
        }
    }
}
