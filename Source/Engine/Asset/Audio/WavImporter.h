#ifndef INCLUDE_WAVE_IMPORTER
#define INCLUDE_WAVE_IMPORTER

#include <string>

#include "Core/CoreMinimal.h"

namespace GameEngine
{
	class WavImporter
	{
	public:
		static bool Import (AudioClip& audioClip, const int8* data, const int64 dataSize);
	};
}

#endif