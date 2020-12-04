#pragma once

#include "Engine/Component/Physics/Collider.h"

namespace GameEngine
{
	class SphereCollider : public Collider
	{
	public:
		SphereCollider ();
		virtual ~SphereCollider ();

		virtual void OnInit () override;

		void SetRadius (float radius);
		float GetRadius () const;

		virtual void OnRenderEditor () override;

	private:
		float m_radius;
	};
}
