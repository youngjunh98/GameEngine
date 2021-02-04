#pragma once

#include <fbxsdk.h>

#include "Type.h"
#include "Engine/Engine/Asset/AssetData.h"

namespace GameEngine
{
	class FBXImporter
	{
	public:
		static bool Start ();
		static void Shutdown ();

		static MeshData Import (uint8* data, const int64 dataSize);
		
	private:
		static void TraverseNode (FbxNode* fbxNode, MeshData& meshData);
		static FbxVector4 GetMeshUV (FbxMesh* fbxMesh, int32 controlPointIndex, int32 uvIndex);
		static FbxVector4 GetMeshNormal (FbxMesh* fbxMesh, int32 controlPointIndex, int32 vertexId);
		static FbxVector4 GetMeshTangent (FbxMesh* fbxMesh, int32 controlPointIndex, int32 vertexId);
		static FbxAMatrix GetWorldMatrix (FbxNode* fbxNode);
	};
}
