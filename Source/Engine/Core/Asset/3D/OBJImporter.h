#pragma once

#include <string>

#include "Type.h"
#include "Engine/Core/CoreMacro.h"
#include "Engine/Engine/Asset/AssetData.h"

namespace GameEngine
{
	class ENGINE_CORE_API OBJImporter
	{
	public:
		static MeshData Import (uint8* data, const int64 dataSize);

	private:
		static std::string ReadToken (const uint8* start);
	};
}
