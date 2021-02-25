#pragma once

#include <vector>

#include "Type.h"
#include "AssetMacro.h"
#include "Engine/Core/Math/Math.h"
#include "Engine/RI/RenderingInterfaceEnum.h"
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

	struct ENGINE_ASSET_API TextureData
	{
		struct ENGINE_ASSET_API MipMapData
		{
			// Pixel data
			std::vector<uint8> m_pixels;

			// Width of texture
			uint32 m_width;

			// Height of texture
			uint32 m_height;

			MipMapData ();
			~MipMapData ();
		};

		struct ENGINE_ASSET_API ArrayData
		{
			// Texture file data
			std::vector<uint8> m_fileData;

			// Mip maps
			std::vector<MipMapData> m_mipMaps;
		};

		// Array of textures
		// Texture array must have the same mip map level, data format and texture size.
		std::vector<ArrayData> m_array;

		// Number of channel
		// 1 -> Gray
		// 2 -> Gray + Alpha
		// 3 -> RGB
		// 4 -> RGB + Alpha
		int32 m_channels;

		// Bytes of pixel component
		// 1 -> 8-bit
		// 2 -> 16-bit
		// 4 -> 32-bit
		int32 m_bytes;

		// Is linear color space?
		bool m_bLinear;

		// Rendering interface resource format
		ERenderingResourceFormat m_format;

		TextureData ();
		~TextureData ();

		// Size of pixel data in bytes
		uint32 GetDataSizeInBytes (uint32 arrayIndex, uint32 mipMapIndex) const;

		// Size of width in bytes (used for 2D and 3D texture)
		uint32 GetRowSizeInBytes (uint32 arrayIndex, uint32 mipMapIndex) const;

		// Return size of first texture
		bool IsSquare () const;

		// Return if first texture is power of two
		bool IsPowerOfTwo () const;

		// Append texture array
		bool AddTextureArray (const TextureData& texture);
	};
}
