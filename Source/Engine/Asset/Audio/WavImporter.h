#pragma once

#include <string>

#include "Type.h"
#include "Engine/Asset/AssetData.h"

namespace GameEngine
{
	class WavImporter
	{
	public:
		static AudioData Import (const uint8* data, const int64 dataSize);
	};
}
