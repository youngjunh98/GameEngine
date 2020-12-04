#pragma once

#include "Engine/Component/Physics/Collider.h"

namespace GameEngine
{
	class BoxCollider : public Collider
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
