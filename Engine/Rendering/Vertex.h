#ifndef INCLUDE_VERTEX
#define INCLUDE_VERTEX

#include "Core/CoreMinimal.h"

namespace GameEngine
{
	struct Vertex
	{
		Vector3 m_position;
		Vector2 m_uv;
		Vector3 m_normal;
		Vector3 m_tangent;
	};
}

#endif