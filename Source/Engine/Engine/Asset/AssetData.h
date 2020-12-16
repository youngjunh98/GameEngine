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
}
