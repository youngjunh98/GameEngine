#pragma once

#include <vector>

#include "Type.h"
#include "Engine/Engine/Rendering/Vertex.h"

namespace GameEngine
{
	struct MeshData
	{
		struct SubMeshData
		{
			std::vector<Vertex> m_vertices;
			std::vector<uint32> m_indices;
		};

		std::vector<SubMeshData> m_subMeshData;
	};

	struct AudioData
	{
		int32 m_channelCount;
		float m_sampleRate;
		std::vector<float> m_sampleData;
	};

	struct TextureData
	{
		// Pixel data
		std::vector<uint8> m_data;
		// size of pixel data in bytes
		uint32 m_dataBytes;
		// Size of width in bytes (used for 2D and 3D texture)
		uint32 m_dataRowBytes;
		// Size of width * height in bytes (used for 3D texture)
		uint32 m_dataSquareBytes;
	};
}
