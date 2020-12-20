#pragma once

#include "Engine/Core/CoreMinimal.h"
#include "Engine/Engine/EngineMacro.h"
#include "Engine/Engine/Component/Rendering/Renderer.h"

namespace GameEngine
{
	class ENGINE_API MeshRenderer : public Renderer
	{
	public:
		REGISTER_OBJECT_HEADER (MeshRenderer)

		MeshRenderer ();
		virtual ~MeshRenderer ();

		virtual void Render (RenderPass* renderPass) override;

		Mesh* GetMesh () const;
		void SetMesh (Mesh* mesh);

		void SetTessellation (bool bTessellation);

		virtual void OnRenderEditor () override;
		virtual void OnSerialize (Json::Json& json) const override;
		virtual void OnDeserialize (const Json::Json& json) override;

	private:
		Mesh* m_mesh;
		bool m_bTessellation;
	};
}
