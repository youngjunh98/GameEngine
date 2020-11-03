#ifndef INCLUDE_FBX_IMPORTER
#define INCLUDE_FBX_IMPORTER

#include <vector>
#include <fbxsdk.h>

#include "Core/CoreMinimal.h"
#include "Rendering/Vertex.h"

namespace GameEngine
{
	class FBXImporter
	{
	public:
		static bool Start ();
		static void Shutdown ();

		static bool Import (Mesh& mesh, int8* data, const int64 dataSize);
		
	private:
		static void TraverseNode (FbxNode* fbxNode, std::vector<std::vector<Vertex>>& subMeshVertices);
		static FbxVector4 GetMeshUV (FbxMesh* fbxMesh, int32 controlPointIndex, int32 uvIndex);
		static FbxVector4 GetMeshNormal (FbxMesh* fbxMesh, int32 controlPointIndex, int32 vertexId);
		static FbxVector4 GetMeshTangent (FbxMesh* fbxMesh, int32 controlPointIndex, int32 vertexId);

		static FbxAMatrix GetWorldMatrix (FbxNode* fbxNode);
	};
}

#endif