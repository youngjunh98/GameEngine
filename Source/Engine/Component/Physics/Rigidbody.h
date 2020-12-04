#ifndef INCLUDE_RIGIDBODY
#define INCLUDE_RIGIDBODY

#include "Core/CoreMinimal.h"
#include "Engine/Component/Component.h"
#include "Engine/Physics/Physics.h"

namespace GameEngine
{
	enum class ERigidbodyConstraintAxis
	{
		X,
		Y,
		Z
	};

	class Rigidbody : public Component
	{
	public:
		friend Physics;

		Rigidbody ();
		virtual ~Rigidbody ();

		virtual void Destroy () override;

		virtual void OnInit () override;
		virtual void OnFixedUpdate () override;

		virtual void OnCollisionStart () override;
		virtual void OnCollisionFinish () override;
		virtual void OnCollisionStay () override;

		virtual void OnTriggerStart () override;
		virtual void OnTriggerFinish () override;
		virtual void OnTriggerStay () override;

		void AddForce (Vector3 force);

		Vector3 GetVelocity () const;
		void SetVelocity (Vector3 velocity);

		Vector3 GetAngularVelocity () const;
		void SetAngularVelocity (Vector3 angularVelocity);

		float GetMass () const;
		void SetMass (float mass);

		float GetLinearDamping () const;
		void SetLinearDamping (float linearDamping);

		float GetAngularDamping () const;
		void SetAngularDamping (float angularDamping);

		bool IsApplyGravity () const;
		void SetApplyGravity (bool bGravity);

		bool IsKinematic () const;
		void SetKinematic (bool bKinematic);

		bool IsConstraintPosition (ERigidbodyConstraintAxis axis) const;
		void SetConstraintPosition (bool bPositionX, bool bPositionY, bool bPositionZ);

		bool IsConstraintRotation (ERigidbodyConstraintAxis axis) const;
		void SetConstraintRotation (bool bRotationX, bool bRotationY, bool bRotationZ);

		virtual void OnRenderEditor () override;

		//virtual void OnSerialize (Json::Json& json) const override;
		//virtual void OnDeserialize (const Json::Json& json) override;

	private:
		float m_mass;

		float m_linearDamping;
		float m_angularDamping;

		bool m_bGravity;
		bool m_bKinematic;

		bool m_bConstraintPositionX;
		bool m_bConstraintPositionY;
		bool m_bConstraintPositionZ;

		bool m_bConstraintPitch;
		bool m_bConstraintYaw;
		bool m_bConstraintRoll;

		Vector3 m_velocity;
		Vector3 m_angularVelocity;

		Vector3 m_lastUpdatePosition;
		Quaternion m_lastUpdateRotation;

		physx::PxRigidDynamic* m_physxRigidDynamic;
	};
}

#endif