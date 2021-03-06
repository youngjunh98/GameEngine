#pragma once

#include <vector>

#include "Type.h"
#include "Vertex.h"
#include "Engine/Core/Object/Object.h"
#include "Engine/RI/RenderingInterfaceResource.h"
#include "Engine/Engine/EngineMacro.h"

namespace GameEngine
{
	class ENGINE_API Mesh : public Object
	{
	public:
		REGISTER_OBJECT_HEADER (Mesh)

		Mesh () : m_subMeshList () {}
		virtual ~Mesh () {}

		virtual void Destroy () override;

		uint32 GetSubMeshCount () const;
		void SetSubMeshCount (uint32 count);

		std::vector<Vertex> GetVertices (uint32 subMeshIndex) const;
		void SetVertices (uint32 subMeshIndex, const std::vector<Vertex>& vertices);

		std::vector<uint32> GetIndices (uint32 subMeshIndex) const;
		void SetIndices (uint32 subMeshIndex, const std::vector<uint32>& indices);

		RI_VertexBuffer* GetVertexBufferResource (uint32 subMeshIndex) const;
		bool UpdateVertexBufferResource (uint32 subMeshIndex);

		RI_IndexBuffer* GetIndexBufferResource (uint32 subMeshIndex) const;
		bool UpdateIndexBufferResource (uint32 subMeshIndex);

		virtual void OnSerialize (Json::Json& json) const override;
		virtual void OnDeserialize (const Json::Json& json) override;

	private:
		struct SubMesh
		{
			std::vector<Vertex> m_vertices;
			std::vector<uint32> m_indices;

			RenderingResourcePtr<RI_VertexBuffer> m_vertexBufferResource;
			RenderingResourcePtr<RI_IndexBuffer> m_indexBufferResource;
		};

		std::vector<SubMesh> m_subMeshList;
	};
}
