#pragma once

#include "Engine/Engine/EngineMacro.h"
#include "Engine/Engine/Component/Physics/Collider.h"

namespace GameEngine
{
	class ENGINE_API SphereCollider : public Collider
	{
	public:
		REGISTER_OBJECT_HEADER (SphereCollider)

		SphereCollider ();
		virtual ~SphereCollider ();

		virtual void OnInit () override;

		void SetRadius (float radius);
		float GetRadius () const;

		virtual void OnRenderEditor () override;

		virtual void OnSerialize (Json::Json& json) const override;
		virtual void OnDeserialize (const Json::Json& json) override;

	protected:
		virtual physx::PxShape* GetShape () override;

	private:
		float m_radius;
	};
}
