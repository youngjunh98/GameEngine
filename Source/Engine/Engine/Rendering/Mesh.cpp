#include "Mesh.h"
#include "Engine/Core/JSON/JsonSerializer.h"
#include "Engine/RI/RenderingInterface.h"

namespace GameEngine
{
	REGISTER_OBJECT_CPP (Mesh)

	void Mesh::Destroy ()
	{
		Object::Destroy ();

		for (SubMesh& subMesh : m_subMeshList)
		{
			subMesh.m_vertices.clear ();
			subMesh.m_indices.clear ();

			subMesh.m_vertexBufferResource = nullptr;
			subMesh.m_indexBufferResource = nullptr;
		}

		m_subMeshList.clear ();
	}

	uint32 Mesh::GetSubMeshCount () const
	{
		return m_subMeshList.size ();
	}

	void Mesh::SetSubMeshCount (uint32 count)
	{
		m_subMeshList.resize (count);
	}

	std::vector<Vertex> Mesh::GetVertices (uint32 subMeshIndex) const
	{
		std::vector<Vertex> vertices;

		if (subMeshIndex < GetSubMeshCount ())
		{
			vertices = m_subMeshList.at (subMeshIndex).m_vertices;
		}

		return vertices;
	}

	void Mesh::SetVertices (uint32 subMeshIndex, const std::vector<Vertex>& vertices)
	{
		if (subMeshIndex >= GetSubMeshCount ())
		{
			return;
		}

		m_subMeshList.at (subMeshIndex).m_vertices = vertices;
	}

	std::vector<uint32> Mesh::GetIndices (uint32 subMeshIndex) const
	{
		std::vector<uint32> indices;

		if (subMeshIndex < GetSubMeshCount ())
		{
			indices = m_subMeshList.at (subMeshIndex).m_indices;
		}

		return indices;
	}

	void Mesh::SetIndices (uint32 subMeshIndex, const std::vector<uint32>& indices)
	{
		if (subMeshIndex >= GetSubMeshCount ())
		{
			return;
		}

		m_subMeshList.at (subMeshIndex).m_indices = indices;
	}

	RI_VertexBuffer* Mesh::GetVertexBufferResource (uint32 subMeshIndex) const
	{
		RI_VertexBuffer* resource = nullptr;

		if (subMeshIndex < GetSubMeshCount ())
		{
			resource = m_subMeshList.at (subMeshIndex).m_vertexBufferResource.get ();
		}

		return resource;
	}

	bool Mesh::UpdateVertexBufferResource (uint32 subMeshIndex)
	{
		if (subMeshIndex >= GetSubMeshCount ())
		{
			return false;
		}

		const auto& vertices = m_subMeshList.at (subMeshIndex).m_vertices;
		auto resource = RenderingInterface::GetModule ().CreateVertexBuffer (vertices.data (), vertices.size (), sizeof (Vertex));

		if (resource == nullptr)
		{
			return false;
		}

		m_subMeshList.at (subMeshIndex).m_vertexBufferResource = resource;

		return true;
	}

	RI_IndexBuffer* Mesh::GetIndexBufferResource (uint32 subMeshIndex) const
	{
		RI_IndexBuffer* resource = nullptr;

		if (subMeshIndex < GetSubMeshCount ())
		{
			resource = m_subMeshList.at (subMeshIndex).m_indexBufferResource.get ();
		}

		return resource;
	}

	bool Mesh::UpdateIndexBufferResource (uint32 subMeshIndex)
	{
		if (subMeshIndex >= GetSubMeshCount ())
		{
			return false;
		}

		const auto& indices = m_subMeshList.at (subMeshIndex).m_indices;
		auto resource = RenderingInterface::GetModule ().CreateIndexBuffer (indices.data (), indices.size (), sizeof (uint32));

		if (resource == nullptr)
		{
			return false;
		}

		m_subMeshList.at (subMeshIndex).m_indexBufferResource = resource;

		return true;
	}

	void Mesh::OnSerialize (Json::Json& json) const
	{
		Object::OnSerialize (json);

		json["subMeshes"] = Json::Json::array ();

		for (const SubMesh& subMesh : m_subMeshList)
		{
			Json::Json subMeshJson;
			subMeshJson["vertices"] = Json::Json::array ();
			subMeshJson["indices"] = Json::Json::array ();

			for (const Vertex& vertex : subMesh.m_vertices)
			{
				Json::Json vertexJson;
				Json::JsonSerializer::Serialize<Vector3> (vertexJson, "position", vertex.m_position);
				Json::JsonSerializer::Serialize<Vector2> (vertexJson, "uv", vertex.m_uv);
				Json::JsonSerializer::Serialize<Vector3> (vertexJson, "normal", vertex.m_normal);
				Json::JsonSerializer::Serialize<Vector3> (vertexJson, "tangent", vertex.m_tangent);
				subMeshJson["vertices"].push_back (vertexJson);
			}

			for (const uint32 index : subMesh.m_indices)
			{
				subMeshJson["indices"].push_back (index);
			}

			json["subMeshes"].push_back (subMeshJson);
		}
	}

	void Mesh::OnDeserialize (const Json::Json& json)
	{
		if (json.contains ("subMeshes") == false)
		{
			return;
		}

		m_subMeshList.clear ();

		uint32 subMeshCount = static_cast<uint32> (json["subMeshes"].size ());
		SetSubMeshCount (subMeshCount);

		uint32 subMeshIndex = 0;
		std::vector<Vertex> vertices;
		std::vector<uint32> indices;

		for (const Json::Json& subMeshJson : json["subMeshes"])
		{
			if (subMeshJson.contains ("vertices"))
			{
				for (const Json::Json& vertexJson : subMeshJson["vertices"])
				{
					Vertex vertex;
					vertex.m_position = Json::JsonSerializer::Deserialize<Vector3> (vertexJson, "position");
					vertex.m_uv = Json::JsonSerializer::Deserialize<Vector2> (vertexJson, "uv");
					vertex.m_normal = Json::JsonSerializer::Deserialize<Vector3> (vertexJson, "normal");
					vertex.m_tangent = Json::JsonSerializer::Deserialize<Vector3> (vertexJson, "tangent");
					vertices.push_back (vertex);
				}
			}

			if (subMeshJson.contains ("indices"))
			{
				for (const Json::Json& indexJson : subMeshJson["indices"])
				{
					uint32 index  = indexJson.get<uint32> ();
					indices.push_back (index);
				}
			}

			SetVertices (subMeshIndex, vertices);
			SetIndices (subMeshIndex, indices);

			UpdateVertexBufferResource (subMeshIndex);
			UpdateIndexBufferResource (subMeshIndex);

			vertices.clear ();
			indices.clear ();

			++subMeshIndex;
		}
	}
}
