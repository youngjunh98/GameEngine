#pragma once

#include "Engine/Engine/EngineMacro.h"
#include "Engine/Engine/Component/Physics/Collider.h"

namespace GameEngine
{
	class ENGINE_API BoxCollider : public Collider
	{
	public:
		BoxCollider ();
		virtual ~BoxCollider ();

		virtual void OnInit () override;

		void SetSize (Vector3 size);
		Vector3 GetSize () const;

		virtual void OnRenderEditor () override;

	private:
		Vector3 m_size;
	};
}
