#pragma once

#include "EngineType.h"
#include "AudioClip.h"

namespace GameEngine
{
	class Audio
	{
	public:
		static bool Initialize ();
		static void Shutdown ();

		static void PlayAudioClip (const AudioClip& clip);

		static float LinearVolumeToDecibel (float volume);
		static float DecibelToLinearVolume (float decibel);

	private:
		Audio ();
		virtual ~Audio () = 0;
	};
}