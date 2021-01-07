#pragma once

#include "Engine/Core/CoreMinimal.h"
#include "Engine/Engine/EngineMacro.h"
#include "Engine/Engine/Physics/Physics.h"
#include "Engine/Engine/Component/Component.h"
#include "Engine/Engine/Component/Physics/Rigidbody.h"

namespace GameEngine
{
	class ENGINE_API Collider : public Component
	{
	public:
		friend Physics;

		Collider (const std::string& name);
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
		
		virtual void OnSerialize (Json::Json& json) const override;
		virtual void OnDeserialize (const Json::Json& json) override;

	protected:
		virtual physx::PxShape* GetShape () = 0;

	protected:
		bool m_bTrigger;
		Vector3 m_offset;
		Rigidbody* m_attachedRigidbody;

		physx::PxShape* m_physxShape;
		physx::PxRigidStatic* m_physxRigidStatic;
	};
}
