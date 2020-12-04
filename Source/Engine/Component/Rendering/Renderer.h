#pragma once

#include <vector>

#include "Core/CoreMinimal.h"
#include "Engine/Component/Component.h"

namespace GameEngine
{
	class Renderer : public Component
	{
	public:
		Renderer (const std::string& name);
		virtual ~Renderer () = 0;

		virtual void Render (RenderPass* renderPass) = 0;

		Material* GetMaterial (uint32 index) const;
		void SetMaterial (Material* material, uint32 index);

		std::vector<Material*>& GetMaterials ();
		void SetMaterials (const std::vector<Material*>& materials);

	private:
		std::vector<Material*> m_materials;
	};
}
