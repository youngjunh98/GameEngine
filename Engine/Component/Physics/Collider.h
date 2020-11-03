#ifndef INCLUDE_COLLIDER
#define INCLUDE_COLLIDER

#include "Core/CoreMinimal.h"
#include "Physics/Physics.h"
#include "Component/Component.h"
#include "Component/Physics/Rigidbody.h"

namespace GameEngine
{
	class Collider : public Component
	{
	public:
		friend Physics;

		Collider ();
		virtual ~Collider ();

		virtual void Destroy () override;

		virtual void OnInit () override;
		virtual void OnStart () override;

		virtual void OnFixedUpdate () override;

		bool GetTrigger () const;
		void SetTrigger (bool bIsTrigger);

		Vector3 GetOffset () const;
		void SetOffset (Vector3 offset);

		Rigidbody* GetAttachedRigidbody () const;
		void SetAttachedRigidbody (Rigidbody& rigidbody);

	protected:
		bool m_bTrigger;
		Vector3 m_offset;
		Rigidbody* m_attachedRigidbody;

		physx::PxShape* m_physxShape;
		physx::PxRigidStatic* m_physxRigidStatic;
	};
}

#endif