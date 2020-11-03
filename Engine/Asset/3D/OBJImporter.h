#ifndef INCLUDE_OBJ_IMPORTER
#define INCLUDE_OBJ_IMPORTER

#include <string>

#include "Core/CoreMinimal.h"

namespace GameEngine
{
	class OBJImporter
	{
	public:
		static bool Import (Mesh& mesh, int8* data, const int64 dataSize);

	private:
		static std::string ReadToken (const int8* start);
	};
}

#endif