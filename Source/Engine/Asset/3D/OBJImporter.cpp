#include <vector>
#include <memory>

#include "ObjImporter.h"

namespace GameEngine
{
	MeshData OBJImporter::Import (uint8* data, const int64 dataSize)
	{
		MeshData meshData;
		meshData.m_subMeshData.resize (1);

		std::vector<Vector3> positions;
		std::vector<Vector3> normals;
		std::vector<Vector2> uvs;
		std::vector<Vector3> tangents;
		//std::vector<Vector3> binormals;

		int64 filePosition = 0;

		while (filePosition < dataSize)
		{
			char character = data[filePosition];

			if (character == '#')
			{
				while (data[filePosition] != '\n')
				{
					filePosition += 1;
				}

				filePosition += 1;
			}
			else
			{
				std::string token = ReadToken (&data[filePosition]);
				filePosition += token.size () + 1;

				if (token == "v")
				{
					float position[3];

					for (uint32 i = 0; i < 3; i++)
					{
						token = ReadToken (&data[filePosition]);
						filePosition += token.size () + 1;

						position[i] = std::stof (token);
					}

					positions.push_back (Vector3 (position[0], position[1], position[2]));
				}
				else if (token == "vn")
				{
					float normal[3];

					for (uint32 i = 0; i < 3; i++)
					{
						token = ReadToken (&data[filePosition]);
						filePosition += token.size () + 1;

						normal[i] = std::stof (token);
					}

					normals.push_back (Vector3 (normal[0], normal[1], normal[2]));
				}
				else if (token == "vt")
				{
					float uv[2];

					for (uint32 i = 0; i < 2; i++)
					{
						token = ReadToken (&data[filePosition]);
						filePosition += token.size () + 1;

						uv[i] = std::stof (token);
					}

					uvs.push_back (Vector2 (uv[0], uv[1]));
				}
				else if (token == "f")
				{
					Vector3 facePositions[3];
					Vector2 faceUVs[3];
					Vector3 faceNormals[3];

					for (uint32 i = 0; i < 3; i++)
					{
						token = ReadToken (&data[filePosition]);
						filePosition += token.size () + 1;

						size_t uvStart = token.find ('/', 0) + 1;
						size_t normalStart = token.find ('/', uvStart) + 1;

						size_t positionIndex = std::stoull (token.substr (0, uvStart - 1)) - 1;
						size_t uvIndex = std::stoull (token.substr (uvStart, normalStart - uvStart - 1)) - 1;
						size_t normalIndex = std::stoull (token.substr (normalStart, token.length () - normalStart)) - 1;

						bool hasNormal = normals.size () > 0;
						bool hasUV = uvs.size () > 0;

						facePositions[i] = positions.at  (positionIndex);
						faceUVs[i] = hasUV ? uvs.at (uvIndex) : Vector2::Zero;
						faceNormals[i] = hasNormal ? normals.at (normalIndex) : Vector3::Zero;
					}

					// Compute tangent and binormal
					Vector3 edge1 = facePositions[1] - facePositions[0];
					Vector3 edge2 = facePositions[2] - facePositions[0];

					Vector2 uv1uv2 = faceUVs[1] - faceUVs[0];
					Vector2 uv1uv3 = faceUVs[2] - faceUVs[0];

					float denom = (1.0f / (uv1uv2.m_x * uv1uv3.m_y - uv1uv2.m_y * uv1uv3.m_x));

					Vector3 tangent = denom * Vector3 (
						uv1uv3.m_y * edge1.m_x - uv1uv2.m_y * edge2.m_x,
						uv1uv3.m_y * edge1.m_y - uv1uv2.m_y * edge2.m_y,
						uv1uv3.m_y * edge1.m_z - uv1uv2.m_y * edge2.m_z
					).Normalized ();

					//Vector3 binormal = denom * Vector3 (
					//	uv1uv3.m_x * edge1.m_x - uv1uv2.m_x * edge2.m_x,
					//	uv1uv3.m_x * edge1.m_y - uv1uv2.m_x * edge2.m_y,
					//	uv1uv3.m_x * edge1.m_z - uv1uv2.m_x * edge2.m_z
					//);

					tangents.push_back (tangent);
					//binormals.push_back (binormal);

					Vertex v1 = { facePositions[0], faceUVs[0], faceNormals[0], tangent };
					Vertex v2 = { facePositions[1], faceUVs[1], faceNormals[1], tangent };
					Vertex v3 = { facePositions[2], faceUVs[2], faceNormals[2], tangent };

					meshData.m_subMeshData.at (0).m_vertices.push_back (v1);
					meshData.m_subMeshData.at (0).m_vertices.push_back (v2);
					meshData.m_subMeshData.at (0).m_vertices.push_back (v3);
				}
				else
				{
					while (data[filePosition] != '\n')
					{
						filePosition += 1;
					}

					filePosition += 1;
				}
			}
		}

		return meshData;
	}

	std::string OBJImporter::ReadToken (const uint8* start)
	{
		std::string result;
		const char* str = reinterpret_cast<const char*> (start);

		for (int32 i = 0; str[i] != ' ' && str[i] && '\t' && str[i] != '\n'; i++)
		{
			result += str[i];
		}

		return result;
	}
}
