#include <vector>
#include <memory>

#include "FBXImporter.h"
#include "FBXMemoryReadStream.h"
#include "Engine/Rendering/Mesh.h"

namespace GameEngine
{
	static FbxManager* g_fbxManager;
	static FbxIOSettings* g_fbxIOSettings;

	bool FBXImporter::Start ()
	{
		g_fbxManager = FbxManager::Create ();

		g_fbxIOSettings = FbxIOSettings::Create (g_fbxManager, IOSROOT);
		g_fbxManager->SetIOSettings (g_fbxIOSettings);

		return true;
	}
	void FBXImporter::Shutdown ()
	{
		g_fbxIOSettings->Destroy ();
		g_fbxIOSettings = nullptr;

		g_fbxManager->Destroy ();
		g_fbxManager = nullptr;
	}

	bool FBXImporter::Import (Mesh& mesh, int8* data, const int64 dataSize)
	{
		FBXMemoryReadStream stream (dataSize, g_fbxManager);
		FbxImporter* fbxImporter = FbxImporter::Create (g_fbxManager, "Importer");

		if (fbxImporter->Initialize (&stream, data, -1, g_fbxManager->GetIOSettings ()) == false)
		{
			const auto* error = fbxImporter->GetStatus ().GetErrorString ();
			fbxImporter->Destroy ();

			return false;
		}

		FbxScene* fbxScene = FbxScene::Create (g_fbxManager, "Scene");

		if (fbxImporter->Import (fbxScene) == false)
		{
			fbxScene->Destroy ();
			fbxImporter->Destroy ();

			return false;
		}

		fbxImporter->Destroy ();

		std::vector<std::vector<Vertex>> subMeshVertices;
		std::vector<std::vector<uint32>> subMeshIndices;

		FbxNode* fbxRootNode = fbxScene->GetRootNode ();

		if (fbxRootNode)
		{
			FbxGeometryConverter fbxGeometryConverter (g_fbxManager);

			if (fbxGeometryConverter.Triangulate (fbxScene, true) == false)
			{
				return false;
			}

			int32 materialCount = fbxScene->GetMaterialCount ();

			mesh.SetSubMeshCount (materialCount);
			subMeshVertices.resize (materialCount);
			subMeshIndices.resize (materialCount);

			TraverseNode (fbxRootNode, subMeshVertices);
		}

		fbxScene->Destroy ();

		for (uint32 subMeshIndex = 0; subMeshIndex < subMeshVertices.size (); subMeshIndex++)
		{
			mesh.SetVertices (subMeshIndex, subMeshVertices.at (subMeshIndex));

			if (mesh.UpdateVertexBufferResource (subMeshIndex) == false)
			{
				return false;
			}
		}

		return true;
	}

	void FBXImporter::TraverseNode (FbxNode* fbxNode, std::vector<std::vector<Vertex>>& subMeshVertices)
	{
		FbxNodeAttribute* fbxNodeAttribute = fbxNode->GetNodeAttribute ();

		if (fbxNodeAttribute != nullptr &&
			fbxNodeAttribute->GetAttributeType () == FbxNodeAttribute::eMesh)
		{
			FbxMesh* fbxMesh = fbxNode->GetMesh ();

			fbxMesh->ApplyPivot ();
			fbxMesh->GenerateTangentsData (0);

			int32 materialCount = fbxNode->GetMaterialCount ();
			int32 polygonCount = fbxMesh->GetPolygonCount ();
			int32 vertexId = 0;

			FbxAMatrix matrix = GetWorldMatrix (fbxNode);

			for (int32 polygonIndex = 0; polygonIndex < polygonCount; polygonIndex++)
			{
				Vector3 positions[3] = { };
				Vector2 uvs[3] = { };
				Vector3 normals[3] = { };
				Vector3 tangents[3] = { };

				for (int32 vertexIndex = 0; vertexIndex < 3; vertexIndex++)
				{
					int32 controlPointIndex = fbxMesh->GetPolygonVertex (polygonIndex, vertexIndex);

					FbxVector4 controlPoint = fbxMesh->GetControlPointAt (controlPointIndex);
					FbxVector4 uv = GetMeshUV (fbxMesh, controlPointIndex, fbxMesh->GetTextureUVIndex (polygonIndex, vertexIndex));
					FbxVector4 normal = GetMeshNormal (fbxMesh, controlPointIndex, vertexId);
					FbxVector4 tangent = GetMeshTangent (fbxMesh, controlPointIndex, vertexId);
					float uvWinding = static_cast<float> (tangent.mData[3]);

					controlPoint = matrix.MultT (controlPoint);

					positions[vertexIndex] = Vector3 (static_cast<float> (controlPoint.mData[0]), static_cast<float> (controlPoint.mData[1]), static_cast<float> (controlPoint.mData[2]));
					uvs[vertexIndex] = Vector2 (static_cast<float> (uv.mData[0]), 1.0f - static_cast<float> (uv.mData[1]));
					normals[vertexIndex] = Vector3 (static_cast<float> (normal.mData[0]), static_cast<float> (normal.mData[1]), static_cast<float> (normal.mData[2]));
					tangents[vertexIndex] = Vector3 (uvWinding * static_cast<float> (tangent.mData[0]), static_cast<float> (tangent.mData[1]), static_cast<float> (tangent.mData[2]));

					vertexId += 1;
				}

				FbxLayerElementMaterial* fbxMaterial = fbxMesh->GetLayer (0)->GetMaterials ();
				int32 materialId = fbxMaterial->GetIndexArray ().GetAt (polygonIndex);

				subMeshVertices.at (materialId).push_back ({ positions[0], uvs[0], normals[0], tangents[0] });
				subMeshVertices.at (materialId).push_back ({ positions[1], uvs[1], normals[1], tangents[1] });
				subMeshVertices.at (materialId).push_back ({ positions[2], uvs[2], normals[2], tangents[2] });
			}
		}

		for (int32 childIndex = 0; childIndex < fbxNode->GetChildCount (); childIndex++)
		{
			TraverseNode (fbxNode->GetChild (childIndex), subMeshVertices);
		}
	}

	FbxVector4 FBXImporter::GetMeshUV (FbxMesh* fbxMesh, int32 controlPointIndex, int32 uvIndex)
	{
		FbxVector4 uv;

		const FbxGeometryElementUV* fbxUV = fbxMesh->GetElementUV (0);
		const FbxLayerElement::EMappingMode uvMapping = fbxUV->GetMappingMode ();
		const FbxLayerElement::EReferenceMode uvReference = fbxUV->GetReferenceMode ();

		if (uvMapping == FbxGeometryElement::eByControlPoint)
		{
			if (uvReference == FbxGeometryElement::eDirect)
			{
				uv = fbxUV->GetDirectArray ().GetAt (controlPointIndex);
			}
			else if (uvReference == FbxGeometryElement::eIndexToDirect)
			{
				int32 index = fbxUV->GetIndexArray ().GetAt (controlPointIndex);
				uv = fbxUV->GetDirectArray ().GetAt (index);
			}
		}
		else if (uvMapping == FbxGeometryElement::eByPolygonVertex)
		{
			uv = fbxUV->GetDirectArray ().GetAt (uvIndex);
		}

		return uv;
	}

	FbxVector4 FBXImporter::GetMeshNormal (FbxMesh * fbxMesh, int32 controlPointIndex, int32 vertexId)
	{
		FbxVector4 normal;

		const FbxGeometryElementNormal* fbxNormal = fbxMesh->GetElementNormal (0);
		const FbxLayerElement::EMappingMode normalMapping = fbxNormal->GetMappingMode ();
		const FbxLayerElement::EReferenceMode normalReference = fbxNormal->GetReferenceMode ();

		if (normalMapping == FbxGeometryElement::eByControlPoint)
		{
			if (normalReference == FbxGeometryElement::eDirect)
			{
				normal = fbxNormal->GetDirectArray ().GetAt (controlPointIndex);
			}
			else if (normalReference == FbxGeometryElement::eIndexToDirect)
			{
				int32 index = fbxNormal->GetIndexArray ().GetAt (controlPointIndex);
				normal = fbxNormal->GetDirectArray ().GetAt (index);
			}
		}
		else if (normalMapping == FbxGeometryElement::eByPolygonVertex)
		{
			if (normalReference == FbxGeometryElement::eDirect)
			{
				normal = fbxNormal->GetDirectArray ().GetAt (vertexId);
			}
			else if (normalReference == FbxGeometryElement::eIndexToDirect)
			{
				int32 index = fbxNormal->GetIndexArray ().GetAt (vertexId);
				normal = fbxNormal->GetDirectArray ().GetAt (index);
			}
		}

		return normal;
	}

	FbxVector4 FBXImporter::GetMeshTangent (FbxMesh * fbxMesh, int32 controlPointIndex, int32 vertexId)
	{
		FbxVector4 tangent;

		const FbxGeometryElementTangent* fbxTangent = fbxMesh->GetElementTangent (0);
		const FbxLayerElement::EMappingMode tangentMapping = fbxTangent->GetMappingMode ();
		const FbxLayerElement::EReferenceMode tangentReference = fbxTangent->GetReferenceMode ();

		if (tangentMapping == FbxGeometryElement::eByControlPoint)
		{
			if (tangentReference == FbxGeometryElement::eDirect)
			{
				tangent = fbxTangent->GetDirectArray ().GetAt (controlPointIndex);
			}
			else if (tangentReference == FbxGeometryElement::eIndexToDirect)
			{
				int32 index = fbxTangent->GetIndexArray ().GetAt (controlPointIndex);
				tangent = fbxTangent->GetDirectArray ().GetAt (index);
			}
		}
		else if (tangentMapping == FbxGeometryElement::eByPolygonVertex)
		{
			if (tangentReference == FbxGeometryElement::eDirect)
			{
				tangent = fbxTangent->GetDirectArray ().GetAt (vertexId);
			}
			else if (tangentReference == FbxGeometryElement::eIndexToDirect)
			{
				int32 index = fbxTangent->GetIndexArray ().GetAt (vertexId);
				tangent = fbxTangent->GetDirectArray ().GetAt (index);
			}
		}

		return tangent;
	}

	FbxAMatrix FBXImporter::GetWorldMatrix (FbxNode* fbxNode)
	{
		FbxAMatrix worldMatrix;
		worldMatrix.SetIdentity ();

		FbxNode* parentNode = fbxNode;

		while (parentNode != nullptr)
		{
			worldMatrix = parentNode->EvaluateLocalTransform () * worldMatrix;
			parentNode = parentNode->GetParent ();
		}

		FbxAMatrix geometryMatrix;
		geometryMatrix.SetT (fbxNode->GetGeometricTranslation (FbxNode::eSourcePivot));
		geometryMatrix.SetR (fbxNode->GetGeometricRotation (FbxNode::eSourcePivot));
		geometryMatrix.SetS (fbxNode->GetGeometricScaling (FbxNode::eSourcePivot));

		return worldMatrix * geometryMatrix;
	}
}