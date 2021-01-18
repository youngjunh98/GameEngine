#include "MeshRenderer.h"
#include "Engine/Core/Asset/AssetManager.h"
#include "Engine/Core/JSON/JsonSerializer.h"
#include "Editor/Core/EditorGUI.h"
#include "Engine/Engine/GameObject.h"
#include "Engine/Engine/Rendering/Mesh.h"
#include "Engine/Engine/Rendering/GlobalRenderer.h"
#include "Engine/Engine/Component/Transform.h"

namespace GameEngine
{
	REGISTER_OBJECT_CPP (MeshRenderer)

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

		Json::JsonSerializer::Serialize (json, "mesh", AssetManager::GetAssetPath (*m_mesh));
		Json::JsonSerializer::Serialize (json, "tessellation", m_bTessellation);
		Json::JsonSerializer::CreateArray (json, "materials");

		for (uint32 i = 0; i < m_mesh->GetSubMeshCount (); i++)
		{
			Material* material = GetMaterial (i);
			PathString materialPath = AssetManager::GetAssetPath (*material);
			Json::Json materialJson = materialPath;
			Json::JsonSerializer::AppendArray (json, "materials", materialJson);
		}
	}

	void MeshRenderer::OnDeserialize (const Json::Json& json)
	{
		Renderer::OnDeserialize (json);

		PathString meshPath = Json::JsonSerializer::Deserialize<PathString> (json, "mesh");
		m_mesh = dynamic_cast<Mesh*> (AssetManager::GetAsset (meshPath).get ());
		m_bTessellation = Json::JsonSerializer::Deserialize<bool> (json, "tessellation");

		uint32 index = 0;

		for (auto it = Json::JsonSerializer::GetArrayBegin (json, "materials"); it != Json::JsonSerializer::GetArrayEnd (json, "materials"); it++)
		{
			Json::Json materialJson = it.value ();
			PathString materialPath = materialJson.get<PathString> ();
			Material* material = dynamic_cast<Material*> (AssetManager::GetAsset (materialPath).get ());
			SetMaterial (material, index);
		}
	}
}