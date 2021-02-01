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

		// Width of texture
		int32 m_width;

		// Height of texture
		int32 m_height;

		// Number of Channel
		// 1 -> Gray
		// 2 -> Gray + Alpha
		// 3 -> RGB
		// 4 -> RGB + Alpha
		int32 m_channels;

		// Bytes of Pixel Component
		// 1 -> 8-bit
		// 2 -> 16-bit
		// 4 -> 32-bit
		int32 m_bytes;

		// Is Linear Color Space?
		bool m_bLinear;

		TextureData () : m_width (0), m_height (0), 
			m_channels (0), m_bytes (0), m_bLinear (false)
		{
		}

		~TextureData ()
		{
		}

		// Size of pixel data in bytes
		uint32 GetDataSizeInBytes () const
		{
			return m_bytes * m_width * m_height * m_channels;
		}

		// Size of width in bytes (used for 2D and 3D texture)
		uint32 GetRowSizeInBytes () const
		{
			return m_bytes * m_width * m_channels;
		}

		bool IsSquare () const
		{
			return (m_width == m_height) && (m_width > 0) && (m_height > 0);
		}

		bool IsPowerOfTwo () const
		{
			return (m_width % 2 == 0) && (m_height % 2 == 0);
		}
	};
}
