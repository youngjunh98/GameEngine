#include "Mesh.h"
#include "Engine/RI/RenderingInterface.h"

namespace GameEngine
{
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
}
