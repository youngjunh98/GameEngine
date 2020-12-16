#ifndef INCLUDE_PHYSX
#define INCLUDE_PHYSX

#define NDEBUG

#if defined (_DEBUG) || defined (DEBUG)
#undef NDEBUG
#endif

#include <PxConfig.h>
#include <PxPhysicsAPI.h>

#endif