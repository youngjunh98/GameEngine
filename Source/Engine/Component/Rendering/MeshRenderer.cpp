#include "MeshRenderer.h"
#include "Engine/GameObject.h"
#include "Engine/Rendering/Mesh.h"
#include "Engine/Rendering/GlobalRenderer.h"
#include "Engine/Component/Transform.h"
#include "Engine/Asset/AssetManager.h"

namespace GameEngine
{
	MeshRenderer::MeshRenderer () :
		m_mesh (nullptr),
		m_bTessellation (false)
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

		g_renderer.SetGlobalShaderConstantBuffer ("CBObject", &objectConstantBuffer);

		renderPass->SetTessellation (m_bTessellation);

		int32 subMeshCount = static_cast<int32> (m_mesh->GetSubMeshCount ());
		int32 materialCount = static_cast<int32> (GetMaterials ().size ());
		int32 renderMeshCount = Math::Min (subMeshCount, materialCount);

		for (int32 i = 0; i < renderMeshCount; i++)
		{
			renderPass->BindMaterial (GetMaterials ().at (i));
			g_renderer.DrawVertices (m_mesh->GetVertexBufferResource (i), m_mesh->GetIndexBufferResource (i));
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

	void MeshRenderer::OnRenderEditor (Editor& editor)
	{
		editor.BeginComponent ("Mesh Renderer", this, true);

		std::wstring assetPath = g_assetManager.GetAssetPath (m_mesh);
		editor.AddPropertyAsset ("Mesh", "Mesh", assetPath);

		m_mesh = g_assetManager.FindAsset<Mesh> (assetPath);

		editor.AddCheckbox ("Enable Tessellation", m_bTessellation);

		if (m_mesh != nullptr)
		{
			for (uint32 i = 0; i < m_mesh->GetSubMeshCount (); i++)
			{
				assetPath = g_assetManager.GetAssetPath (GetMaterial (i));
				editor.AddPropertyAsset ("Material " + std::to_string (i), "Material", assetPath);

				auto* foundMaterial = g_assetManager.FindAsset<Material> (assetPath);
				SetMaterial (foundMaterial, i);
			}
		}
	}

	void MeshRenderer::OnSerialize (Json::Json& json) const
	{
		Renderer::OnSerialize (json);

		json["type"] = "MeshRenderer";
		json["mesh"] = g_assetManager.GetAssetPath (m_mesh);
		json["tessellation"] = m_bTessellation;
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
	}
}