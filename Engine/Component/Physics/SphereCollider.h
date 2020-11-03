#ifndef INCLUDE_SPHERE_COLLIDER
#define INCLUDE_SPHERE_COLLIDER

#include "Component/Physics/Collider.h"

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

		virtual void OnRenderEditor (Editor& editor) override;

	private:
		float m_radius;
	};
}

#endif