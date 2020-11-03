#pragma once

#include "Core/CoreMinimal.h"
#include "Component/Rendering/Renderer.h"

namespace GameEngine
{
	class MeshRenderer : public Renderer
	{
	public:
		MeshRenderer ();
		virtual ~MeshRenderer ();

		virtual void Render (RenderPass* renderPass) override;

		Mesh* GetMesh () const;
		void SetMesh (Mesh* mesh);

		void SetTessellation (bool bTessellation);

		virtual void OnRenderEditor (Editor& editor) override;
		virtual void OnSerialize (Json::Json& json) const override;
		virtual void OnDeserialize (const Json::Json& json) override;

	private:
		Mesh* m_mesh;
		bool m_bTessellation;
	};
}
