#include <memory>
#include <mmdeviceapi.h>

#define REFTIMES_PER_SEC  10000000
#define REFTIMES_PER_MILLISEC  10000

#include "WindowsAudio.h"
#include "WindowsThread.h"

namespace GameEngine
{
    namespace Platform
    {
        static std::unique_ptr<WindowsThread> g_audioThread;
        static CRITICAL_SECTION g_audioThreadCS;

        int32 AudioThreadFunction (void* thread, int32 threadId, void* data)
        {
            auto* audio = reinterpret_cast<WindowsAudio*> (data);
            BYTE* audioBuffer;
            DWORD releaseBufferFlags = 0;

            Microsoft::WRL::ComPtr<IAudioClient> client = audio->m_audioClient;
	        Microsoft::WRL::ComPtr<IAudioRenderClient> renderClient = audio->m_audioRenderClient;

            UINT channelCount = static_cast<UINT> (audio->m_channelCount);
            UINT32 bufferFrameCount = audio->m_bufferFrameCount;
            UINT sampleBytes = audio->m_sampleFrameBytes / channelCount;

            if (FAILED (renderClient->GetBuffer (bufferFrameCount, &audioBuffer)))
            {
                return 0;
            }
            
            if (FAILED (renderClient->ReleaseBuffer (bufferFrameCount, AUDCLNT_BUFFERFLAGS_SILENT)))
            {
                return 0;
            }

            if (FAILED (client->Start ()))
            {
                return 0;
            }

            float sampleRate = audio->m_sampleRate;
            float bufferSeconds = static_cast<float> (bufferFrameCount) / sampleRate;
            float bufferRefTimes = bufferSeconds * static_cast<float> (REFTIMES_PER_SEC);
            REFERENCE_TIME hnsActualDuration = static_cast<REFERENCE_TIME> (bufferRefTimes);

            bool bShutdownAudio = false;

            while (bShutdownAudio == false)
            {
                Sleep (static_cast<DWORD> (hnsActualDuration / REFTIMES_PER_MILLISEC / 2));

                UINT numFramesPadding;

                if (FAILED (client->GetCurrentPadding (&numFramesPadding)))
                {
                    return 0;
                }

                UINT numFramesAvailable = bufferFrameCount - numFramesPadding;

                if (FAILED (renderClient->GetBuffer (numFramesAvailable, &audioBuffer)))
                {
                    return 0;
                }

                UINT initBytes = audio->m_sampleFrameBytes * numFramesAvailable;
                std::memset (audioBuffer, 0, initBytes);

                EnterCriticalSection (&g_audioThreadCS);

                if (audio->m_bKeepRunning)
                {
                    auto trackIter = audio->m_tracks.begin ();

                    while (trackIter != audio->m_tracks.end ())
                    {
                        bool bTrackFinished = trackIter->ReadNextSamples (audioBuffer, channelCount, numFramesAvailable, sampleRate, sampleBytes * 8);

                        if (bTrackFinished)
                        {
                            trackIter = audio->m_tracks.erase (trackIter);
                        }
                        else
                        {
                            ++trackIter;
                        }
                    }
                }
                else
                {
                    bShutdownAudio = true;
                }

                LeaveCriticalSection (&g_audioThreadCS);

                if (FAILED (renderClient->ReleaseBuffer (numFramesAvailable, releaseBufferFlags)))
                {
                    return 0;
                }
            }

            Sleep (static_cast<DWORD> (hnsActualDuration / REFTIMES_PER_MILLISEC / 2));
            client->Stop ();

            return 0;
        }

        WindowsAudio::WindowsAudio()
        {
        }

        WindowsAudio::~WindowsAudio()
        {
        }

        bool WindowsAudio::Initialize (uint32 desiredBufferFrames)
        {
            const CLSID CLSID_MMDeviceEnumerator = __uuidof(MMDeviceEnumerator);
            const IID IID_IMMDeviceEnumerator = __uuidof(IMMDeviceEnumerator);
            Microsoft::WRL::ComPtr<IMMDeviceEnumerator> deviceEnumerator;

            if (FAILED (CoCreateInstance (CLSID_MMDeviceEnumerator, nullptr, CLSCTX_ALL, IID_IMMDeviceEnumerator, reinterpret_cast<void**> (deviceEnumerator.ReleaseAndGetAddressOf ()))))
		    {
			    return false;
		    }

	        Microsoft::WRL::ComPtr<IMMDevice> device;

            if (FAILED (deviceEnumerator->GetDefaultAudioEndpoint (EDataFlow::eRender, ERole::eConsole, device.ReleaseAndGetAddressOf ())))
		    {
			    return false;
		    }

            const IID IID_IAudioClient = __uuidof(IAudioClient);

            if (FAILED (device->Activate (IID_IAudioClient, CLSCTX_ALL, nullptr, reinterpret_cast<void**> (m_audioClient.ReleaseAndGetAddressOf ()))))
            {
                return false;
            }

            WAVEFORMATEX* waveFormat;

            if (FAILED (m_audioClient->GetMixFormat (&waveFormat)))
            {
                return false;
            }

            bool bIsPCM = waveFormat->wFormatTag == WAVE_FORMAT_PCM;
            bool bisFloat = waveFormat->wFormatTag == WAVE_FORMAT_IEEE_FLOAT;
            const int minExtensibleSize = 22;

            if (waveFormat->wFormatTag == WAVE_FORMAT_EXTENSIBLE && waveFormat->cbSize >= minExtensibleSize)
            {
                WAVEFORMATEXTENSIBLE* waveFormatExtensible = reinterpret_cast<WAVEFORMATEXTENSIBLE*> (waveFormat);
                bIsPCM = waveFormatExtensible->SubFormat == KSDATAFORMAT_SUBTYPE_PCM;
                bisFloat = waveFormatExtensible->SubFormat == KSDATAFORMAT_SUBTYPE_IEEE_FLOAT;
            }

            if (bIsPCM == false && bisFloat == false)
            {
                return false;
            }

            float desiredBufferSeconds = static_cast<float> (desiredBufferFrames) / static_cast<float> (waveFormat->nSamplesPerSec);
            float desiredBufferRefTimes = desiredBufferSeconds * static_cast<float> (REFTIMES_PER_SEC);
            REFERENCE_TIME hnsRequestDuration = static_cast<REFERENCE_TIME> (desiredBufferRefTimes);
            DWORD streamFlags = 0;

            if (FAILED (m_audioClient->Initialize (AUDCLNT_SHAREMODE_SHARED, streamFlags, hnsRequestDuration, 0, waveFormat, nullptr)))
            {
                return false;
            }

            UINT bufferSize;

            if (FAILED (m_audioClient->GetBufferSize (&bufferSize)))
            {
                return false;
            }

            const IID IID_IAudioRenderClient = __uuidof(IAudioRenderClient);

            if (FAILED (m_audioClient->GetService (IID_IAudioRenderClient, reinterpret_cast<void**> (m_audioRenderClient.ReleaseAndGetAddressOf ()))))
            {
                return false;
            }

            if (InitializeCriticalSectionEx (&g_audioThreadCS, 0, 0) == false)
            {
                return false;
            }

            m_bKeepRunning = true;
            m_bufferFrameCount = bufferSize;
            m_channelCount = waveFormat->nChannels;
            m_sampleRate = static_cast<float> (waveFormat->nSamplesPerSec);
            m_sampleFrameBytes = waveFormat->nBlockAlign;

            CoTaskMemFree (waveFormat);

            g_audioThread = std::make_unique<WindowsThread> (AudioThreadFunction, this);

            if (g_audioThread == nullptr)
            {
                return false;
            }

            return true;
        }

        void WindowsAudio::Shutdown ()
        {
            if (g_audioThread != nullptr)
            {
                LockAudioThread ();

                m_bKeepRunning = false;

                UnlockAudioThread ();

                g_audioThread->Join ();
                g_audioThread = nullptr;
            }

            DeleteCriticalSection (&g_audioThreadCS);

            m_audioClient = nullptr;
            m_audioRenderClient = nullptr;

            GenericAudio::Shutdown ();
        }

        void WindowsAudio::LockAudioThread ()
        {
            EnterCriticalSection (&g_audioThreadCS);
        }

        void WindowsAudio::UnlockAudioThread ()
        {
            LeaveCriticalSection (&g_audioThreadCS);
        }
    }
}