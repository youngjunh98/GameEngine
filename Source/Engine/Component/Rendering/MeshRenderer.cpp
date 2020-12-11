#include "MeshRenderer.h"
#include "Engine/GameObject.h"
#include "Engine/Rendering/Mesh.h"
#include "Engine/Rendering/GlobalRenderer.h"
#include "Engine/Component/Transform.h"
#include "Engine/Asset/AssetManager.h"
#include "Editor/EditorGUI.h"

namespace GameEngine
{
	MeshRenderer::MeshRenderer () : Renderer ("Mesh Renderer"),
		m_mesh (nullptr), m_bTessellation (false)
	{
	}

	MeshRenderer::~MeshRenderer ()
	{
	}

	void MeshRenderer::Render (RenderPass* renderPass)
	{
		if (m_mesh == nullptr)
		{
			return;
		}

		Transform& transform = GetGameObject ().GetTransform ();
		Vector3 position = transform.GetPosition ();
		Quaternion rotation = transform.GetRotation ();
		Vector3 scale = transform.GetScale ();

		// Transpose of inverse of A is the inverse of transpose of A.
		// (A-1)T = (AT)-1
		ObjectConstantBuffer objectConstantBuffer;
		objectConstantBuffer.m_localToWorld = Matrix4x4::ScaleRotateTranslate (position, rotation, scale).Transposed ();
		objectConstantBuffer.m_worldToLocal = objectConstantBuffer.m_localToWorld.Inversed ();

		GlobalRenderer::SetGlobalShaderConstantBuffer ("CBObject", &objectConstantBuffer);

		renderPass->SetTessellation (m_bTessellation);

		int32 subMeshCount = static_cast<int32> (m_mesh->GetSubMeshCount ());
		int32 materialCount = static_cast<int32> (GetMaterials ().size ());
		int32 renderMeshCount = Math::Min (subMeshCount, materialCount);

		for (int32 i = 0; i < renderMeshCount; i++)
		{
			renderPass->BindMaterial (GetMaterials ().at (i));
			GlobalRenderer::DrawVertices (m_mesh->GetVertexBufferResource (i), m_mesh->GetIndexBufferResource (i));
		}
	}

	Mesh* MeshRenderer::GetMesh () const
	{
		return m_mesh;
	}

	void MeshRenderer::SetMesh (Mesh* mesh)
	{
		m_mesh = mesh;
	}

	void MeshRenderer::SetTessellation (bool bTessellation)
	{
		m_bTessellation = bTessellation;
	}

	void MeshRenderer::OnRenderEditor ()
	{
		Mesh* mesh = static_cast<Mesh*> (EditorGUI::InputAsset ("Mesh", "Mesh", GetMesh ()));

		if (mesh != nullptr)
		{
			SetMesh (mesh);
		}

		m_bTessellation = EditorGUI::InputCheckbox ("Enable Tessellation", m_bTessellation);

		if (m_mesh != nullptr)
		{
			for (uint32 i = 0; i < m_mesh->GetSubMeshCount (); i++)
			{
				Material* material = GetMaterial (i);
				material = static_cast<Material*> (EditorGUI::InputAsset ("Material " + std::to_string (i), "Material", material));

				if (material != nullptr)
				{
					SetMaterial (material, i);
				}
			}
		}
	}

	void MeshRenderer::OnSerialize (Json::Json& json) const
	{
		Renderer::OnSerialize (json);

		json["type"] = "MeshRenderer";
		json["mesh"] = g_assetManager.GetAssetPath (m_mesh);
		json["tessellation"] = m_bTessellation;
		json["materials"] = Json::Json::array ();

		for (uint32 i = 0; i < m_mesh->GetSubMeshCount (); i++)
		{
			Material* material = GetMaterial (i);
			PathString materialPath = g_assetManager.GetAssetPath (material);
			json["materials"].push_back (materialPath);
		}
	}

	void MeshRenderer::OnDeserialize (const Json::Json& json)
	{
		Renderer::OnDeserialize (json);

		std::wstring meshPath = json["mesh"];
		auto* found = g_assetManager.FindAsset<Mesh> (meshPath);

		if (found != nullptr)
		{
			m_mesh = found;
		}

		json.at ("tessellation").get_to (m_bTessellation);

		if (json.find ("materials") != json.end ())
		{
			uint32 index = 0;
			for (auto materialInfo : json["materials"].items ())
			{
				PathString path = materialInfo.value ();
				Material* material = g_assetManager.FindAsset<Material> (path);

				if (material != nullptr)
				{
					SetMaterial (material, index);
					++index;
				}
			}
		}
	}
}