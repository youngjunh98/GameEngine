#ifndef INCLUDE_SOUND
#define INCLUDE_SOUND

#include <memory>
#include <vector>

#include "Core/CoreMinimal.h"

namespace GameEngine
{
	class Sound
	{
	public:
		Sound ();
		virtual ~Sound ();

		bool Initialize (int32 mixSampleCount, int32 channelCount, float sampleRate);
		void Shutdown ();

		void Play (SoundClip& clip);

		static float LinearVolumeToDecibel (float volume);
		static float DecibelToLinearVolume (float decibel);

		//friend int SoundThread (void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames, double streamTime, RtAudioStreamStatus status, void *userData);
		friend int32 SoundThreadFunction (void* thread, int32 threadId, void* data);

	private:
		struct SoundTrack
		{
		public:
			SoundClip* m_soundClip;
			float m_framesPlayed;
			bool m_bFinished;
		};

		int32 m_mixSampleCount;
		int32 m_channelCount;
		float m_sampleRate;
		bool m_bPlay;

		std::unique_ptr<float[]> m_mixBuffer;
		std::vector<SoundTrack> m_tracks;
	};

	extern class Sound g_sound;
}

#endif