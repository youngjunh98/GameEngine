#ifndef INCLUDE_ENGINE
#define INCLUDE_ENGINE

#include "EngineMacro.h"

namespace GameEngine
{
	class ENGINE_API Engine
	{
	public:
		Engine () {}
		~Engine () {}

		bool Init ();
		void Shutdown ();

		void Run ();
	};

	ENGINE_API extern Engine g_engine;
}

#endif