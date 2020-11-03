#ifndef INCLUDE_RENDERER
#define INCLUDE_RENDERER

#include <vector>

#include "Core/CoreMinimal.h"
#include "Component/Component.h"

namespace GameEngine
{
	class Renderer : public Component
	{
	public:
		Renderer () {}
		virtual ~Renderer () {}

		virtual void Render (RenderPass* renderPass) = 0;

		Material* GetMaterial (uint32 index);
		void SetMaterial (Material* material, uint32 index);

		std::vector<Material*>& GetMaterials ();
		void SetMaterials (const std::vector<Material*>& materials);

	private:
		std::vector<Material*> m_materials;
	};
}

#endif