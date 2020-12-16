#pragma once

#include <vector>

#include "EngineMacro.h"

namespace GameEngine
{
	using PostRenderEvent = void (*) ();

	class ENGINE_API Engine final
	{
	private:
		Engine ();
		~Engine ();

	public:
		Engine (const Engine&) = delete;
		Engine& operator= (const Engine&) = delete;

		static Engine& GetInstance ();

		bool Init ();
		void Shutdown ();

		void Run ();

		void AddPostRenderCallback (PostRenderEvent callback);
		void RemovePostRenderCallback (PostRenderEvent callback);

	private:
		std::vector<PostRenderEvent> m_postRenderEvents;
	};
}
