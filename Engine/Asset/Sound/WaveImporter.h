#ifndef INCLUDE_WAVE_IMPORTER
#define INCLUDE_WAVE_IMPORTER

#include <string>

#include "Core/CoreMinimal.h"

namespace GameEngine
{
	class WaveImporter
	{
	public:
		static bool Import (SoundClip& soundClip, const int8* data, const int64 dataSize);
	};
}

#endif