#include <cstring>
#include <mutex>

#include <Windows.h>
#include <wrl.h>
#include <mmdeviceapi.h>
#include <Audioclient.h>
#include <audiopolicy.h>

#define REFTIMES_PER_SEC  10000000
#define REFTIMES_PER_MILLISEC  10000

#include "Platform/Platform.h"
#include "Sound/Sound.h"
#include "Sound/SoundClip.h"

namespace GameEngine
{
	Microsoft::WRL::ComPtr<IMMDeviceEnumerator> deviceEnumerator;
	Microsoft::WRL::ComPtr<IMMDevice> device;
	Microsoft::WRL::ComPtr<IAudioClient> audioClient;
	Microsoft::WRL::ComPtr<IAudioRenderClient> audioRenderClient;
	UINT bufferFrameCount;
	WAVEFORMATEX* waveFormat;
	std::unique_ptr<PlatformThread> soundThread;
	CRITICAL_SECTION cs;

	Sound g_sound;

	int32 SoundThreadFunction (void* thread, int32 threadId, void* data)
	{
		Sound* sound = reinterpret_cast<Sound*> (data);
		DWORD flags = 0;
		BYTE* audioData;

		if (FAILED (audioRenderClient->GetBuffer (bufferFrameCount, &audioData)))
		{
			return 0;
		}

		DWORD bufferFlags = AUDCLNT_BUFFERFLAGS_SILENT;

		if (FAILED (audioRenderClient->ReleaseBuffer (bufferFrameCount, bufferFlags)))
		{
			return 0;
		}

		if (FAILED (audioClient->Start ()))
		{
			return 0;
		}

		// 재생 코드
		UINT framesPerSec = bufferFrameCount / waveFormat->nSamplesPerSec;
		REFERENCE_TIME hnsActualDuration = REFTIMES_PER_SEC * static_cast<REFERENCE_TIME> (framesPerSec);

		while (true)
		{
			Sleep (static_cast<DWORD> (hnsActualDuration / REFTIMES_PER_MILLISEC / 2));

			UINT numFramesPadding;

			if (FAILED (audioClient->GetCurrentPadding (&numFramesPadding)))
			{
				return 0;
			}

			UINT numFramesAvailable = bufferFrameCount - numFramesPadding;

			if (FAILED (audioRenderClient->GetBuffer (numFramesAvailable, &audioData)))
			{
				return 0;
			}

			// 믹스 코드
			UINT initBytes = static_cast<UINT> (waveFormat->nBlockAlign) * numFramesAvailable;
			std::memset (audioData, 0, initBytes);

			EnterCriticalSection (&cs);

			if (sound->m_bPlay == false)
			{
				LeaveCriticalSection (&cs);

				if (FAILED (audioRenderClient->ReleaseBuffer (numFramesAvailable, flags)))
				{
					return 0;
				}

				break;
			}

			auto track = sound->m_tracks.begin ();

			while (track != sound->m_tracks.end ())
			{
				if (track->m_bFinished)
				{
					track = sound->m_tracks.erase (track);
					continue;
				}

				const auto* clip = track->m_soundClip;
				const auto* clipSamples = clip->GetSampleData ();
				float pitch = clip->SampleRate () / waveFormat->nSamplesPerSec;

				int32 clipFrameCount = clip->FrameCount ();
				int32 clipChannelCount = clip->ChannelCount ();

				for (int32 frameIndex = 0; frameIndex < numFramesAvailable; frameIndex++)
				{
					int32 currentTrackFrame = static_cast<int32> (track->m_framesPlayed);

					if (currentTrackFrame >= clipFrameCount)
					{
						track->m_bFinished = true;
						break;
					}

					for (int32 channel = 0; channel < clipChannelCount; channel++)
					{
						float* data = reinterpret_cast<float*> (audioData);
						int32 audioFrame = frameIndex * waveFormat->nChannels + channel;
						int32 clipFrame = currentTrackFrame * clipChannelCount + channel;
						data[audioFrame] += clipSamples[clipFrame];
					}

					track->m_framesPlayed += pitch;
				}

				track += 1;
			}

			LeaveCriticalSection (&cs);

			if (FAILED (audioRenderClient->ReleaseBuffer (numFramesAvailable, flags)))
			{
				return 0;
			}
		}

		Sleep (static_cast<DWORD> (hnsActualDuration / REFTIMES_PER_MILLISEC / 2));

		if (FAILED (audioClient->Stop ()))
		{
			return 0;
		}
		
		return 0;
	}

	Sound::Sound () :
		m_mixSampleCount (0),
		m_channelCount (0),
		m_sampleRate (0.0f),
		m_bPlay (false)
	{
	}

	Sound::~Sound ()
	{
	}

	bool Sound::Initialize (int32 mixSampleCount, int32 channelCount, float sampleRate)
	{
		// 시작 코드
		if (FAILED (CoCreateInstance (__uuidof (MMDeviceEnumerator), nullptr, CLSCTX_ALL, __uuidof (IMMDeviceEnumerator), reinterpret_cast<void**> (deviceEnumerator.ReleaseAndGetAddressOf ()))))
		{
			return false;
		}

		if (FAILED (deviceEnumerator->GetDefaultAudioEndpoint (EDataFlow::eRender, ERole::eConsole, device.ReleaseAndGetAddressOf ())))
		{
			return false;
		}

		if (FAILED (device->Activate (__uuidof(IAudioClient), CLSCTX_ALL, nullptr, reinterpret_cast<void**> (audioClient.ReleaseAndGetAddressOf ()))))
		{
			return false;
		}

		if (FAILED (audioClient->GetMixFormat (&waveFormat)))
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

		REFERENCE_TIME hnsRequestDuration = REFTIMES_PER_SEC;
		DWORD streamFlags = 0;

		if (FAILED (audioClient->Initialize (AUDCLNT_SHAREMODE_SHARED, streamFlags, hnsRequestDuration, 0, waveFormat, nullptr)))
		{
			return false;
		}

		if (FAILED (audioClient->GetBufferSize (&bufferFrameCount)))
		{
			return false;
		}

		if (FAILED (audioClient->GetService (__uuidof (IAudioRenderClient), reinterpret_cast<void**> (audioRenderClient.ReleaseAndGetAddressOf ()))))
		{
			return false;
		}

		if (InitializeCriticalSectionEx (&cs, 0, 0) == false)
		{
			return false;
		}

		m_bPlay = true;
		soundThread = std::make_unique<PlatformThread> (SoundThreadFunction, this);

		return true;
	}

	void Sound::Shutdown ()
	{
		EnterCriticalSection (&cs);
		m_bPlay = false;
		LeaveCriticalSection (&cs);

		soundThread->Join ();

		DeleteCriticalSection (&cs);

		// 종료 코드
		CoTaskMemFree (waveFormat);
		waveFormat = nullptr;

		deviceEnumerator = nullptr;
		device = nullptr;
		audioClient = nullptr;
		audioRenderClient = nullptr;
	}

	void Sound::Play (SoundClip& clip)
	{
		SoundTrack track;
		track.m_soundClip = &clip;
		track.m_framesPlayed = 0.0f;
		track.m_bFinished = false;

		EnterCriticalSection (&cs);
		m_tracks.push_back (track);
		LeaveCriticalSection (&cs);
	}

	float Sound::LinearVolumeToDecibel (float volume)
	{
		return 20.0f * Math::Log10 (Math::Max (0.0000001f, Math::Absolute (volume)));;
	}

	float Sound::DecibelToLinearVolume (float decibel)
	{
		return Math::Pow (10.0f, decibel * 0.05f);
	}
}