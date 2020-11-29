#include "Physics.h"
#include "Engine/GameObject.h"
#include "Engine/Component/Physics/Rigidbody.h"
#include "Engine/Component/Physics/BoxCollider.h"
#include "Engine/Component/Physics/SphereCollider.h"
#include "Engine/Component/Transform.h"

namespace GameEngine
{
	Physics g_physics;

	static physx::PxDefaultErrorCallback g_physxErrorCallback;
	static physx::PxDefaultAllocator g_physxAllocatorCallback;

	static physx::PxFilterFlags PhysicsSimulationFilterShader (
		physx::PxFilterObjectAttributes attributes0, physx::PxFilterData filterData0,
		physx::PxFilterObjectAttributes attributes1, physx::PxFilterData filterData1,
		physx::PxPairFlags& pairFlags, const void* constantBlock, physx::PxU32 constantBlockSize)
	{
		pairFlags |= physx::PxPairFlag::eDETECT_DISCRETE_CONTACT | physx::PxPairFlag::eSOLVE_CONTACT;
		pairFlags |= physx::PxPairFlag::eNOTIFY_TOUCH_FOUND | physx::PxPairFlag::eNOTIFY_TOUCH_LOST | physx::PxPairFlag::eNOTIFY_TOUCH_PERSISTS;

		bool bTrigger = ((attributes0 & physx::PxFilterObjectFlag::eTRIGGER) != 0) || ((attributes1 & physx::PxFilterObjectFlag::eTRIGGER) != 0);

		if (bTrigger)
		{
			pairFlags &= ~physx::PxPairFlag::eSOLVE_CONTACT;
		}

		return physx::PxFilterFlag::eDEFAULT;
	}

	Physics::Physics () : m_physxFoundation (nullptr), m_physxPhysics (nullptr),
		m_physxCpuDispatcher (nullptr), m_physxScene (nullptr), m_physxDefaultMaterial (nullptr)
	{
	}

	Physics::~Physics ()
	{
	}

	bool Physics::Init ()
	{
		m_physxFoundation = PxCreateFoundation (PX_PHYSICS_VERSION, g_physxAllocatorCallback, g_physxErrorCallback);

		if (m_physxFoundation == nullptr)
		{
			return false;
		}

		m_physxPhysics = PxCreatePhysics (PX_PHYSICS_VERSION, *m_physxFoundation, physx::PxTolerancesScale ());

		if (m_physxPhysics == nullptr)
		{
			return false;
		}

		m_physxCpuDispatcher = physx::PxDefaultCpuDispatcherCreate (1);

		if (m_physxCpuDispatcher == nullptr)
		{
			return false;
		}
		
		m_physxDefaultMaterial = m_physxPhysics->createMaterial (0.5f, 0.5f, 0.1f);

		if (m_physxDefaultMaterial == nullptr)
		{
			return false;
		}

		return true;
	}

	void Physics::Shutdown ()
	{
		DeletePhysicScene ();

		m_physxCpuDispatcher->release ();
		m_physxPhysics->release ();
		m_physxFoundation->release ();
	}

	void Physics::Simulate (float timeStep)
	{
		if (m_physxScene == nullptr)
		{
			return;
		}

		m_physxScene->simulate (timeStep);
		m_physxScene->fetchResults (true);
	}

	void Physics::CreatePhysicScene ()
	{
		physx::PxSceneDesc physxSceneDesc (m_physxPhysics->getTolerancesScale ());
		physxSceneDesc.gravity = physx::PxVec3 (0.0f, -9.81f, 0.0f);
		physxSceneDesc.cpuDispatcher = m_physxCpuDispatcher;
		physxSceneDesc.filterShader = PhysicsSimulationFilterShader;
		physxSceneDesc.simulationEventCallback = &m_eventListener;

		m_physxScene = m_physxPhysics->createScene (physxSceneDesc);
	}

	void Physics::DeletePhysicScene ()
	{
		if (m_physxScene == nullptr)
		{
			return;
		}
		
		m_physxScene->release ();
		m_physxScene = nullptr;
	}

	void Physics::CreateRigidDynamic (Rigidbody& rigidbody, Vector3 position, Quaternion rotation)
	{
		physx::PxTransform physxTransform;
		physxTransform.p = physx::PxVec3 (position.m_x, position.m_y, position.m_z);
		physxTransform.q = physx::PxQuat (rotation.m_x, rotation.m_y, rotation.m_z, rotation.m_w);

		auto* physxRigidDynamic = m_physxPhysics->createRigidDynamic (physxTransform);
		m_physxScene->addActor (*physxRigidDynamic);

		m_rigidbodyMap.insert ({ rigidbody.m_physxRigidDynamic, &rigidbody });

		rigidbody.m_physxRigidDynamic = physxRigidDynamic;
	}

	void Physics::DeleteRigidDynamic (Rigidbody& rigidbody)
	{
		for (auto* collider : rigidbody.GetGameObject ().GetComponents<Collider> ())
		{
			DetachCollider (rigidbody, *collider);
		}

		m_rigidbodyMap.erase (rigidbody.m_physxRigidDynamic);
		m_physxScene->removeActor (*rigidbody.m_physxRigidDynamic);
	}

	void Physics::CreateRigidStatic (Collider& collider, Vector3 position, Quaternion rotation)
	{
		physx::PxTransform physxTransform;
		physxTransform.p = physx::PxVec3 (position.m_x, position.m_y, position.m_z);
		physxTransform.q = physx::PxQuat (rotation.m_x, rotation.m_y, rotation.m_z, rotation.m_w);

		auto* physxRigidStatic = m_physxPhysics->createRigidStatic (physxTransform);
		m_physxScene->addActor (*physxRigidStatic);

		collider.m_physxRigidStatic = physxRigidStatic;
		collider.m_physxRigidStatic->attachShape (*collider.m_physxShape);
	}

	void Physics::DeleteRigidStatic (Collider& collider)
	{
		collider.m_physxRigidStatic->detachShape (*collider.m_physxShape);

		m_physxScene->removeActor (*collider.m_physxRigidStatic);
	}

	void Physics::AttachCollider (Rigidbody& rigidbody, Collider& collider)
	{
		if (collider.m_attachedRigidbody != nullptr)
		{
			return;
		}

		if (collider.m_physxRigidStatic != nullptr)
		{
			DeleteRigidStatic (collider);
		}

		rigidbody.m_physxRigidDynamic->attachShape (*collider.m_physxShape);
		collider.m_attachedRigidbody = &rigidbody;
	}

	void Physics::DetachCollider (Rigidbody& rigidbody, Collider& collider)
	{
		if (collider.m_attachedRigidbody != &rigidbody)
		{
			return;
		}

		rigidbody.m_physxRigidDynamic->detachShape (*collider.m_physxShape);
		collider.m_attachedRigidbody = nullptr;
	}

	void Physics::CreateBoxCollider (BoxCollider& boxCollider)
	{
		Vector3 halfSize = boxCollider.GetSize () * 0.5f;

		physx::PxBoxGeometry physxBoxGeometry (halfSize.m_x, halfSize.m_y, halfSize.m_z);
		boxCollider.m_physxShape = m_physxPhysics->createShape (physxBoxGeometry, *m_physxDefaultMaterial, true);
	}

	void Physics::CreateSphereCollider (SphereCollider& sphereCollider)
	{
		physx::PxSphereGeometry physxSphereGeometry (sphereCollider.GetRadius ());
		sphereCollider.m_physxShape = m_physxPhysics->createShape (physxSphereGeometry, *m_physxDefaultMaterial, true);
	}

	Rigidbody* Physics::FindRigidbody (physx::PxRigidActor* physxRigidActor)
	{
		auto found = m_rigidbodyMap.find (physxRigidActor);

		if (found == m_rigidbodyMap.end ())
		{
			return nullptr;
		}

		return (*found).second;
	}

	void Physics::FireRigidbodyOnCollisionStartEvent (physx::PxRigidActor* physxRigidActor)
	{
		Rigidbody* rigidbody = FindRigidbody (physxRigidActor);

		if (rigidbody != nullptr)
		{
			rigidbody->OnCollisionStart ();
		}
	}

	void Physics::FireRigidbodyOnCollisionFinishEvent (physx::PxRigidActor* physxRigidActor)
	{
		Rigidbody* rigidbody = FindRigidbody (physxRigidActor);

		if (rigidbody != nullptr)
		{
			rigidbody->OnCollisionFinish ();
		}
	}

	void Physics::FireRigidbodyOnCollisionStayEvent (physx::PxRigidActor* physxRigidActor)
	{
		Rigidbody* rigidbody = FindRigidbody (physxRigidActor);

		if (rigidbody != nullptr)
		{
			rigidbody->OnCollisionStay ();
		}
	}

	void Physics::FireRigidbodyOnTriggerStartEvent (physx::PxRigidActor* physxRigidActor)
	{
		Rigidbody* rigidbody = FindRigidbody (physxRigidActor);

		if (rigidbody != nullptr)
		{
			rigidbody->OnTriggerStart ();
		}
	}

	void Physics::FireRigidbodyOnTriggerFinishEvent (physx::PxRigidActor* physxRigidActor)
	{
		Rigidbody* rigidbody = FindRigidbody (physxRigidActor);

		if (rigidbody != nullptr)
		{
			rigidbody->OnTriggerFinish ();
		}
	}

	void Physics::FireRigidbodyOnTriggerStayEvent (physx::PxRigidActor* physxRigidActor)
	{
		Rigidbody* rigidbody = FindRigidbody (physxRigidActor);

		if (rigidbody != nullptr)
		{
			rigidbody->OnTriggerStay ();
		}
	}
}