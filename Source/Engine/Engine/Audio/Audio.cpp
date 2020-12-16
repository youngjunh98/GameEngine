#include "Audio.h"
#include "Engine/Platform/Platform.h"
#include "Engine/Core/Math/Math.h"

namespace GameEngine
{
	static Platform::Audio g_audio;
	static uint32 g_bufferSize = 102400;

	bool Audio::Initialize ()
	{
		return g_audio.Initialize (g_bufferSize);
	}

	void Audio::Shutdown ()
	{
		return g_audio.Shutdown ();
	}

	void Audio::PlayAudioClip (const AudioClip& clip)
	{
		AudioTrack track(clip.GetSampleData (), clip.ChannelCount (), clip.SampleFrameCount (), clip.SampleRate ());

		g_audio.LockAudioThread ();
		g_audio.AddTrackNoLock (track);
		g_audio.UnlockAudioThread ();
	}

	float Audio::LinearVolumeToDecibel (float volume)
	{
		return 20.0f * Math::Log10 (Math::Max (0.0000001f, Math::Absolute (volume)));;
	}

	float Audio::DecibelToLinearVolume (float decibel)
	{
		return Math::Pow (10.0f, decibel * 0.05f);
	}
}