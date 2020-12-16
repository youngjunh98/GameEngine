#pragma once

#include <string>

#include "Type.h"
#include "Engine/Core/CoreMacro.h"
#include "Engine/Engine/Asset/AssetData.h"

namespace GameEngine
{
	class ENGINE_CORE_API WavImporter
	{
	public:
		static AudioData Import (const int8* data, const int64 dataSize);
	};
}
