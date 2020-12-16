#include "PhysicsEventListener.h"
#include "Physics.h"

namespace GameEngine
{
	PhysicsEventListener::PhysicsEventListener ()
	{
	}

	PhysicsEventListener::~PhysicsEventListener ()
	{
	}

	void PhysicsEventListener::onConstraintBreak (physx::PxConstraintInfo* constraints, physx::PxU32 count)
	{
	}

	void PhysicsEventListener::onWake (physx::PxActor** actors, physx::PxU32 count)
	{
	}

	void PhysicsEventListener::onSleep (physx::PxActor** actors, physx::PxU32 count)
	{
	}

	void PhysicsEventListener::onContact (const physx::PxContactPairHeader& pairHeader, const physx::PxContactPair* pairs, physx::PxU32 nbPairs)
	{
		physx::PxRigidActor* rigidActor1 = pairHeader.actors[0];
		physx::PxRigidActor* rigidActor2 = pairHeader.actors[1];

		for (physx::PxU32 i = 0; i < nbPairs; i++)
		{
			const physx::PxContactPair& contactPair = pairs[i];

			if (contactPair.events & physx::PxPairFlag::eNOTIFY_TOUCH_FOUND)
			{
				g_physics.FireRigidbodyOnCollisionStartEvent (rigidActor1);
				g_physics.FireRigidbodyOnCollisionStartEvent (rigidActor2);
			}
			else if (contactPair.events & physx::PxPairFlag::eNOTIFY_TOUCH_LOST)
			{
				g_physics.FireRigidbodyOnCollisionFinishEvent (rigidActor1);
				g_physics.FireRigidbodyOnCollisionFinishEvent (rigidActor2);
			}
			else if (contactPair.events & physx::PxPairFlag::eNOTIFY_TOUCH_PERSISTS)
			{
				g_physics.FireRigidbodyOnCollisionStayEvent (rigidActor1);
				g_physics.FireRigidbodyOnCollisionStayEvent (rigidActor2);
			}
		}
	}

	void PhysicsEventListener::onTrigger (physx::PxTriggerPair* pairs, physx::PxU32 count)
	{
		for (physx::PxU32 i = 0; i < count; i++)
		{
			const physx::PxTriggerPair& triggerPair = pairs[i];

			physx::PxRigidActor* rigidActor1 = triggerPair.triggerActor;
			physx::PxRigidActor* rigidActor2 = triggerPair.otherActor;

			if (triggerPair.status & physx::PxPairFlag::eNOTIFY_TOUCH_FOUND)
			{
				g_physics.FireRigidbodyOnTriggerStartEvent (rigidActor1);
				g_physics.FireRigidbodyOnTriggerStartEvent (rigidActor2);
			}
			else if (triggerPair.status & physx::PxPairFlag::eNOTIFY_TOUCH_LOST)
			{
				g_physics.FireRigidbodyOnTriggerFinishEvent (rigidActor1);
				g_physics.FireRigidbodyOnTriggerFinishEvent (rigidActor2);
			}
			else if (triggerPair.status & physx::PxPairFlag::eNOTIFY_TOUCH_PERSISTS)
			{
				g_physics.FireRigidbodyOnTriggerStayEvent (rigidActor1);
				g_physics.FireRigidbodyOnTriggerStayEvent (rigidActor2);
			}
		}
	}

	void PhysicsEventListener::onAdvance (const physx::PxRigidBody* const * bodyBuffer, const physx::PxTransform* poseBuffer, const physx::PxU32 count)
	{
	}
}