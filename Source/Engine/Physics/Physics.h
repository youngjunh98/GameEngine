#ifndef INCLUDE_PHYSICS
#define INCLUDE_PHYSICS

#include <unordered_map>

#include "Core/CoreMinimal.h"
#include "Engine/Physics/PhysX.h"
#include "Engine/Physics/PhysicsEventListener.h"

namespace GameEngine
{
	class Physics
	{
	public:
		Physics ();
		virtual ~Physics ();

		bool Init ();
		void Shutdown ();

		void Simulate (float timeStep);

		void CreatePhysicScene ();
		void DeletePhysicScene ();

		void CreateRigidDynamic (Rigidbody& rigidbody, Vector3 position, Quaternion rotation);
		void DeleteRigidDynamic (Rigidbody& rigidbody);

		void CreateRigidStatic (Collider& collider, Vector3 position, Quaternion rotation);
		void DeleteRigidStatic (Collider& collider);

		void AttachCollider (Rigidbody& rigidbody, Collider& collider);
		void DetachCollider (Rigidbody& rigidbody, Collider& collider);

		void CreateBoxCollider (BoxCollider& boxCollider);
		void CreateSphereCollider (SphereCollider& sphereCollider);

		void FireRigidbodyOnCollisionStartEvent (physx::PxRigidActor* physxRigidActor);
		void FireRigidbodyOnCollisionFinishEvent (physx::PxRigidActor* physxRigidActor);
		void FireRigidbodyOnCollisionStayEvent (physx::PxRigidActor* physxRigidActor);

		void FireRigidbodyOnTriggerStartEvent (physx::PxRigidActor* physxRigidActor);
		void FireRigidbodyOnTriggerFinishEvent (physx::PxRigidActor* physxRigidActor);
		void FireRigidbodyOnTriggerStayEvent (physx::PxRigidActor* physxRigidActor);

	private:
		Rigidbody* FindRigidbody (physx::PxRigidActor* physxRigidActor);

	private:
		std::unordered_map<physx::PxRigidActor*, Rigidbody*> m_rigidbodyMap;
		PhysicsEventListener m_eventListener;

		physx::PxFoundation* m_physxFoundation;
		physx::PxPhysics* m_physxPhysics;
		physx::PxDefaultCpuDispatcher* m_physxCpuDispatcher;
		physx::PxScene* m_physxScene;
		physx::PxMaterial* m_physxDefaultMaterial;
	};

	extern Physics g_physics;
}

#endif