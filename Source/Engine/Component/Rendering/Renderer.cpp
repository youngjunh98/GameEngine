#include "Renderer.h"

namespace GameEngine
{
	Renderer::Renderer (const std::string& name) : Component (name)
	{
	}

	Renderer::~Renderer ()
	{
	}

	Material* Renderer::GetMaterial (uint32 index) const
	{
		return index < m_materials.size () ? m_materials.at (index) : nullptr;
	}

	void Renderer::SetMaterial (Material* material, uint32 index)
	{
		if (index < m_materials.size ())
		{
			m_materials.at (index) = material;
		}
		else
		{
			for (uint32 i = m_materials.size (); i <= index; i++)
			{
				m_materials.push_back (nullptr);
			}

			m_materials.at (index) = material;
		}
	}

	std::vector<Material*>& Renderer::GetMaterials ()
	{
		return m_materials;
	}

	void Renderer::SetMaterials (const std::vector<Material*>& materials)
	{
		m_materials = materials;
	}
}